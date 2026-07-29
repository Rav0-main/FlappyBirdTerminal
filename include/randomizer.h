#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <random>

template <typename IntType = int>
class Randomizer
{
   private:
    std::uniform_int_distribution<IntType> distrib_;

   public:
    inline static std::mt19937 gen{std::random_device{}()};

    Randomizer(IntType start, IntType end) : distrib_(start, end) {}

    IntType operator()() { return distrib_(gen); }
};

#endif  // !RANDOMIZER_H
