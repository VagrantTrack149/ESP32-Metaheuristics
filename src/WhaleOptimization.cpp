#include "WhaleOptimization.h"
#include "Rng.h"
#include <cmath>

namespace bio {

WhaleOptimization::WhaleOptimization(const Bounds& bounds, ObjectiveFunction fn,
                                      int tamPoblacion, float b)
    : Optimizer(bounds, fn, tamPoblacion), _b(b) {}

void WhaleOptimization::iniciar() {
    _poblacion.resize(_tamPoblacion);
    for (int i = 0; i < _tamPoblacion; ++i) {
        _poblacion[i] = agenteAleatorio();
        evaluar(_poblacion[i]);
        actualizarMejor(_poblacion[i]);
    }
}

void WhaleOptimization::paso(int iteracionActual, int GenTotales) {
    size_t dim = _bounds.dim();
    float a = 2.0f - 2.0f * (static_cast<float>(iteracionActual) / GenTotales);

    for (int i = 0; i < _tamPoblacion; ++i) {
        float r1 = RNG::uniform(0.0f, 1.0f);
        float r2 = RNG::uniform(0.0f, 1.0f);
        float A = 2.0f * a * r1 - a;
        float C = 2.0f * r2;
        float p = RNG::uniform(0.0f, 1.0f);

        for (size_t j = 0; j < dim; ++j) {
            float nuevo;
            if (p < 0.5f) {
                if (fabsf(A) < 1.0f) {
                    float d = fabsf(C * _mejor.posicion[j] - _poblacion[i].posicion[j]);
                    nuevo = _mejor.posicion[j] - A * d;
                } else {
                    int idxAleatorio = RNG::indiceDistinto(_tamPoblacion, i);
                    float d = fabsf(C * _poblacion[idxAleatorio].posicion[j] - _poblacion[i].posicion[j]);
                    nuevo = _poblacion[idxAleatorio].posicion[j] - A * d;
                }
            } else {
                float l = RNG::uniform(-1.0f, 1.0f);
                float dPrima = fabsf(_mejor.posicion[j] - _poblacion[i].posicion[j]);
                nuevo = dPrima * expf(_b * l) * cosf(2.0f * PI * l) + _mejor.posicion[j];
            }
            _poblacion[i].posicion[j] = nuevo;
        }
        clampVector(_poblacion[i].posicion);
        evaluar(_poblacion[i]);
        actualizarMejor(_poblacion[i]);
    }
}

} 