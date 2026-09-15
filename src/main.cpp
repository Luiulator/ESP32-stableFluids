#include "stable_fluids/StableFluids.hpp"
#include "stable_fluids/forces.hpp"

#if defined(ARDUINO)
#include <Arduino.h>

/* Aquí el código de la ESP32 */

constexpr size_t NX = 32;
constexpr size_t NY = 48;

/*
Instancio la malla. tenemos que instanciarla como variable global para que se
vaya a la SRAM de la ESp32 De lo contrario, intentaría alocar los bytes
correspondientes a la stack de 8 KB, o sea, stack overflow
*/

stable_fluids::FluidGrid2D<NX, NY> grid;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nStable Fluids en una ESP32");
  Serial.printf("Grid de %u x %u (%u celdas)\n", (unsigned)NX, (unsigned)NY,
                (unsigned)(NX * NY));
  Serial.printf("Tamaño de malla: %.2f KB\n", sizeof(grid) / 1024.0f);

  grid.params.dt = 0.016f;
  grid.params.visc = 0.001f;
  grid.params.diff = 0.001f;

  Serial.println("Setup completada");
}

uint32_t step_count = 0;
uint32_t total_time_us = 0;

void loop() {

  for (size_t i = NX / 2 - 2; i <= NX / 2 + 2; i++) {
    grid.density[i][3] = 100.0f;
  }

  stable_fluids::add_buoyancy(grid, 0.0f, 9.81f);

  uint32_t start_us = micros();
  stable_fluids::step(grid, 20);
  uint32_t elapsed_us = micros() - start_us;

  total_time_us += elapsed_us;
  step_count++;

  if (step_count % 60 == 0) {
    float avg_ms = (total_time_us / 60.0f) / 1000.0f;
    float fps = 1000.0 / avg_ms;
    Serial.printf("[Step %5u] Step time: %.2f ms | %.1f FPS\n", step_count,
                  avg_ms, fps);
    total_time_us = 0;
  }

  delay(1);
}

#else
#include <chrono>
#include <iostream>

/* Aquí el código para nativo */

#endif
