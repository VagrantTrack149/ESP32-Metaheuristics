#pragma once

#include "Optimizerbase.h"

namespace bio {

struct Particula {
    Agente actual;
    std::vector<float> velocidad;
    Agente mejorPersonal;
};

class ParticleSwarm : public Optimizer {
public:
    ParticleSwarm(const Bounds& bounds, ObjectiveFunction fn, int tamPoblacion,
                  float c1 = 1.5f, float c2 = 1.5f, float wMax = 0.9f, float wMin = 0.4f);

    void iniciar() override;
    void paso(int iteracionActual, int GenTotales) override;

    const std::vector<Particula>& enjambre() const { return _enjambre; }

private:
    std::vector<Particula> _enjambre;
    float _c1, _c2, _wMax, _wMin;
};

} 