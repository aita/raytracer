#ifndef MATERIAL_H_
#define MATERIAL_H_
#include <Eigen/Dense>

#include "hittable.h"

class Material {
 public:
  virtual bool scatter(const Ray& r_in,
                       const HitRecord& rec,
                       Eigen::Vector3f& attenuation,
                       Ray& scatterd) const = 0;
};

#endif