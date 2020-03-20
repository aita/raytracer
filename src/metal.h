#ifndef METAL_H_
#define METAL_H_
#include <Eigen/Dense>
#include <algorithm>

#include "material.h"
#include "random.h"

class Metal : public Material {
 public:
  Metal(const Eigen::Vector3f& albedo, float fuzziness)
      : albedo_(albedo), fuzziness_(std::clamp(fuzziness, 0.f, 1.f)) {}

  virtual bool scatter(const Ray& r_in,
                       const HitRecord& rec,
                       Eigen::Vector3f& attenuation,
                       Ray& scattered) const {
    auto reflected = reflect(r_in.direction().normalized(), rec.normal);
    scattered = Ray(rec.p, reflected + fuzziness_ * RandomInUnitSphere());
    attenuation = albedo_;
    return scattered.direction().dot(rec.normal) > 0.f;
  }

 private:
  Eigen::Vector3f albedo_;
  float fuzziness_;
};

#endif