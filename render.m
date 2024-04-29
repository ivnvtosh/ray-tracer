#import <Cocoa/Cocoa.h>
#import <OpenGL/gl3.h>

#define GL_SILENCE_DEPRECATION

@interface OpenGLView : NSOpenGLView

{
  int		width;
  int		height;
}

- (void) selectGLContext;
- (void) draw_image;

@end

struct s_app
{
  void  *app;
  int   width;
  int   height;
  int   *data;
  OpenGLView *view;

  GLuint vshader;
  GLuint fshader;
  GLuint program;

  GLint image_texture; 
  GLint image_pos;
  GLint image_size;
  GLint image_winhalfsize;
  GLint image_position;

  GLfloat		vertexes[8];

  GLuint texture;
  GLuint vbuffer;

  int (*update)();
};

typedef struct s_app t_app;

t_app app;

@implementation OpenGLView

- (void) selectGLContext
{
  if ([NSOpenGLContext currentContext] != [self openGLContext])
  {
    [[self openGLContext] makeCurrentContext];
  }
}

- (void) draw_image
{
  glUseProgram(app.program);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, app.texture);
  glUniform1i(app.image_texture, 0);

  glUniform2f(app.image_winhalfsize, app.width / 2, app.height / 2);
  glUniform2f(app.image_pos, 0, app.height);
  glUniform2f(app.image_size, app.width, -app.height);

  glBindBuffer(GL_ARRAY_BUFFER, app.vbuffer);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
  glEnableVertexAttribArray(0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
  glBlendEquation(GL_FUNC_ADD);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableVertexAttribArray(0);
}

@end

void create_app()
{
  app.app = [NSApplication sharedApplication];
}

void create_view()
{
  NSRect frame = NSMakeRect(0, 0, app.width, app.height);

  NSOpenGLPixelFormatAttribute pfa_attrs[] =
  {
    NSOpenGLPFADepthSize, 
    32,
    NSOpenGLPFAOpenGLProfile, 
    NSOpenGLProfileVersionLegacy,
    0
  };

  NSOpenGLPixelFormat* pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:pfa_attrs];

  app.view = [[OpenGLView alloc] initWithFrame:frame pixelFormat:pixFmt];

  [[app.view openGLContext] makeCurrentContext];
  [[app.view openGLContext] setView:app.view];
  [app.view prepareOpenGL];

  [app.view setNextKeyView:app.view];

  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();

  [pixFmt release];
}

void create_window()
{
  NSRect frame = NSMakeRect(0, 0, app.width, app.height);
  
  NSUInteger windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;

  NSWindow* win = [[NSWindow alloc] initWithContentRect:frame
                                    styleMask:windowStyle
                                    backing:NSBackingStoreBuffered
                                    defer:NO];

  [win setContentView:app.view];
  [win setTitle:@"Cornell Box"];
  [win makeKeyAndOrderFront:app.view];
}

void create_shaders_image()
{
  char  *source;
  int	length;
  GLint action_ok;

  app.vshader = glCreateShader(GL_VERTEX_SHADER);
  source = "#version 110\n"
		  "attribute vec2 position;"
		  "uniform vec2 winhalfsize;"
		  "uniform vec2 imagepos;"
		  "uniform vec2 imagesize;"
		  "varying vec2 texcoord;"
		  "void main()"
		  "{"
		  " texcoord = position / imagesize;"
		  " vec2 pos = position - winhalfsize + imagepos;"
		  " pos = pos / winhalfsize;"
		  " gl_Position = vec4(pos, 0.0, 1.0);"
		  "}";
  length = strlen(source);
  glShaderSource(app.vshader, 1, (const GLchar**)&source, &length);
  glCompileShader(app.vshader);

  glGetShaderiv(app.vshader, GL_COMPILE_STATUS, &action_ok);
  if (!action_ok) {
    exit(1);
  }

  app.fshader = glCreateShader(GL_FRAGMENT_SHADER);
  source = "#version 110 \n"
		  "uniform sampler2D texture;"
		  "varying vec2 texcoord;"
		  "void main()"
		  "{"
		  " gl_FragColor = texture2D(texture, texcoord);"
		  "}";
  length = strlen(source);
  glShaderSource(app.fshader, 1, (const GLchar**)&source, &length);
  glCompileShader(app.fshader);

  glGetShaderiv(app.fshader, GL_COMPILE_STATUS, &action_ok);
  if (!action_ok) {
    exit(1);
  }

  app.program = glCreateProgram();
  glAttachShader(app.program, app.vshader);
  glAttachShader(app.program, app.fshader);
  glLinkProgram(app.program);

  glGetProgramiv(app.program, GL_LINK_STATUS, &action_ok);
  if (!action_ok) {
    exit(1);
  }

  glFlush();
}

void create()
{
  create_shaders_image();

  glUseProgram(app.program);
  
  app.image_texture = glGetUniformLocation(app.program, "texture");
  app.image_pos = glGetUniformLocation(app.program, "imagepos");
  app.image_size = glGetUniformLocation(app.program, "imagesize");
  app.image_winhalfsize = glGetUniformLocation(app.program, "winhalfsize");
  app.image_position = glGetAttribLocation(app.program, "position");

  glFlush();
}

void image()
{
  app.vertexes[0] = 0.0;
  app.vertexes[1] = 0.0;
  app.vertexes[2] = app.width;
  app.vertexes[3] = 0.0;
  app.vertexes[4] = app.width;
  app.vertexes[5] = -app.height;
  app.vertexes[6] = 0.0;
  app.vertexes[7] = -app.height;
}

void create_texture()
{
  glGenTextures(1, &app.texture);
  glBindTexture(GL_TEXTURE_2D, app.texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA8,
    app.width,
    app.height,
    0,
    GL_BGRA,
    GL_UNSIGNED_BYTE,
    app.data
  );

  glGenBuffers(1, &app.vbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, app.vbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(app.vertexes), app.vertexes, GL_DYNAMIC_DRAW);
}

void update_texture()
{
  glBindTexture(GL_TEXTURE_2D, app.texture);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA8,
    app.width, 
    app.height, 
    0,
	  GL_BGRA,
    GL_UNSIGNED_BYTE,
    app.data
  );
}

void put()
{
  [app.view selectGLContext];

  create_texture();

  [app.view draw_image];
}

void do_loop_flush()
{
  app.update();
  [app.view selectGLContext];
  update_texture();
  [app.view draw_image];
  glFlush(); 
}

void loop()
{
  CFRunLoopObserverRef observer;
  CFRunLoopObserverContext ocontext = {
    .version = 0, 
    .info = NULL,
    .retain = NULL, 
    .release = NULL, 
    .copyDescription = NULL
  };

  observer = CFRunLoopObserverCreate(NULL, kCFRunLoopBeforeTimers, true, 0, do_loop_flush, &ocontext);
  CFRunLoopAddObserver(CFRunLoopGetMain(), observer, kCFRunLoopCommonModes);

  [NSApp run];
}

void render(int *data, int (*update)())
{
  app.width = 512;
  app.height = 512;
  app.data = data;
  app.update = update;
  
  create_app();
  create_view();
  create_window();
  create();
  image();

  put();

  glFlush(); 

  loop();
}
