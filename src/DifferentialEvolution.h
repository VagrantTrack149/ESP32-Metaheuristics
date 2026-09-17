#pragma once

#include "Optimizerbase.h"

namespace bio {

class DifferentialEvolution : public Optimizer {
public:
    DifferentialEvolution(const Bounds& bounds, ObjectiveFunction fn, int tamPoblacion,
                           float F = 0.5f, float CR = 0.9f);

    void iniciar() override;
    void paso(int iteracionActual, int GenTotales) override;

    const std::vector<Agente>& poblacion() const { return _poblacion; }

private:
    std::vector<Agente> _poblacion;
    float _F;
    float _CR;
};

} 