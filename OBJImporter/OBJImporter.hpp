#ifndef OBJIMPORTER_HPP
#define OBJIMPORTER_HPP

#include <vector>

#include "TriangleMesh.hpp"

namespace Engine {

struct OBJRequest {
 public:
  std::string filepath;
  std::string filename;

  OBJRequest& Filepath(std::string filepath) noexcept;
  OBJRequest& Filename(std::string filename) noexcept;
};

class OBJImporter {
 public:
  TriangleMesh Import(OBJRequest);

 private:
  std::string path;

  using Vertices = std::vector<Vector3>;
  using Materials = std::unordered_map<std::string, Material>;
  using Triangles = std::vector<Triangle>;

  Vertices vertices;
  Materials materials;
  Triangles triangles;

  std::string currentMaterial;

  void AddVertex(std::istringstream& line);
  void AddFace(std::istringstream& line);
  void AddMaterials(std::string& filename);
};

}  // namespace Engine

#endif  // OBJIMPORTER_HPP
