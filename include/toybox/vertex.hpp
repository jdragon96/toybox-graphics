#ifndef TOYBOX_VERTEX_H
#define TOYBOX_VERTEX_H

#include <string>
#include <vector>

namespace Toybox {
struct Vertex {
  //=> vertex geometry 좌표
  float x;
  float y;
  float z;

  //=> color
  float r;
  float g;
  float b;

  //=> normal
  float nx;
  float ny;
  float nz;

  //=> texture coordinate
  float tx;
  float ty;
};

struct Mesh {
  std::vector<Vertex> vertexes;
  std::vector<uint32_t> indexes;
};
} // namespace Toybox

#endif