#pragma once

#include "OptimizerTypes.h"
#include <cfloat>

namespace bio {

class Optimizer {
public:
    Optimizer(const Bounds& bounds, ObjectiveFunction fn, int tamPoblacion)
        : _bounds(bounds), _fn(fn), _tamPoblacion(tamPoblacion) {
        _mejor.fitness = FLT_MAX;
    }

    virtual ~Optimizer() {}

    virtual void iniciar() = 0;
    virtual void paso(int iteracionActual, int GenTotales) = 0;

    using CallbackProgreso = std::function<void(int iter, int totalIter, const Agente& mejor)>;

    void ejecutar(int Gen, CallbackProgreso callback = nullptr) {
        iniciar();
        for (int it = 0; it < Gen; ++it) {
            paso(it, Gen);
            if (callback) callback(it, Gen, _mejor);
        }
    }

    const Agente& mejor() const { return _mejor; }
    const Bounds& bounds() const { return _bounds; }
    int tamPoblacion() const { return _tamPoblacion; }

protected:
    float clamp(float v, float lo, float hi) const {
        return v < lo ? lo : (v > hi ? hi : v);
    }

    void clampVector(std::vector<float>& v) const {
        for (size_t i = 0; i < v.size() && i < _bounds.lower.size(); ++i) {
            v[i] = clamp(v[i], _bounds.lower[i], _bounds.upper[i]);
        }
    }

    void evaluar(Agente& a) const {
        a.fitness = _fn(a.posicion);
    }

    void actualizarMejor(const Agente& candidato) {
        if (candidato.fitness < _mejor.fitness) {
            _mejor = candidato;
        }
    }

    Agente agenteAleatorio() const;

    Bounds _bounds;
    ObjectiveFunction _fn;
    int _tamPoblacion;
    Agente _mejor;
};

} 