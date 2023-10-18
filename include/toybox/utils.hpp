#ifndef TOYBOX_UTILS_H
#define TOYBOX_UTILS_H

#include <chrono>
#include <random>
#include <toybox/vertex.hpp>
#include <vector>

namespace Toybox {

// https://stackoverflow.com/questions/9878965/rand-between-0-and-1
class Utils {

public:
  Utils() {
    auto seed = GetSeed();
    this->timeSeed =
        std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
    seed.seed(ss);
  }

  static Utils &instance() {
    static Utils inst;
    return inst;
  }

  std::mt19937_64 &GetSeed() {
    static std::mt19937_64 rng;
    return rng;
  }

  std::uniform_real_distribution<double> &GetUniformSeed() {
    static std::uniform_real_distribution<double> unif(0, 1);
    return unif;
  }

  //! Normal distribution 중 랜덤한 수를 조회한다.
  double GetUniformNum() { return GetUniformSeed()(GetSeed()); }

  //! FoV를 기반으로, far plane의 너비를 계산한다.
  float CalcWidthBasedOnFoV(float fovWidthDeg, float camToFarPlaneDistance) {
    float tan_theta = tan(fovWidthDeg / 2 / 180.f * 3.141592);
    return atan(tan_theta) * camToFarPlaneDistance * 2; // meter
  }

  //! FoV를 기반으로, far plane의 높이를 계산한다.
  float CalcHeightBasedOnFoV(float fovHeightDeg, float camToFarPlaneDistance) {
    float tan_theta = tan(fovHeightDeg / 2.0f / 180.f * 3.141592f);
    return atan(tan_theta) * camToFarPlaneDistance * 2; // meter
  }

  //! FoV를 기반으로, far plane의 높이를 계산한다.
  float CalcLengthBasedOnFoV(float fovDeg, float camToFarPlaneDistance) {
    //float tan_theta = tan((fovDeg / 2.0f) / 180.f * 3.141592f);
    return tan((fovDeg / 2.0f) / 180.f * 3.141592f) * camToFarPlaneDistance *
           2.0f; // meter
  } 

public:
  uint64_t timeSeed;
};
} // namespace Toybox

#endif