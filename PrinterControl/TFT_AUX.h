#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#ifndef _TFT_AUX
#define	_TFT_AUX


// Definimos las constantes del controlador PID
#define KP 1   // Constante Proporcional
#define KI 0.4  // Constante Integral
#define KD 0.05  // Constante Derivativa
typedef struct {
    const char* name;
    uint16_t rgb565;
} BrightnessEntry;

// Matriz de brillo y sus valores correspondientes

#ifdef __cplusplus
 extern "C" {
#endif
uint8_t temprature_sens_read();
uint16_t get_rgb565(uint8_t* colorSelection, uint8_t* brightness);
int readTemp(float Vo);
int calculatePIDOutput(int setpoint, int actual_temp);

#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();
#endif
