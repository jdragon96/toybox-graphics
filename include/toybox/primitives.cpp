#include "toybox/primitives.hpp"
#include "toybox/utils.hpp"
#include <exception>
#include <stdexcept>

using namespace Toybox;

Toybox::Mesh Primitives::MakeCube(CoordSystemEnum system, float sideLength) {
  /*******************************************************
  아래와 같은 vertex index로 정육각형 큐브를 생성한다.

  +y      +z
  |      /
  | 5---------6
  |/|  /     /|
  / | /     / |
  1--------2  |
  | 4------|--8
  |/       | /
  0--------3 ------- +x (왼손좌표계 기준)

  - vertex: 6 * 4 = 24개
  - indexes: 6 * 6 = 36개
  *********************************************************/
  //> 1. create vertexes
  int plainLength = 6;
  int dim = 3;
  int planePerDim = 2;
  Toybox::Mesh mesh;
  std::vector<Toybox::Vertex> vertexes;
  std::vector<unsigned int> indexes;

  float PI = 3.141592;
  float PI_DIV_FOUR = 3.141592 / 4;
  float PI_DIV_TWO = 3.141592 / 2;

  std::vector<int> xList{-1, -1, 1, 1};
  std::vector<int> yList{-1, 1, 1, -1};
  std::vector<int> txList{0, 0, 1, 1};
  std::vector<int> tyList{1, 0, 0, 1};
  std::vector<int> reversePlane{-1, 1};
  float x, y;

  //> 축 세개
  for (int i = 0; i < dim; ++i) {

    //=> 앞, 뒤
    for (auto rev : reversePlane) {

      //=> 면 하나당 네 개의 vertex 생성
      for (int k = 0; k < 4; ++k) {
        Toybox::Vertex vtx;

        if (system == CoordSystemEnum::LEFTHAND) {
          x = xList[k] * (rev * (-1));
          y = yList[k];
        } else {
          y = xList[k] * (rev * (-1));
          x = yList[k];
        }

        //기준축에 대한 값을 채운다.
        switch (i) {
        case 0: // x축
          vtx.x = rev;
          vtx.y = x;
          vtx.z = y;
          // vtx.nx = rev;
          // vtx.ny = 0;
          // vtx.nz = 0;
          break;
        case 1: // y축
          vtx.y = rev;
          vtx.z = x;
          vtx.x = y;
          // vtx.nx = 0;
          // vtx.ny = rev;
          // vtx.nz = 0;
          break;
        case 2: // z축
          vtx.z = rev;
          vtx.x = x;
          vtx.y = y;
          // vtx.nx = 0;
          // vtx.ny = 0;
          // vtx.nz = rev;
          break;
        }

        // 컬러값 매핑.. 랜덤으로
        vtx.r = Utils::instance().GetUniformNum();
        vtx.g = Utils::instance().GetUniformNum();
        vtx.b = Utils::instance().GetUniformNum();

        // texture 좌표값 매핑
        vtx.tx = txList[k];
        vtx.ty = tyList[k];

        vtx.x *= (sideLength / 2.0f);
        vtx.y *= (sideLength / 2.0f);
        vtx.z *= (sideLength / 2.0f);
        vtx.nx = vtx.x;
        vtx.ny = vtx.y;
        vtx.nz = vtx.z;
        vertexes.push_back(vtx);
      }
    }
  }

  //> index 생성
  for (int planeIndex = 0; planeIndex < 6; ++planeIndex) {
    // upper triangle
    indexes.push_back(planeIndex * 4);
    indexes.push_back(planeIndex * 4 + 1);
    indexes.push_back(planeIndex * 4 + 2);
    // lower triangle
    indexes.push_back(planeIndex * 4);
    indexes.push_back(planeIndex * 4 + 2);
    indexes.push_back(planeIndex * 4 + 3);
  }

  mesh.vertexes = vertexes;
  mesh.indexes = indexes;

  return mesh;
}

Toybox::Mesh Primitives::MakeGrid(CoordSystemEnum system, int xGridLength,
                                  int yGridLength, float gridSize) {
  /*******************************************************
  아래와 같은 바둑판 형식의 grid를 생성한다.

  +y    +z
  |    /
  |   /
  3------4------5
  | /    |      |
  |/     |      |
  0------1------2----- +x

  - vertex 개수: (y grid 개수 + 1) * (x grid 개수 + 1)
  - index 개수: y grid 개수 * x 그리드 개수 * 2
  *********************************************************/
  Toybox::Mesh mesh;
  std::vector<Toybox::Vertex> vertexes;
  std::vector<unsigned int> indexes;
  int indexLength = static_cast<int>(xGridLength * yGridLength);

  //> vertex 정보 생성하기
  for (int y = 0; y <= yGridLength; ++y) {
    for (int x = 0; x <= xGridLength; ++x) {
      Toybox::Vertex v;

      //=> geometry
      v.x = gridSize * x;
      v.y = gridSize * y;
      v.z = 0.0f;

      //=> color
      v.r = Utils::instance().GetUniformNum();
      v.g = Utils::instance().GetUniformNum();
      v.b = Utils::instance().GetUniformNum();

      //=> normal
      v.nx = 0;
      v.ny = 0;
      v.nz = system == CoordSystemEnum::LEFTHAND ? -1 : 1;

      //=> texture coord... 어떻게 해야할까
      // 좌측 하단(0,1)부터 시작
      v.tx = static_cast<float>(float(x) / float(yGridLength));
      v.ty = static_cast<float>(1 - (float(y) / float(yGridLength)));
      vertexes.push_back(v);
    }
  }

  //> index 정보 생성하기
  uint32_t xVertexLength = xGridLength + 1;
  for (uint32_t y = 0; y < yGridLength; ++y) {
    uint32_t upperY = (y + 1) * xVertexLength;
    uint32_t crntY = y * xVertexLength;
    for (uint32_t x = 0; x < xGridLength; ++x) {
      uint32_t nextX = x + 1;

      // upper triangle
      indexes.push_back(crntY + x);
      indexes.push_back(upperY + x);
      indexes.push_back(upperY + x + 1);
      // lower triangle
      indexes.push_back(crntY + x);
      indexes.push_back(upperY + x + 1);
      indexes.push_back(crntY + x + 1);
    }
  }

  mesh.vertexes = vertexes;
  mesh.indexes = indexes;
  return mesh;
}

Toybox::Mesh Primitives::MakeCylinder(CoordSystemEnum system, float radius,
                                      float height, float unitAngle) {
  /*******************************************************
  아래와 같은 바둑판 형식의 grid를 생성한다.

     +x
      |
      |
     2  1
   3      0 ---- + z
     4  5

  - vertex 개수: (y grid 개수 + 1) * (x grid 개수 + 1)
  - index 개수: y grid 개수 * x 그리드 개수 * 2
  *********************************************************/
  Toybox::Mesh mesh;
  std::vector<Toybox::Vertex> vertexes;
  std::vector<unsigned int> indexes;
  float unitRadian = unitAngle / 180.0f * 3.141592;
  int circleVertexLength = static_cast<int>(360.0f / unitAngle);

  //> vertex 정보 생성하기
  for (int y = 0; y < 2; ++y) {
    for (int x = 0; x < circleVertexLength; ++x) {
      Toybox::Vertex v;

      //=> Normal vector 기준, 반시계 방향으로 회전한다.
      v.x = sin(unitRadian * x);
      v.y = height * y;
      v.z = cos(unitRadian * x);

      //=> color
      v.r = Utils::instance().GetUniformNum();
      v.g = Utils::instance().GetUniformNum();
      v.b = Utils::instance().GetUniformNum();

      //=> normal
      // v.nx = 0;
      // v.ny = y == 0 ? -1 : 1;
      // v.nz = 0;

      v.nx = v.x;
      v.ny = v.y;
      v.nz = v.z;
      //=> texture coord... 어떻게 해야할까
      // 좌측 하단(0,1)부터 시작
      v.tx = static_cast<float>(float(x) / float(circleVertexLength - 1));
      v.ty = static_cast<float>(1 - y);
      vertexes.push_back(v);
    }
  }

  //> index 정보 생성하기
  int nextX;
  for (int x = 0; x < circleVertexLength; ++x) {
    nextX = (x + 1) == circleVertexLength ? 0 : (x + 1);
    // upper triangle
    indexes.push_back(x);
    indexes.push_back(circleVertexLength + x);
    indexes.push_back(circleVertexLength + nextX);
    // lower triangle
    indexes.push_back(x);
    indexes.push_back(circleVertexLength + nextX);
    indexes.push_back(nextX);
  }

  mesh.vertexes = vertexes;
  mesh.indexes = indexes;
  return mesh;
}

// Toybox::Mesh Primitives::MakeSphere(CoordSystemEnum system) {
//  /*******************************************************
//  구체 객체를 생성한다.
//
//  6
//  | 5
//  |   4
//  |   3
//  |   2
//  | 1
//  0
//  - vertex 개수: (y grid 개수 + 1) * (x grid 개수 + 1)
//  - index 개수: y grid 개수 * x 그리드 개수 * 2
//  *********************************************************/
//  Toybox::Mesh mesh;
//  std::vector<Toybox::Vertex> vertexes;
//  std::vector<uint32_t> indexes;
//
//  float PI = 3.141592;
//  int vtxStack = 10;
//  int horizonStack = 20;
//  float vtxUnit = 2.0f / vtxStack;
//  float vtxUnitRad = PI / vtxStack;
//  float horizonUnitRad = (PI * 2.0f) / horizonStack;
//
//  //> vertex 정보 생성하기
//  for (int y = 0; y <= vtxStack; ++y) {
//    float crntY = y * vtxUnit - 1;
//    for (int x = 0; x <= horizonStack; ++x) {
//      Toybox::Vertex v;
//      //=> Normal vector 기준, 반시계 방향으로 회전한다.
//      v.x = cos(horizonUnitRad * x) * sin(vtxUnitRad * y);
//      v.y = crntY;
//      v.z = sin(horizonUnitRad * x) * sin(vtxUnitRad * y);
//
//      //=> color
//      v.r = Utils::instance().GetUniformNum();
//      v.g = Utils::instance().GetUniformNum();
//      v.b = Utils::instance().GetUniformNum();
//
//      //=> normal
//      v.nx = v.x;
//      v.ny = v.y;
//      v.nz = v.z;
//
//      //=> texture coord... 어떻게 해야할까
//      // 좌측 하단(0,1)부터 시작
//      v.tx = static_cast<float>(float(x) / float(horizonStack));
//      v.ty = static_cast<float>(float(y) / float(vtxStack));
//      vertexes.push_back(v);
//    }
//  }
//
//  //> index 정보 생성하기
//  int nextX;
//  for (int y = 0; y < vtxStack; ++y) {
//    int crntY = y * (horizonStack + 1);
//    int nextY = (y + 1) * (horizonStack + 1);
//    for (int x = 0; x < horizonStack; ++x) {
//      //> Upper triangle
//      indexes.push_back(crntY + x);
//      indexes.push_back(nextY + x);
//      indexes.push_back(nextY + x + 1);
//      //> Lower triangle
//      indexes.push_back(crntY + x);
//      indexes.push_back(nextY + x + 1);
//      indexes.push_back(crntY + x + 1);
//    }
//  }
//
//  mesh.vertexes = vertexes;
//  mesh.indexes = indexes;
//  return mesh;
//}

Toybox::Mesh Primitives::MakeSandClock(CoordSystemEnum system) {
  /*******************************************************
  구체 객체를 생성한다.

  6
  | 5
  |   4
  |   3
  |   2
  | 1
  0
  - vertex 개수: (y grid 개수 + 1) * (x grid 개수 + 1)
  - index 개수: y grid 개수 * x 그리드 개수 * 2
  *********************************************************/
  Toybox::Mesh mesh;
  std::vector<Toybox::Vertex> vertexes;
  std::vector<unsigned int> indexes;

  float PI = 3.141592;
  int vtxStack = 10;
  int horizonStack = 20;
  float vtxUnit = 2.0f / vtxStack;
  float vtxUnitRad = PI / vtxStack;
  float horizonUnitRad = (PI * 2.0f) / horizonStack;

  //> vertex 정보 생성하기
  for (int y = 0; y <= vtxStack; ++y) {
    float crntY = y * vtxUnit - 1;
    for (int x = 0; x <= horizonStack; ++x) {
      Toybox::Vertex v;
      //=> Normal vector 기준, 반시계 방향으로 회전한다.
      v.x = cos(horizonUnitRad * x) * cos(vtxUnitRad * y);
      v.y = crntY;
      v.z = sin(horizonUnitRad * x) * cos(vtxUnitRad * y);

      //=> color
      v.r = Utils::instance().GetUniformNum();
      v.g = Utils::instance().GetUniformNum();
      v.b = Utils::instance().GetUniformNum();

      //=> normal
      v.nx = v.x;
      v.ny = v.y;
      v.nz = v.z;

      //=> texture coord... 어떻게 해야할까
      // 좌측 하단(0,1)부터 시작
      v.tx = static_cast<float>(float(x) / float(horizonStack));
      v.ty = static_cast<float>(float(y) / float(vtxStack));
      vertexes.push_back(v);
    }
  }

  //> index 정보 생성하기
  int nextX;
  for (int y = 0; y < vtxStack; ++y) {
    int crntY = y * (horizonStack + 1);
    int nextY = (y + 1) * (horizonStack + 1);
    for (int x = 0; x < horizonStack; ++x) {
      //> Upper triangle
      indexes.push_back(crntY + x);
      indexes.push_back(nextY + x);
      indexes.push_back(nextY + x + 1);
      //> Lower triangle
      indexes.push_back(crntY + x);
      indexes.push_back(nextY + x + 1);
      indexes.push_back(crntY + x + 1);
    }
  }

  mesh.vertexes = vertexes;
  mesh.indexes = indexes;
  return mesh;
}
//
// Toybox::Mesh Primitives::MakeSphere(
//  CoordSystemEnum system,
//  const float radius,
//  const int numSlices,
//  const int numStacks)
//{
//  const float dTheta = -(3.141592 * 2) / float(numSlices);
//  const float dPhi = -(3.141592) / float(numStacks);
//
//  Mesh meshData;
//
//  std::vector<Vertex> &vertices = meshData.vertexes;
//
//  for (int j = 0; j <= numStacks; j++) {
//    float angle = dPhi * j;
//
//    // 스택에 쌓일 수록 시작점을 x-y 평면에서 회전 시켜서 위로 올리는 구조
//    float x = -sin(angle) * radius;
//    float y = cos(angle) * radius;
//    float z = 0.0f;
//
//    for (int i = 0; i <= numSlices; i++) {
//      Vertex vtx;
//      angle = dTheta * float(i);
//      vtx.x = cos(angle) * x;
//      vtx.y = y;
//      vtx.z = -sin(angle) * x;
//      //v.position = Vector3::Transform(
//      //    stackStartPoint, Matrix::CreateRotationY(dTheta * float(i)));
//
//      v.normal = v.position; // 원점이 구의 중심
//      v.normal.Normalize();
//      v.texcoord = Vector2(float(i) / numSlices, 1.0f - float(j) / numStacks);
//
//      vertices.push_back(v);
//    }
//  }
//
//  // cout << vertices.size() << endl;
//
//  vector<uint32_t> &indices = meshData.indices;
//
//  for (int j = 0; j < numStacks; j++) {
//
//    const int offset = (numSlices + 1) * j;
//
//    for (int i = 0; i < numSlices; i++) {
//
//      indices.push_back(offset + i);
//      indices.push_back(offset + i + numSlices + 1);
//      indices.push_back(offset + i + 1 + numSlices + 1);
//
//      indices.push_back(offset + i);
//      indices.push_back(offset + i + 1 + numSlices + 1);
//      indices.push_back(offset + i + 1);
//    }
//  }
//}

Toybox::Mesh Primitives::MakeSphere(const float radius, const int numSlice,
                                    const int numStack) {
  const float dTheta = -(3.141592 * 2) / float(numSlice);
  const float dPhi = -3.141592 / float(numStack);
  Toybox::Mesh object;

  for (int i = 0; i <= numStack; ++i) {
    Vertex stackStartPoint;
    stackStartPoint.x = (-radius) * (-sin(dPhi * i));
    stackStartPoint.y = (-radius) * cos(dPhi * i);
    stackStartPoint.z = 0;

    for (int j = 0; j <= numSlice; ++j) {
      Vertex vtx;
      vtx.x = stackStartPoint.x * cos(dTheta * j);
      vtx.y = stackStartPoint.y;
      vtx.z = stackStartPoint.x * (-sin(dTheta * j));
      float nFactor = sqrt(pow(vtx.x, 2) + pow(vtx.y, 2) + pow(vtx.z, 2));
      vtx.nx = vtx.x / nFactor;
      vtx.ny = vtx.y / nFactor;
      vtx.nz = vtx.z / nFactor;
      vtx.tx = float(j) / numSlice;
      vtx.ty = 1.0f - float(i) / numStack;
      object.vertexes.push_back(vtx);
    }
  }

  for (int j = 0; j < numStack; j++) {
    const int offset = (numSlice + 1) * j;
    for (int i = 0; i < numSlice; i++) {
      object.indexes.push_back(offset + i);
      object.indexes.push_back(offset + i + numSlice + 1);
      object.indexes.push_back(offset + i + 1 + numSlice + 1);

      object.indexes.push_back(offset + i);
      object.indexes.push_back(offset + i + 1 + numSlice + 1);
      object.indexes.push_back(offset + i + 1);
    }
  }

  return object;
}

Toybox::Mesh Primitives::MakeSquare() {
  Toybox::Mesh mesh;

  std::vector<float> xList = {-1.0f, 1.0f, 1.0f, -1.0f};
  std::vector<float> yList = {1.0f, 1.0f, -1.0f, -1.0f};
  std::vector<float> xTexcoordList = {0.0f, 1.0f, 1.0f, 0.0f};
  std::vector<float> yTexcoordList = {0.0f, 0.0f, 1.0f, 1.0f};
  std::vector<int> indices = {0, 1, 2, 0, 2, 3};

  for (int i = 0; i < 4; ++i) {
    Toybox::Vertex vtx;
    vtx.x = xList[i];
    vtx.y = yList[i];
    vtx.z = 0.0f;
    vtx.r = 0.0f;
    vtx.g = 0.0f;
    vtx.b = 1.0f;
    vtx.nx = 0.0f;
    vtx.ny = 0.0f;
    vtx.nz = -1.0f;
    vtx.tx = xTexcoordList[i];
    vtx.ty = yTexcoordList[i];
    mesh.vertexes.push_back(vtx);
  }

  for (auto it : indices) {
    mesh.indexes.push_back(it);
  }

  return mesh;
}

Toybox::Mesh Primitives::MakeAxis() {
  Toybox::Mesh object;

  Vertex red_origin;
  red_origin.x = 0.0f;
  red_origin.y = 0.0f;
  red_origin.z = 0.0f;
  red_origin.r = 1.0f;
  red_origin.g = 0.0f;
  red_origin.b = 0.0f;

  Vertex red;
  red.x = 1.0f;
  red.y = 0.0f;
  red.z = 0.0f;
  red.r = 1.0f;
  red.g = 0.0f;
  red.b = 0.0f;

  Vertex green_origin;
  green_origin.x = 0.0f;
  green_origin.y = 0.0f;
  green_origin.z = 0.0f;
  green_origin.r = 0.0f;
  green_origin.g = 1.0f;
  green_origin.b = 0.0f;

  Vertex green;
  green.x = 0.0f;
  green.y = 1.0f;
  green.z = 0.0f;
  green.r = 0.0f;
  green.g = 1.0f;
  green.b = 0.0f;

  Vertex blue_origin;
  blue_origin.x = 0.0f;
  blue_origin.y = 0.0f;
  blue_origin.z = 0.0f;
  blue_origin.r = 0.0f;
  blue_origin.g = 0.0f;
  blue_origin.b = 1.0f;

  Vertex blue;
  blue.x = 0.0f;
  blue.y = 0.0f;
  blue.z = 1.0f;
  blue.r = 0.0f;
  blue.g = 0.0f;
  blue.b = 1.0f;

  object.vertexes.push_back(red_origin);
  object.vertexes.push_back(red);
  object.vertexes.push_back(green_origin);
  object.vertexes.push_back(green);
  object.vertexes.push_back(blue_origin);
  object.vertexes.push_back(blue);

  std::vector<uint32_t> indexes = {0, 1, 2, 3, 4, 5};
  object.indexes = indexes;
  return object;
}

Toybox::Mesh Primitives::MakeFrustum(std::vector<float> origin,
                                     float fovDegHeight, float fovDegWidth,
                                     float farPlaneDistance) {
  if (origin.size() != 3)
    throw std::runtime_error("origin 좌표는 3차원 입력이 필요합니다.");

  float half_height_far_plane =
      Utils::instance().CalcLengthBasedOnFoV(fovDegHeight, farPlaneDistance) /
      2.f;
  float half_width_far_plane =
      Utils::instance().CalcLengthBasedOnFoV(fovDegWidth, farPlaneDistance) /
      2.f;

  //> Far plane 포인트 생성하기
  std::vector<float> xList = {-half_width_far_plane, half_width_far_plane,
                              half_width_far_plane, -half_width_far_plane};
  std::vector<float> yList = {half_height_far_plane, half_height_far_plane,
                              -half_height_far_plane, -half_height_far_plane};
  std::vector<float> xTexcoordList = {0.0f, 1.0f, 1.0f, 0.0f};
  std::vector<float> yTexcoordList = {0.0f, 0.0f, 1.0f, 1.0f};

  Toybox::Mesh object;
  for (int i = 0; i < 4; ++i) {
    Toybox::Vertex vtx;
    vtx.x = xList[i];
    vtx.y = yList[i];
    vtx.z = farPlaneDistance;
    vtx.r = 1.0f;
    vtx.g = 1.0f;
    vtx.b = 1.0f;
    vtx.tx = xTexcoordList[i];
    vtx.ty = yTexcoordList[i];
    object.vertexes.push_back(vtx);
  }

  //> Origin to Plane 직선 생성하기
  Toybox::Vertex vtx;
  vtx.x = origin[0];
  vtx.y = origin[1];
  vtx.z = origin[2];
  vtx.r = 1.0f;
  vtx.g = 1.0f;
  vtx.b = 1.0f;
  object.vertexes.push_back(vtx);

  std::vector<uint32_t> indexes = {0, 1, 1, 2, 2, 3, 3, 0,
                                   4, 0, 4, 1, 4, 2, 4, 3};
  object.indexes = indexes;
  return object;
}
