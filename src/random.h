#ifndef RANDOM_H_
#define RANDOM_H_

#include <functional>
#include <random>

template <typename T>
inline T Random() {
  static thread_local std::uniform_real_distribution<T> distribution(0.0, 1.0);
  static thread_local std::mt19937 generator;
  static thread_local std::function<T()> randGenerator =
      std::bind(distribution, generator);
  return randGenerator();
}

Eigen::Vector3f RandomInUnitSphere() {
  Eigen::Vector3f p;
  do {
    p = 2.f * Eigen::Vector3f(Random<double>(), Random<double>(),
                              Random<double>()) -
        Eigen::Vector3f(1.f, 1.f, 1.f);
  } while (p.squaredNorm() >= 1.f);
  return p;
}

#endif