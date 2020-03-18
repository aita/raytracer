#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"

class Camera
{
    Eigen::Vector3f lower_left_corner_;
    Eigen::Vector3f horizontal_;
    Eigen::Vector3f vertical_;
    Eigen::Vector3f origin_;

public:
    Camera(Eigen::Vector3f lowerLeftCorner,
           Eigen::Vector3f horizontal,
           Eigen::Vector3f vertical,
           Eigen::Vector3f origin)
        : lower_left_corner_(lowerLeftCorner),
          horizontal_(horizontal),
          vertical_(vertical),
          origin_(origin) {}

    Camera() : Camera(Eigen::Vector3f(-2.0f, -1.0f, -1.0f),
                      Eigen::Vector3f(4.0f, 0.0f, 0.0f),
                      Eigen::Vector3f(0.0f, 2.0f, 0.0f),
                      Eigen::Vector3f(0.0f, 0.0f, 0.0f)) {}

    Ray getRay(float u, float v) const
    {
        return Ray(origin_, lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_);
    }
};

#endif