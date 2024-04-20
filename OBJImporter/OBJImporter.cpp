#include "OBJImporter.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace Engine;

float seadx = 0.9;
float seady = 0.9;
float seadz = 0.9;

OBJRequest& OBJRequest::Filepath(std::string filepath) noexcept {
  this->filepath = filepath;
  return *this;
}

OBJRequest& OBJRequest::Filename(std::string filename) noexcept {
  this->filename = filename;
  return *this;
}

TriangleMesh OBJImporter::Import(OBJRequest request) {
  path = request.filepath + "/";

  std::ifstream file(path + request.filename + ".obj");

  std::string line;
  std::string keyword;
  while (std::getline(file, line)) {
    std::istringstream iss(line);

    iss >> keyword;

    if (keyword == "v") {
      AddVertex(iss);

    } else if (keyword == "f") {
      AddFace(iss);

    } else if (keyword == "mtllib") {
      std::string filename;
      iss >> filename;
      AddMaterials(filename);

    } else if (keyword == "usemtl") {
      iss >> currentMaterial;
    }
  }

  return triangles;
}

void OBJImporter::AddVertex(std::istringstream& line) {
  Vector3 vertex;
  line >> vertex.x >> vertex.y >> vertex.z;
  vertices.push_back(vertex);
}

void OBJImporter::AddFace(std::istringstream& line) {
  Triangle triangle;

  std::string faceData;
  int index;
  for (int i = 0; i < 3; i += 1) {
    line >> faceData;
    std::istringstream faceIss(faceData);
    faceIss >> index;
    triangle.vertices[i] = vertices[index - 1];
  }
  seadx *= 0.9;
  seady *= 0.95;
  seadz *= 0.93;
  triangle.material = materials[currentMaterial];
  triangle.material.color.x *= seadx;
  triangle.material.color.y *= seady;
  triangle.material.color.z *= seadz;
  triangles.push_back(triangle);
}

void OBJImporter::AddMaterials(std::string& filename) {
  std::ifstream file(path + filename);

  Material currentMaterial;
  std::string line;
  std::string keyword;
  while (std::getline(file, line)) {
    std::istringstream iss(line);

    iss >> keyword;

    if (keyword == "newmtl") {
      if (!currentMaterial.name.empty()) {
        materials[currentMaterial.name] = currentMaterial;
      }

      iss >> currentMaterial.name;

    } else if (keyword == "Kd") {
      Vector3 color;
      iss >> color.x >> color.y >> color.z;
      currentMaterial.color = color;
    }
  }

  if (!currentMaterial.name.empty()) {
    materials[currentMaterial.name] = currentMaterial;
  }
}
