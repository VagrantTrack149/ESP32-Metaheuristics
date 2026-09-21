#include <ESP32-Metaheuristics.h>
#include <algorithm>
#include <math.h>

using namespace bio;

const int CIUDADES = 22;
const int TAM_POBLACION = 80;
const int Gen = 500;

struct Punto {
    float x;
    float y;
};

const Punto puntos[CIUDADES] = {
    {57, 28}, {54, -65}, {46, 79}, {8, 111}, {-36, 52}, {-22, -76},
    {34, 129}, {74, 6}, {-6, -41}, {21, 45}, {37, 155}, {-38, 35},
    {-5, -24}, {70, -74}, {59, -26}, {114, -56}, {83, -41}, {-40, -28},
    {21, -12}, {0, 71}, {50, 140}, {-20, 70}
};

void construirRuta(const std::vector<float>& claves, int ruta[CIUDADES]) {
    std::vector<int> orden(CIUDADES);
    for (int i = 0; i < CIUDADES; ++i) orden[i] = i;

    std::sort(orden.begin(), orden.end(), [&claves](int a, int b) {
        return claves[a] < claves[b];
    });

    for (int i = 0; i < CIUDADES; ++i) ruta[i] = orden[i];
}

float distancia(int a, int b) {
    const float dx = puntos[a].x - puntos[b].x;
    const float dy = puntos[a].y - puntos[b].y;
    return sqrtf(dx * dx + dy * dy);
}

float longitudRuta(const std::vector<float>& claves) {
    int ruta[CIUDADES];
    construirRuta(claves, ruta);

    float longitud = 0.0f;
    for (int i = 0; i < CIUDADES; ++i) {
        longitud += distancia(ruta[i], ruta[(i + 1) % CIUDADES]);
    }
    return longitud;
}

void imprimirRuta(const std::vector<float>& claves) {
    int ruta[CIUDADES];
    construirRuta(claves, ruta);

    Serial.print("Ruta: ");
    for (int i = 0; i < CIUDADES; ++i) {
        Serial.printf("%d -> ", ruta[i] + 1);
    }
    Serial.printf("%d\n", ruta[0] + 1);
}

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    RNG::seed();

    Bounds limites = Bounds::uniforme(CIUDADES, 0.0f, 1.0f);
    GeneticAlgorithm ga(limites, longitudRuta, TAM_POBLACION);

    Serial.println("TSP con GA y random keys...");
    ga.ejecutar(Gen, [](int it, int total, const Agente& mejor) {
        if ((it % 50 == 0) || (it == total - 1)) {
            Serial.printf("Iter %d/%d | Distancia: %.2f\n", it + 1, total, mejor.fitness);
        }
    });

    Serial.printf("\nDistancia final: %.2f\n", ga.mejor().fitness);
    imprimirRuta(ga.mejor().posicion);
}

void loop() { delay(10000); }