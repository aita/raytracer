#ifndef RANDOM_H_
#define RANDOM_H_

#include <functional>
#include <random>

template <typename T>
inline T Random() {
  static std::uniform_real_distribution<T> distribution(0.0, 1.0);
  static std::mt19937 generator;
  static std::function<T()> randGenerator = std::bind(distribution, generator);
  return randGenerator();
}

#endif