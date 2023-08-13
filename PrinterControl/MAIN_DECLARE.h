#include <TFT_eSPI.h>
#include <OneButton.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Arduino.h>
#include <analogWrite.h>
#include <WiFiManager.h> // Incluye la biblioteca WiFiManager
#include "TFT_MENU_SM_ENUM.h"
#include "TFT_AUX.h"
#include "EEPROM.h"
#include "NotoSans_Bold.h"
#include "OpenFontRender.h"
#include <PNGdec.h>
#include "background.h"
#include "xbm.h"


#define TTF_FONT NotoSans_Bold

// Configuración de la pantalla tft
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite topBarSprite = TFT_eSprite(&tft); // Sprite para la top bar
TFT_eSprite testFieldSprite = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object
TFT_eSprite btnSprite = TFT_eSprite(&tft);
TFT_eSprite diagBoxSprite = TFT_eSprite(&tft);
TFT_eSprite diagTableSprite = TFT_eSprite(&tft);
TFT_eSprite diagCellSprite = TFT_eSprite(&tft);
TFT_eSprite bottomBarSprite = TFT_eSprite(&tft); // Sprite para la bottom bar
OpenFontRender ofr;

PNG png; // PNG decoder inatance

int16_t xpos = 0;
int16_t ypos = 20;

OneButton myButton(BUTTON_PIN, true); // Crea una instancia del objeto OneButton con el parámetro "activeLow" como true
// Rango del joystick
const int joystickMin = 0;
const int joystickMax = 4095; // Para un joystick de 12 bits (0-4095)

// Rango deseado (0 a 100)
const int desiredMin = 0;
const int desiredMax = 100;

// Configuración del servidor NTP
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;      // Zona horaria (CET, UTC+1)
const int daylightOffset_sec = 3600;  // Desplazamiento horario en verano (CEST, UTC+2)

// Variables para la fecha y la hora
char currentDate[20];
char currentTime[10];
char currentDay[20];

enum main_sm State = CONTROL;
enum control_sm StateCT = AUTOCT;
enum cabin_sm StateCAB = DOWNCAB;
enum diag_sm StateDIAG = RST_CRTL;
enum diagPlus_sm StateDIAGPLUS = LESS_BTN;

enum diag_sts diagSTS[NUMROW-1][NUMCOL-1];

bool onScreen = 0;
bool auto_mode = 0;
int reading = 0; // Value to be displayed
typedef struct CabinData{
  // Agrega aquí las variables que quieras enviar
  bool on_off = false;
  uint8_t brightness = 1;
  uint8_t colorSelection = 0;
  int fan_cabin = 50; // Value to be displayed
} CabinData;

typedef struct WifiData{
  // Agrega aquí las variables que quieras enviar
  char ssid[32];
  char password[64];
} WifiData;



CabinData cabin;
int temperature = 0;

//ESP NOW MAC ADDRESS
uint8_t CABIN_MAC[] = {0xEC, 0x62, 0x60, 0x9C, 0x8E, 0xEC};
esp_now_peer_info_t peerInfo;
