#include "Optimizerbase.h"
#include "Rng.h"

namespace bio {

Agente Optimizer::agenteAleatorio() const {
    Agente a;
    a.posicion.resize(_bounds.dim());
    for (size_t j = 0; j < _bounds.dim(); ++j) {
        a.posicion[j] = RNG::uniform(_bounds.lower[j], _bounds.upper[j]);
    }
    a.fitness = FLT_MAX;
    return a;
}

} 