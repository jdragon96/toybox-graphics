#ifndef TOYBOX_PRIMITIVES_H
#define TOYBOX_PRIMITIVES_H

#include <chrono>
#include <random>
#include <toybox/vertex.hpp>
#include <vector>

namespace Toybox {

enum CoordSystemEnum { LIGHTHAND, LEFTHAND };

class Primitives {
public:
public:
  //! Cube 객체를 생성한다.
  static Toybox::Mesh MakeCube(CoordSystemEnum system, float sideLength);

  //! Square 객체를 생성한다.
  // static Toybox::Mesh MakeSquareLeft();

  //! Cylinder 객체를 생성한다.
  static Toybox::Mesh MakeCylinder(CoordSystemEnum system, float radius,
                                   float height, float unitAngle);

  //! Grid 형태의 판을 생성한다.
  static Toybox::Mesh MakeGrid(CoordSystemEnum system, int xGridLength,
                               int yGridLength, float gridSize);

  //! 구체 만들기
  //static Toybox::Mesh MakeSphere(CoordSystemEnum system);
  //static Toybox::Mesh MakeSphere(
  //  CoordSystemEnum system, 
  //  const float radius,
  //  const int numSlices, 
  //  const int numStacks);

  //! 모래시계 모형 만들기
  static Toybox::Mesh MakeSandClock(CoordSystemEnum system);

  static Toybox::Mesh MakeSphere(const float radius, const int sumSlice,
                                 const int numStack);

  static Toybox::Mesh MakeSquare();

  static Toybox::Mesh MakeAxis();
  
  static Toybox::Mesh MakeFrustum(std::vector<float> origin, float fovDegHeight, float fovDegWidth, float farPlaneDistance);

  //! Subdivision
  // static Toybox::Mesh MakeSubdivision(Toybox::Mesh& primitive);
};
} // namespace Toybox

#endif