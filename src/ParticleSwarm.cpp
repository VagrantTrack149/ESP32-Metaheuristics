#include "ParticleSwarm.h"
#include "Rng.h"

namespace bio {

ParticleSwarm::ParticleSwarm(const Bounds& bounds, ObjectiveFunction fn, int tamPoblacion,
                              float c1, float c2, float wMax, float wMin)
    : Optimizer(bounds, fn, tamPoblacion), _c1(c1), _c2(c2), _wMax(wMax), _wMin(wMin) {}

void ParticleSwarm::iniciar() {
    size_t dim = _bounds.dim();
    _enjambre.resize(_tamPoblacion);

    for (int i = 0; i < _tamPoblacion; ++i) {
        Particula& p = _enjambre[i];
        p.actual = agenteAleatorio();
        evaluar(p.actual);

        p.velocidad.resize(dim);
        for (size_t j = 0; j < dim; ++j) {
            float rango = _bounds.upper[j] - _bounds.lower[j];
            p.velocidad[j] = RNG::uniform(-rango, rango) * 0.1f;
        }

        p.mejorPersonal = p.actual;
        actualizarMejor(p.actual);
    }
}

void ParticleSwarm::paso(int iteracionActual, int GenTotales) {
    size_t dim = _bounds.dim();

    float w = _wMax - (_wMax - _wMin) * (static_cast<float>(iteracionActual) / GenTotales);

    for (int i = 0; i < _tamPoblacion; ++i) {
        Particula& p = _enjambre[i];

        for (size_t j = 0; j < dim; ++j) {
            float r1 = RNG::uniform(0.0f, 1.0f);
            float r2 = RNG::uniform(0.0f, 1.0f);

            float compCognitivo = _c1 * r1 * (p.mejorPersonal.posicion[j] - p.actual.posicion[j]);
            float compSocial = _c2 * r2 * (_mejor.posicion[j] - p.actual.posicion[j]);

            p.velocidad[j] = w * p.velocidad[j] + compCognitivo + compSocial;
            p.actual.posicion[j] += p.velocidad[j];
        }
        clampVector(p.actual.posicion);
        evaluar(p.actual);

        if (p.actual.fitness < p.mejorPersonal.fitness) {
            p.mejorPersonal = p.actual;
        }
        actualizarMejor(p.actual);
    }
}

} 