#pragma once

#include "Optimizerbase.h"

namespace bio {

class GeneticAlgorithm : public Optimizer {
public:
    GeneticAlgorithm(const Bounds& bounds, ObjectiveFunction fn, int tamPoblacion,
                      float tasaElitismo = 0.2f, float fraccionSeleccion = 0.4f,
                      float tasaMutacion = 0.2f, float sigmaMutacion = 0.05f);

    void iniciar() override;
    void paso(int iteracionActual, int GenTotales) override;

    const std::vector<Agente>& poblacion() const { return _poblacion; }

private:
    std::vector<Agente> _poblacion;
    float _tasaElitismo;
    float _fraccionSeleccion;
    float _tasaMutacion;
    float _sigmaMutacion;

    Agente cruzar(const Agente& p1, const Agente& p2) const;
};

} 