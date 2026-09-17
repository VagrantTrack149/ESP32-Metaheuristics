#pragma once

#include "Optimizerbase.h"

namespace bio {

class ArtificialBeeColony : public Optimizer {
public:
    ArtificialBeeColony(const Bounds& bounds, ObjectiveFunction fn, int tamPoblacion,
                         int limiteAbandono = 20);

    void iniciar() override;
    void paso(int iteracionActual, int GenTotales) override;

    const std::vector<Agente>& fuentes() const { return _fuentes; }

private:
    std::vector<Agente> _fuentes;
    std::vector<int> _contadorIntentos;
    int _limiteAbandono;

    Agente generarVecino(const Agente& base, const Agente& vecino, size_t dim) const;
    void faseEmpleadas();
    void faseObservadoras();
    void faseExploradoras();
};

} 