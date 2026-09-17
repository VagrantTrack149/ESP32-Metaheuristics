#include "GeneticAlgorithm.h"
#include "Rng.h"
#include <algorithm>

namespace bio {

GeneticAlgorithm::GeneticAlgorithm(const Bounds& bounds, ObjectiveFunction fn, int tamPoblacion,
                                    float tasaElitismo, float fraccionSeleccion,
                                    float tasaMutacion, float sigmaMutacion)
    : Optimizer(bounds, fn, tamPoblacion),
      _tasaElitismo(tasaElitismo),
      _fraccionSeleccion(fraccionSeleccion),
      _tasaMutacion(tasaMutacion),
      _sigmaMutacion(sigmaMutacion) {}

void GeneticAlgorithm::iniciar() {
    _poblacion.resize(_tamPoblacion);
    for (int i = 0; i < _tamPoblacion; ++i) {
        _poblacion[i] = agenteAleatorio();
        evaluar(_poblacion[i]);
        actualizarMejor(_poblacion[i]);
    }
}

Agente GeneticAlgorithm::cruzar(const Agente& p1, const Agente& p2) const {
    Agente hijo;
    size_t dim = _bounds.dim();
    hijo.posicion.resize(dim);
    for (size_t j = 0; j < dim; ++j) {
        hijo.posicion[j] = (p1.posicion[j] + p2.posicion[j]) / 2.0f;

        if (RNG::uniform(0.0f, 1.0f) < _tasaMutacion) {
            float rango = _bounds.upper[j] - _bounds.lower[j];
            float delta = RNG::gauss(0.0f, rango * _sigmaMutacion);
            hijo.posicion[j] += delta;
        }
    }
    clampVector(hijo.posicion);
    return hijo;
}

void GeneticAlgorithm::paso(int /*iteracionActual*/, int /*GenTotales*/) {
    std::sort(_poblacion.begin(), _poblacion.end(), [](const Agente& a, const Agente& b) {
        return a.fitness < b.fitness;
    });

    int nElite = static_cast<int>(_tamPoblacion * _tasaElitismo);
    int limiteSeleccion = std::max(2, static_cast<int>(_tamPoblacion * _fraccionSeleccion));

    std::vector<Agente> nuevaPoblacion;
    nuevaPoblacion.reserve(_tamPoblacion);

    for (int i = 0; i < nElite && i < _tamPoblacion; ++i) {
        nuevaPoblacion.push_back(_poblacion[i]);
    }

    while (static_cast<int>(nuevaPoblacion.size()) < _tamPoblacion) {
        int i1 = RNG::uniformInt(0, limiteSeleccion);
        int i2 = RNG::uniformInt(0, limiteSeleccion);
        Agente hijo = cruzar(_poblacion[i1], _poblacion[i2]);
        evaluar(hijo);
        nuevaPoblacion.push_back(hijo);
    }

    _poblacion = std::move(nuevaPoblacion);

    for (auto& a : _poblacion) {
        actualizarMejor(a);
    }
}

} 