#include <ESP32-Metaheuristics.h>
#include <math.h>

using namespace bio;

const int PUNTOS = 10;
const int DIMENSIONES = PUNTOS * 3;
const int TAM_POBLACION = 40;
const int Gen = 200;

float errorCirculo(const std::vector<float>& puntos) {
    float error = 0.0f;
    const float pendienteX = 0.5f;
    const float pendienteY = 0.2f;

    for (int i = 0; i < PUNTOS; ++i) {
        float t = 2.0f * PI * i / PUNTOS;
        float xObjetivo = cosf(t);
        float yObjetivo = sinf(t);
        float zObjetivo = pendienteX * xObjetivo + pendienteY * yObjetivo;

        error += (puntos[i] - xObjetivo) * (puntos[i] - xObjetivo);
        error += (puntos[PUNTOS + i] - yObjetivo) * (puntos[PUNTOS + i] - yObjetivo);
        error += (puntos[2 * PUNTOS + i] - zObjetivo) * (puntos[2 * PUNTOS + i] - zObjetivo);
    }
    return error / (PUNTOS * 3);
}

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }

    RNG::seed();

    Bounds limites = Bounds::uniforme(DIMENSIONES, -1.2f, 1.2f);
    GreyWolfOptimizer gwo(limites, errorCirculo, TAM_POBLACION);

    Serial.println("Ajustando un circulo inclinado con GWO...");

    gwo.ejecutar(Gen, [](int it, int total, const Agente& mejor) {
        if ((it + 1) % 20 == 0 || it == 0) {
            Serial.printf("Iter %d/%d | Mejor fitness: %.6f\n", it + 1, total, mejor.fitness);
        }
    });

    Serial.printf("\nFitness final: %.6f\n", gwo.mejor().fitness);
}

void loop() {
    delay(10000);
}
