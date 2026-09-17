#include "ArtificialBeeColony.h"
#include "Rng.h"
#include <algorithm>

namespace bio {

ArtificialBeeColony::ArtificialBeeColony(const Bounds& bounds, ObjectiveFunction fn,
                                          int tamPoblacion, int limiteAbandono)
    : Optimizer(bounds, fn, tamPoblacion), _limiteAbandono(limiteAbandono) {}

void ArtificialBeeColony::iniciar() {
    _fuentes.resize(_tamPoblacion);
    _contadorIntentos.assign(_tamPoblacion, 0);
    for (int i = 0; i < _tamPoblacion; ++i) {
        _fuentes[i] = agenteAleatorio();
        evaluar(_fuentes[i]);
        actualizarMejor(_fuentes[i]);
    }
}

Agente ArtificialBeeColony::generarVecino(const Agente& base, const Agente& vecino, size_t dim) const {
    Agente nueva = base;
    size_t j = RNG::uniformInt(0, dim);
    float phi = RNG::uniform(-1.0f, 1.0f);
    nueva.posicion[j] = base.posicion[j] + phi * (base.posicion[j] - vecino.posicion[j]);
    clampVector(nueva.posicion);
    return nueva;
}

void ArtificialBeeColony::faseEmpleadas() {
    size_t dim = _bounds.dim();
    for (int i = 0; i < _tamPoblacion; ++i) {
        int k = RNG::indiceDistinto(_tamPoblacion, i);
        Agente candidata = generarVecino(_fuentes[i], _fuentes[k], dim);
        evaluar(candidata);

        if (candidata.fitness < _fuentes[i].fitness) {
            _fuentes[i] = candidata;
            _contadorIntentos[i] = 0;
        } else {
            _contadorIntentos[i]++;
        }
    }
}

void ArtificialBeeColony::faseObservadoras() {
    size_t dim = _bounds.dim();

    std::vector<float> aptitud(_tamPoblacion);
    float sumaAptitud = 0.0f;
    for (int i = 0; i < _tamPoblacion; ++i) {
        aptitud[i] = 1.0f / (1.0f + std::max(0.0f, _fuentes[i].fitness));
        sumaAptitud += aptitud[i];
    }

    int i = 0, evaluadas = 0;
    while (evaluadas < _tamPoblacion) {
        float r = RNG::uniform(0.0f, sumaAptitud);
        float acumulado = 0.0f;
        int elegido = 0;
        for (int j = 0; j < _tamPoblacion; ++j) {
            acumulado += aptitud[j];
            if (acumulado >= r) { elegido = j; break; }
        }

        int k = RNG::indiceDistinto(_tamPoblacion, elegido);
        Agente candidata = generarVecino(_fuentes[elegido], _fuentes[k], dim);
        evaluar(candidata);

        if (candidata.fitness < _fuentes[elegido].fitness) {
            _fuentes[elegido] = candidata;
            _contadorIntentos[elegido] = 0;
        } else {
            _contadorIntentos[elegido]++;
        }
        evaluadas++;
        i++;
        (void)i;
    }
}

void ArtificialBeeColony::faseExploradoras() {
    for (int i = 0; i < _tamPoblacion; ++i) {
        if (_contadorIntentos[i] > _limiteAbandono) {
            _fuentes[i] = agenteAleatorio();
            evaluar(_fuentes[i]);
            _contadorIntentos[i] = 0;
        }
    }
}

void ArtificialBeeColony::paso(int /*iteracionActual*/, int /*GenTotales*/) {
    faseEmpleadas();
    faseObservadoras();
    faseExploradoras();

    for (auto& f : _fuentes) {
        actualizarMejor(f);
    }
}

} // namespace bio