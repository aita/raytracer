#ifndef _HITTABLE_LIST_H_
#define _HITTABLE_LIST_H_

#include <vector>
#include <memory>
#include "hittable.h"

class HittableList : public Hittable
{
    std::vector<std::shared_ptr<Hittable>> v_;

public:
    HittableList() {}
    HittableList(std::initializer_list<std::shared_ptr<Hittable>> l) : v_(l) {}

    virtual bool hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const;
};

bool HittableList::hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const
{

    HitRecord tmp;
    bool hitAnything = false;
    double closestSoFar = tmax;
    for (auto it = v_.begin(); it != v_.end(); ++it)
    {
        if ((*it)->hit(r, tmin, closestSoFar, tmp))
        {
            hitAnything = true;
            closestSoFar = tmp.t;
            rec = tmp;
        }
    }
    return hitAnything;
}

#endif