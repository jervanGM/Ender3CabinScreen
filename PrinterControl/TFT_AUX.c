#include "TFT_AUX.h"
#include <Arduino.h>


uint16_t get_rgb565(uint8_t* colorSelection, uint8_t* brightness) {
  // Matriz de brillo y sus valores correspondientes
  BrightnessEntry colour[][4] = {
      {{"WHITE_25", 0x4208}, {"WHITE_50", 0x7BEF}, {"WHITE_75", 0xBDF7}, {"WHITE_100", 0xFFFF}},
      {{"PURPLE_25", 0x2004}, {"PURPLE_50", 0x4008}, {"PURPLE_75", 0x6811}, {"PURPLE_100", 0x8010}},
      {{"BLUE_25", 0x0008}, {"BLUE_50", 0x0010}, {"BLUE_75", 0x0019}, {"BLUE_100", 0x001F}},
      {{"GREEN_25", 0x0140}, {"GREEN_50", 0x03A0}, {"GREEN_75", 0x05E0}, {"GREEN_100", 0x07E0}},
      {{"RED_25", 0x5800}, {"RED_50", 0xA800}, {"RED_75", 0xD000}, {"RED_100", 0xF800}},
      {{"YELLOW_25", 0x8427}, {"YELLOW_50", 0xAD66}, {"YELLOW_75", 0xD6A3}, {"YELLOW_100", 0xFFE0}},
      {{"ORANGE_25", 0x5100}, {"ORANGE_50", 0xA120}, {"ORANGE_75", 0xDAE0}, {"ORANGE_100", 0xFD20}},
      {{"PINK_25", 0x79EF}, {"PINK_50", 0xB1B7}, {"PINK_75", 0xD0FA}, {"PINK_100", 0xF81F}},
      {{"BROWN_25", 0x51A0}, {"BROWN_50", 0x7240}, {"BROWN_75", 0x8AE0}, {"BROWN_100", 0xA340}},
      {{"SKYBLUE_25", 0x224B}, {"SKYBLUE_50", 0x449B}, {"SKYBLUE_75", 0x69A5}, {"SKYBLUE_100", 0x867D}}
  };
  int length_x = sizeof(colour) / sizeof(colour[0]);
  int length_y = sizeof(colour[0]) / sizeof(colour[0][0]);
  if(*colorSelection>= length_x) *colorSelection = 0;
  if(*brightness >= length_y) *brightness = length_y-1;
  return colour[*colorSelection][*brightness].rgb565;
}

int readTemp(float Vo){
    float R1 = 10000;
    float c1 = 2.108508173e-03, c2 = 0.7979204727e-04, c3 = 6.535076315e-07;
    float R2 = R1 * ((4096.0 / Vo) - 1.0);
    float logR2 = log(R2);
    float T = 1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2);
    int Tc = (int)(T - 273.15);

    return Tc;
}

int calculatePIDOutput(int setpoint, int actual_temp) {
    // Definimos variables estáticas para mantener el estado entre llamadas
    static int integral = 0;      // Acumulación de errores para el término integral
    static int prev_error = 0;    // Error anterior para el término derivativo

    int error = actual_temp - setpoint;
    integral += error;

    // Controlamos el valor de integral para evitar un crecimiento excesivo
    if (integral > 100) integral = 100;
    else if (integral < 0) integral = 0;

    int derivative = error - prev_error;

    // Calculamos el valor del controlador PID
    int pid_output = KP * error + KI * integral + KD * derivative;

    // Controlamos el valor de salida para que esté entre 0 y 100
    if (pid_output > 100) pid_output = 100;
    else if (pid_output < 0) pid_output = 0;

    // Guardamos el error actual para usarlo en el próximo ciclo
    prev_error = error;

    return pid_output;
}
