#include "MAIN_DECLARE.h"

void setup() {
  // Inicializar la pantalla tft
  Serial.begin(9600);
  //GPIO initialization
  GPIOSetup();
  // Inicializar TFT
  TFTSetup();
  // Loading a font takes a few milliseconds, so for test purposes it is done outside the test loop
  if (ofr.loadFont(TTF_FONT, sizeof(TTF_FONT))) {
    Serial.println("Render initialize error");
    return;
  } 
  //ESPNow Initialization
  ESPNowSetup();
  //Wifi Initialization
  WifiSetup();
  //Arduino OTA Configuration
  OTASetup();
  tft.fillScreen(TFT_BLACK);
  // Inicializar el servidor NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //EEPROM Initialization
  EEPROMSetup();
  // Inicializar la UI
  drawTopBar();
  //initiateScreen();
  drawBottomBar();
}

void loop() {
   ArduinoOTA.handle();
  //Serial.printf("Memoria libre %u bytes\n", ESP.getFreeSketchSpace());
  // Leer el botón
  myButton.tick();
  // Leer la fecha y la hora actual
  updateDateTime();
  // Actualizar la topbar
  writeTopBar();
  //Ejecutar menu
  mainMenuSM();
  //ofr.unloadFont(); // Recover space used by font metrics etc
  //Write persistent values
  EEPROM_write();
  sendData(0);
}
