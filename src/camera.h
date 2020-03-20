#ifndef CAMERA_H_
#define CAMERA_H_
#include <cmath>

#include "ray.h"

class Camera {
 public:
  Camera(Eigen::Vector3f look_from,
         Eigen::Vector3f look_at,
         Eigen::Vector3f up,
         float vfov,
         float aspect) {
    float theta = vfov * M_PI / 180.f;
    float half_height = std::tan(theta / 2);
    float half_width = aspect * half_height;
    origin_ = look_from;
    auto w = (look_from - look_at).normalized();
    auto u = up.cross(w).normalized();
    auto v = w.cross(u);
    lower_left_corner_ = origin_ - half_width * u - half_height * v - w;
    horizontal_ = 2 * half_width * u;
    vertical_ = 2 * half_height * v;
  }

  Ray getRay(float u, float v) const {
    return Ray(origin_,
               lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_);
  }

 private:
  Eigen::Vector3f lower_left_corner_;
  Eigen::Vector3f horizontal_;
  Eigen::Vector3f vertical_;
  Eigen::Vector3f origin_;
};

#endif