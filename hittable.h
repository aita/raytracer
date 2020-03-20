#ifndef HITTABLE_H_
#define HITTABLE_H_

#include "ray.h"

struct HitRecord {
  float t;
  Eigen::Vector3f p;
  Eigen::Vector3f normal;
};

class Hittable {
 public:
  virtual bool hit(const Ray& r,
                   float min,
                   float max,
                   HitRecord& rec) const = 0;
};

#endif