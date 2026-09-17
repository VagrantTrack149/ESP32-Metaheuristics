#include "GreyWolfOptimizer.h"
#include "Rng.h"
#include <cfloat>
#include <cmath>

namespace bio {

GreyWolfOptimizer::GreyWolfOptimizer(const Bounds& bounds, ObjectiveFunction fn, int tamPoblacion)
    : Optimizer(bounds, fn, tamPoblacion) {
    _alfa.fitness = FLT_MAX;
    _beta.fitness = FLT_MAX;
    _delta.fitness = FLT_MAX;
}

void GreyWolfOptimizer::iniciar() {
    _manada.resize(_tamPoblacion);
    for (int i = 0; i < _tamPoblacion; ++i) {
        _manada[i] = agenteAleatorio();
        evaluar(_manada[i]);
        actualizarMejor(_manada[i]);
    }
    actualizarLideres();
}

void GreyWolfOptimizer::actualizarLideres() {
    for (const auto& lobo : _manada) {
        if (lobo.fitness < _alfa.fitness) {
            _delta = _beta;
            _beta = _alfa;
            _alfa = lobo;
        } else if (lobo.fitness < _beta.fitness) {
            _delta = _beta;
            _beta = lobo;
        } else if (lobo.fitness < _delta.fitness) {
            _delta = lobo;
        }
    }
}

void GreyWolfOptimizer::paso(int iteracionActual, int GenTotales) {
    size_t dim = _bounds.dim();

    float a = 2.0f - 2.0f * (static_cast<float>(iteracionActual) / GenTotales);

    for (int i = 0; i < _tamPoblacion; ++i) {
        for (size_t j = 0; j < dim; ++j) {
            float r1 = RNG::uniform(0.0f, 1.0f), r2 = RNG::uniform(0.0f, 1.0f);
            float A1 = 2.0f * a * r1 - a;
            float C1 = 2.0f * r2;
            float dAlfa = fabsf(C1 * _alfa.posicion[j] - _manada[i].posicion[j]);
            float X1 = _alfa.posicion[j] - A1 * dAlfa;

            r1 = RNG::uniform(0.0f, 1.0f); r2 = RNG::uniform(0.0f, 1.0f);
            float A2 = 2.0f * a * r1 - a;
            float C2 = 2.0f * r2;
            float dBeta = fabsf(C2 * _beta.posicion[j] - _manada[i].posicion[j]);
            float X2 = _beta.posicion[j] - A2 * dBeta;

            r1 = RNG::uniform(0.0f, 1.0f); r2 = RNG::uniform(0.0f, 1.0f);
            float A3 = 2.0f * a * r1 - a;
            float C3 = 2.0f * r2;
            float dDelta = fabsf(C3 * _delta.posicion[j] - _manada[i].posicion[j]);
            float X3 = _delta.posicion[j] - A3 * dDelta;

            _manada[i].posicion[j] = (X1 + X2 + X3) / 3.0f;
        }
        clampVector(_manada[i].posicion);
        evaluar(_manada[i]);
        actualizarMejor(_manada[i]);
    }

    actualizarLideres();
}

} 