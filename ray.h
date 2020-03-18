#ifndef _RAY_H_
#define _RAY_H_
#include <Eigen/Dense>

class Ray
{
    Eigen::Vector3f origin_;
    Eigen::Vector3f direction_;

public:
    Ray() {}
    Ray(const Eigen::Vector3f &a, const Eigen::Vector3f &b) : origin_(a), direction_(b) {}
    Eigen::Vector3f origin() const { return origin_; }
    Eigen::Vector3f direction() const { return direction_; }
    Eigen::Vector3f pointAtParameter(float t) const { return origin_ + t * direction_; }
};

#endif