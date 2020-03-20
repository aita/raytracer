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

#endif