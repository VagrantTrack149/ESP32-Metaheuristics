#pragma once

#include "Optimizerbase.h"

namespace bio {

class WhaleOptimization : public Optimizer {
public:
    WhaleOptimization(const Bounds& bounds, ObjectiveFunction fn, int tamPoblacion, float b = 1.0f);

    void iniciar() override;
    void paso(int iteracionActual, int GenTotales) override;

    const std::vector<Agente>& poblacion() const { return _poblacion; }

private:
    std::vector<Agente> _poblacion;
    float _b;
};

} 