#pragma once

#include "Optimizerbase.h"

namespace bio {

class GreyWolfOptimizer : public Optimizer {
public:
    GreyWolfOptimizer(const Bounds& bounds, ObjectiveFunction fn, int tamPoblacion);

    void iniciar() override;
    void paso(int iteracionActual, int GenTotales) override;

    const std::vector<Agente>& manada() const { return _manada; }

private:
    std::vector<Agente> _manada;
    Agente _alfa, _beta, _delta;

    void actualizarLideres();
};

} 