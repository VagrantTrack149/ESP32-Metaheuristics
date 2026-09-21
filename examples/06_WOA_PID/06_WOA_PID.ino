#include <ESP32-Metaheuristics.h>

using namespace bio;

const int TAM_POBLACION = 40;
const int Gen = 100;

float simularPID(const std::vector<float>& k) {
    const float dt = 0.01f;
    const float setpoint = 100.0f;
    float y = 0.0f, integral = 0.0f, prevE = 0.0f;
    float IAE = 0.0f;

    for (int i = 0; i < 800; ++i) {
        float e = setpoint - y;
        integral += e * dt;
        float deriv = (e - prevE) / dt;
        float u = k[0] * e + k[1] * integral + k[2] * deriv;

        if (u > 200.0f) u = 200.0f;
        if (u < -200.0f) u = -200.0f;
        y += (u - 0.1f * y) * dt;

        IAE += fabsf(e) * dt;
        prevE = e;
    }
    return IAE;
}

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    RNG::seed();

    Bounds limites;
    limites.lower = {0.0f, 0.0f, 0.0f};
    limites.upper = {25.0f, 25.0f, 25.0f};

    WhaleOptimization woa(limites, simularPID, TAM_POBLACION);

    Serial.println("Auto-tuning PID con WOA...");
    woa.ejecutar(Gen, [](int it, int total, const Agente& mejor) {
        if ((it % 10 == 0) || (it == total - 1)) {
            Serial.printf("Iter %d/%d | IAE: %.4f\n", it + 1, total, mejor.fitness);
        }
    });

    const auto& g = woa.mejor().posicion;
    Serial.printf("\nKp=%.4f  Ki=%.4f  Kd=%.4f\n", g[0], g[1], g[2]);
    Serial.printf("IAE final: %.4f\n", woa.mejor().fitness);
}

void loop() { delay(10000); }