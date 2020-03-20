#ifndef CAMERA_H_
#define CAMERA_H_
#include <cmath>

#include "random.h"
#include "ray.h"

Eigen::Vector3f RandomInUnitDisk() {
  Eigen::Vector3f p;
  do {
    p = 2.f * Eigen::Vector3f(Random<double>(), Random<double>(), 0.f) -
        Eigen::Vector3f(1.f, 1.f, 0.f);
  } while (p.dot(p) >= 1.f);
  return p;
}

class Camera {
 public:
  Camera(Eigen::Vector3f look_from,
         Eigen::Vector3f look_at,
         Eigen::Vector3f up,
         float vfov,
         float aspect,
         float aperture,
         float focus_distance) {
    lens_radius_ = aperture / 2.f;
    float theta = vfov * M_PI / 180.f;
    float half_height = std::tan(theta / 2.f);
    float half_width = aspect * half_height;
    origin_ = look_from;
    w_ = (look_from - look_at).normalized();
    u_ = up.cross(w_).normalized();
    v_ = w_.cross(u_);
    lower_left_corner_ = origin_ - half_width * focus_distance * u_ -
                         half_height * focus_distance * v_ -
                         focus_distance * w_;
    horizontal_ = 2 * half_width * focus_distance * u_;
    vertical_ = 2 * half_height * focus_distance * v_;
  }

  Ray getRay(float s, float t) const {
    auto rd = lens_radius_ * RandomInUnitSphere();
    auto offset = u_ * rd.x() + v_ * rd.y();
    return Ray(origin_ + offset, lower_left_corner_ + s * horizontal_ +
                                     t * vertical_ - origin_ - offset);
  }

 private:
  Eigen::Vector3f lower_left_corner_;
  Eigen::Vector3f horizontal_;
  Eigen::Vector3f vertical_;
  Eigen::Vector3f origin_;
  Eigen::Vector3f u_, v_, w_;
  float lens_radius_;
};

#endif