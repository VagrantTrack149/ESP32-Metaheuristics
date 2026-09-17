#pragma once

#include <Arduino.h>
#include <vector>

namespace bio {

class RNG {
public:
    static void seed(uint32_t s = 0);
    static float uniform(float min, float max);
    static int uniformInt(int min, int max);
    static float gauss(float mean, float stddev);
    static int indiceDistinto(int min, int exclude);
};

}