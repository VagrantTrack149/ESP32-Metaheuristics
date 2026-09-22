# ESP32-Metaheuristics

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue.svg)](https://www.arduino.cc/reference/en/libraries/)
[![ESP32](https://img.shields.io/badge/Platform-ESP32-red.svg)](https://www.espressif.com/en/products/socs/esp32)

Librería de algoritmos de optimización metaheurística (genéticos, evolutivos y bioinspirados) para **ESP32**, diseñada para ser utilizada con el **Arduino IDE** y **PlatformIO**.

Incluye implementaciones ligeras de varios algoritmos clásicos, pensadas para ejecutarse en microcontroladores con recursos limitados.

> **Estado del proyecto:** En desarrollo activo (WIP). Algunas APIs pueden cambiar.

---

## 📋 Tabla de contenidos

- [Algoritmos soportados](#-algoritmos-soportados)
- [Instalación](#-instalación)
- [Uso básico](#-uso-básico)
- [Ejemplos incluidos](#-ejemplos-incluidos)
- [API principal](#-api-principal)
- [Rendimiento y limitaciones](#-rendimiento-y-limitaciones)
- [Contribuciones](#-contribuciones)
- [Licencia](#-licencia)

---

## Algoritmos soportados

| Algoritmo | Inspiración | Archivo principal |
|-----------|-------------|-------------------|
| **GA** – Genetic Algorithm | Evolución biológica (selección, cruce, mutación) | `GeneticAlgorithm.h/.cpp` |
| **DE** – Differential Evolution | Evolución diferencial | `DifferentialEvolution.h/.cpp` |
| **PSO** – Particle Swarm Optimization | Comportamiento de enjambres (aves, peces) | `ParticleSwarmOptimization.h/.cpp` |
| **ABC** – Artificial Bee Colony | Comportamiento de colonias de abejas | `ArtificialBeeColony.h/.cpp` |
| **GWO** – Grey Wolf Optimizer | Jerarquía y caza de lobos grises | `GreyWolfOptimizer.h/.cpp` |
| **WOA** – Whale Optimization Algorithm | Caza con red de burbujas de ballenas jorobadas | `WhaleOptimization.h/.cpp` |

Cada algoritmo está implementado en C++ con un enfoque en bajo consumo de memoria y velocidad de ejecución, aprovechando las capacidades del ESP32.

---

## Instalación

### Arduino IDE (Library Manager)

1. Abre el **Arduino IDE**.
2. Ve a **Herramientas → Administrar bibliotecas…**.
3. Busca **“ESP32-Metaheuristics”**.
4. Haz clic en **Instalar**.

### Instalación manual

1. Descarga el repositorio como ZIP.
2. En Arduino IDE: **Programa → Incluir biblioteca → Añadir biblioteca .ZIP**.
3. O copia la carpeta `ESP32-Metaheuristics` en tu carpeta de librerías de Arduino (`~/Documents/Arduino/libraries/`).

---

## 🚀 Uso básico

El flujo típico es:

1. Definir la función de **fitness** (objetivo a minimizar o maximizar).
2. Configurar los parámetros del algoritmo (tamaño de población, generaciones, límites).
3. Inicializar el algoritmo con esa configuración.
4. Ejecutar la optimización en `setup()` o `loop()`.
5. Obtener la mejor solución.

Ejemplo genérico (la sintaxis exacta puede variar según el algoritmo):

```cpp
#include <ESP32-Metaheuristics.h>

// Función de fitness: minimizar (x-3)^2 + (y+2)^2
float fitness(float* vars, int dim) {
  return pow(vars[0] - 3.0, 2) + pow(vars[1] + 2.0, 2);
}

GeneticAlgorithm ga;

void setup() {
  Serial.begin(115200);

  // Configuración
  ga.setPopulationSize(30);
  ga.setGenerations(100);
  ga.setDimension(2);
  ga.setBounds(-10.0, 10.0);   // límites inferior y superior
  ga.setFitnessFunction(fitness);

  // Ejecutar optimización
  ga.run();

  // Obtener mejor solución
  float* best = ga.getBestSolution();
  Serial.printf("Mejor solución: x=%.3f, y=%.3f, fitness=%.4f\n",
                best[0], best[1], ga.getBestFitness());
}

void loop() {
  // Vacío
}
```

>  **Nota:** Consulta los ejemplos incluidos para ver la API exacta de cada algoritmo, ya que puede haber ligeras diferencias en los nombres de métodos.

---

## Ejemplos incluidos

En la carpeta [`examples/`](https://github.com/VagrantTrack149/ESP32-Metaheuristics/tree/main/examples) encontrarás proyectos listos para cargar en tu ESP32:

| Ejemplo | Descripción |
|---------|-------------|
| `01_GA_Circle` | Optimización de una función circular con GA. |
| `01_GA_PID` | Sintonización de un controlador PID usando GA. |
| `01_TSP_GA` | Problema del viajante (TSP) con GA. |
| `02_DE_Circle` | Optimización de función circular con DE. |
| `02_DE_PID` | Sintonización de PID con DE. |
| `03_PSO_Circle` | Optimización con PSO. |
| `03_PSO_PID` | Sintonización de PID con PSO. |
| `04_ABC_Circle` | Optimización con ABC. |
| `04_ABC_PID` | Sintonización de PID con ABC. |

*(Se irán añadiendo más ejemplos para GWO y WOA.)*

Cada ejemplo incluye comentarios y puede compilarse directamente en el Arduino IDE o PlatformIO.

---

##  API principal

Aunque la API puede evolucionar, la estructura general es similar para todos los algoritmos:

| Clase | Métodos comunes |
|-------|----------------|
| `GeneticAlgorithm` | `setPopulationSize()`, `setGenerations()`, `setDimension()`, `setBounds()`, `setFitnessFunction()`, `run()`, `getBestSolution()`, `getBestFitness()` |
| `DifferentialEvolution` | ídem, con parámetros adicionales como `setF()`, `setCR()` |
| `ParticleSwarmOptimization` | ídem, con `setInertia()`, `setC1()`, `setC2()` |
| `ArtificialBeeColony` | ídem, con `setLimit()`, `setColonySize()` |
| `GreyWolfOptimizer` | ídem |
| `WhaleOptimization` | ídem |

Para detalles específicos, revisa los archivos de cabecera en [`src/`](https://github.com/VagrantTrack149/ESP32-Metaheuristics/tree/main/src).

---

## Rendimiento y limitaciones

- **Optimizado para ESP32:** Uso de memoria controlado, sin asignaciones dinámicas innecesarias.
- **Precisión:** Se utilizan `float` para equilibrar velocidad y exactitud.
- **Tamaño de población:** Se recomienda entre 20 y 50 individuos para problemas de 2–10 dimensiones.
- **Generaciones:** Ajustar según la complejidad; valores típicos: 50–200.
- **Tiempo de ejecución:** Depende del fitness y la dimensión; en ESP32 a 240 MHz suele ser de milisegundos a pocos segundos.

> **Consejo:** Para problemas de alta dimensionalidad, considera reducir la población o el número de generaciones para evitar bloqueos prolongados.

---

## Contribuciones

Las contribuciones son bienvenidas. Si deseas añadir un nuevo algoritmo, mejorar la documentación o corregir un error:

1. Haz un **fork** del repositorio.
2. Crea una rama con tu mejora (`git checkout -b feature/nuevo-algoritmo`).
3. Realiza tus cambios y haz commit.
4. Envía un **pull request**.

Por favor, mantén el estilo de código y añade ejemplos cuando sea posible.

---

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulta el archivo [LICENSE](https://github.com/VagrantTrack149/ESP32-Metaheuristics/blob/main/LICENSE) para más detalles.

---

## Referencias

- Documentación de Arduino para ESP32: [https://docs.espressif.com/projects/arduino-esp32/](https://docs.espressif.com/projects/arduino-esp32/)
- Algoritmos genéticos en microcontroladores: [TinyGA](https://github.com/per1234/TinyGA)
- Optimización por enjambre de partículas en ESP32: aplicaciones en sintonización de PID
- Grey Wolf Optimizer: inspirado en la jerarquía de lobos grises
- Whale Optimization Algorithm: basado en la caza con red de burbujas

---

**¿Preguntas o sugerencias?** Abre un *issue* en el repositorio.
