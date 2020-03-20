#ifndef HITTABLE_LIST_H_
#define HITTABLE_LIST_H_

#include <memory>
#include <vector>

#include "hittable.h"

class HittableList : public Hittable {
 public:
  HittableList(std::initializer_list<std::shared_ptr<Hittable>> l) : v_(l) {}

  virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;

 private:
  std::vector<std::shared_ptr<Hittable>> v_;
};

bool HittableList::hit(const Ray& r,
                       float tmin,
                       float tmax,
                       HitRecord& rec) const {
  HitRecord tmp;
  bool hitAnything = false;
  double closestSoFar = tmax;
  for (auto& it : v_) {
    if (it->hit(r, tmin, closestSoFar, tmp)) {
      hitAnything = true;
      closestSoFar = tmp.t;
      rec = tmp;
    }
  }
  return hitAnything;
}

#endif