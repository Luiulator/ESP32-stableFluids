#include "stable_fluids/StableFluids.hpp"
#include "stable_fluids/forces.hpp"

#include <Arduino.h>

/* Aquí el código de la ESP32 */

constexpr size_t NX = 64;
constexpr size_t NY = 16;

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1       // No usamos pin de reset físico
#define SCREEN_ADDRESS 0x3C // Dirección I2C típica para 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*
Instancio la malla. tenemos que instanciarla como variable global para que se
vaya a la SRAM de la ESp32. De lo contrario, intentaría alocar los bytes
correspondientes a la stack de 8 KB, o sea, stack overflow.
*/

stable_fluids::FluidGrid2D<NX, NY> grid;

void renderFluidToOLED() {
  display.clearDisplay();

  const float MAX_DENSITY = 100.0f;

  // Mi intento de representar algunos tonos de gris mediante dithering. Resultó ser la clave lol.

  const int bayer[2][2] = {{0, 2}, {3, 1}};

  for (size_t i = 1; i <= NX; i++) {
    for (size_t j = 1; j <= NY; j++) {
      float d = grid.density[i][j];
      if (d <= 0.05f)
        continue;

      int gray_level = (int)((d / MAX_DENSITY) * 4.0f);
      if (gray_level > 4)
        gray_level = 4;
      if (gray_level < 1 && gray_level >= 0.2f)
        gray_level = 1;

      int base_x = (i - 1) * 2;
      int base_y = (NY - j) * 2;

      for (int py = 0; py < 2; py++) {
        for (int px = 0; px < 2; px++) {
          if (gray_level > bayer[py][px]) {
            display.drawPixel(base_x + px, base_y + py, SSD1306_WHITE);
          }
        }
      }
    }
  }

  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22);

  Wire.setClock(400000);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Error: No se encontró la pantalla SSD1306");
    for (;;)
      ; // Detener si falla
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println("CFD Ready!");
  display.display();
  delay(1000);

  Serial.println("\nStable Fluids en una ESP32");
  Serial.printf("Grid de %u x %u (%u celdas)\n", (unsigned)NX, (unsigned)NY,
                (unsigned)(NX * NY));
  Serial.printf("Tamaño de malla: %.2f KB\n", sizeof(grid) / 1024.0f);

  grid.params.dt = 0.016f;
  grid.params.visc = 0.01f;
  grid.params.diff = 0.01f;

  Serial.println("Setup completada");
}

uint32_t step_count = 0;
uint32_t total_time_us = 0;

void loop() {

  for (size_t i = NY / 2 - 2; i <= NY / 2 + 2; i++) {
    grid.density[3][i] = 200.0f;
  }

  stable_fluids::add_buoyancy(grid, 9.81f, 0.0f);

  uint32_t start_us = micros();
  stable_fluids::step(grid, 25);
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

  renderFluidToOLED();

  delay(1);
}