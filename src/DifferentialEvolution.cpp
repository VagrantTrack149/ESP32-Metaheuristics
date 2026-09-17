#include "DifferentialEvolution.h"
#include "Rng.h"

namespace bio {

DifferentialEvolution::DifferentialEvolution(const Bounds& bounds, ObjectiveFunction fn,
                                              int tamPoblacion, float F, float CR)
    : Optimizer(bounds, fn, tamPoblacion), _F(F), _CR(CR) {}

void DifferentialEvolution::iniciar() {
    _poblacion.resize(_tamPoblacion);
    for (int i = 0; i < _tamPoblacion; ++i) {
        _poblacion[i] = agenteAleatorio();
        evaluar(_poblacion[i]);
        actualizarMejor(_poblacion[i]);
    }
}

void DifferentialEvolution::paso(int /*iteracionActual*/, int /*GenTotales*/) {
    size_t dim = _bounds.dim();

    for (int i = 0; i < _tamPoblacion; ++i) {
        int r1 = RNG::indiceDistinto(_tamPoblacion, i);
        int r2 = RNG::indiceDistinto(_tamPoblacion, i);
        while (r2 == r1) r2 = RNG::indiceDistinto(_tamPoblacion, i);
        int r3 = RNG::indiceDistinto(_tamPoblacion, i);
        while (r3 == r1 || r3 == r2) r3 = RNG::indiceDistinto(_tamPoblacion, i);

        Agente prueba;
        prueba.posicion.resize(dim);

        size_t jRand = RNG::uniformInt(0, dim);

        for (size_t j = 0; j < dim; ++j) {
            if (RNG::uniform(0.0f, 1.0f) < _CR || j == jRand) {
                float mutado = _poblacion[r1].posicion[j] +
                                _F * (_poblacion[r2].posicion[j] - _poblacion[r3].posicion[j]);
                prueba.posicion[j] = mutado;
            } else {
                prueba.posicion[j] = _poblacion[i].posicion[j];
            }
        }
        clampVector(prueba.posicion);
        evaluar(prueba);

        if (prueba.fitness < _poblacion[i].fitness) {
            _poblacion[i] = prueba;
        }
        actualizarMejor(_poblacion[i]);
    }
}

} 