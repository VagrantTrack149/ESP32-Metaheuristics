#include "Rng.h"
#include <cmath>

#if defined(ESP32)
#include "esp_system.h"
#endif

namespace bio {

void RNG::seed(uint32_t s) {
        #if defined(ESP32)
            (void)s;
        #else
            randomSeed(s);
        #endif
}

float RNG::uniform(float min, float max) {
        #if defined(ESP32)
            uint32_t r = esp_random();
            float t = static_cast<float>(r) / 4294967295.0f;
            return min + t*(max-min);
        #else
            return min + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (max - min);
        #endif
}

int RNG::uniformInt(int min, int max) {
    if (max <= min) return min;
    return min + static_cast<int>(uniform(0.0f, 1.0f) * (max - min));
}

float RNG::gauss(float mean, float stddev) {
    float u1 = uniform(1e-6f, 1.0f);
    float u2 = uniform(1e-6f, 1.0f);
    float z0 = sqrtf(-2.0f * logf(u1)) * cosf(2.0f * PI * u2);
    return mean + z0 * stddev;
}

int RNG::indiceDistinto(int min, int exclude) {
    if (min <= 1) return exclude;
    int idx = exclude;
    while (idx == exclude) {
        idx = uniformInt(0, min);
    }
    return idx;
}

}