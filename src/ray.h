#ifndef RAY_H_
#define RAY_H_
#include <Eigen/Dense>

class Ray {
 public:
  Ray() {}
  Ray(const Eigen::Vector3f& origin, const Eigen::Vector3f& direction)
      : origin_(origin), direction_(direction) {}
  Eigen::Vector3f origin() const { return origin_; }
  Eigen::Vector3f direction() const { return direction_; }
  Eigen::Vector3f pointAtParameter(float t) const {
    return origin_ + t * direction_;
  }

 private:
  Eigen::Vector3f origin_;
  Eigen::Vector3f direction_;
};

Eigen::Vector3f reflect(const Eigen::Vector3f& v, const Eigen::Vector3f& n) {
  return v - 2.f * v.dot(n) * n;
}

bool refract(const Eigen::Vector3f& v,
             const Eigen::Vector3f& n,
             float ni_over_nt,
             Eigen::Vector3f& refracted) {
  Eigen::Vector3f uv = v.normalized();
  float dt = uv.dot(n);
  float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.f - dt * dt);
  if (discriminant > 0) {
    refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
    return true;
  }
  return false;
}

#endif