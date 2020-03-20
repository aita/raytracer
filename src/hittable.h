#ifndef HITTABLE_H_
#define HITTABLE_H_
#include <functional>
#include <optional>

#include "ray.h"

class Material;

struct HitRecord {
  float t;
  Eigen::Vector3f p;
  Eigen::Vector3f normal;
  std::optional<std::reference_wrapper<Material>> material;
};

class Hittable {
 public:
  virtual bool hit(const Ray& r,
                   float min,
                   float max,
                   HitRecord& rec) const = 0;
};

#endif