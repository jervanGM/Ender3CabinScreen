

void TFTSetup(){
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  topBarSprite.setColorDepth(8);
  topBarSprite.createSprite(tft.width(), TOPBARHEIGHT);
  topBarSprite.fillSprite(TFT_BLACK);

  bottomBarSprite.setColorDepth(8);
  bottomBarSprite.createSprite(tft.width(), BOTTOMBARHEIGHT);
  bottomBarSprite.fillSprite(TFT_DARKGREY);
}

void GPIOSetup(){
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configura el pin del botón como entrada con resistencia pull-up
  pinMode(TEMP_PIN, INPUT);
  pinMode(OC1A_PIN, OUTPUT);

  //analogWriteResolution(OC1A_PIN, PWM_RESOLUTION);
}

void WifiSetup() {
  // Inicializar WiFiManager
  WiFiManager wifiManager;
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("No hay una red wifi conectada,", tft.width() / 2, tft.height() / 2 - 30);
  tft.drawString("conéctese al siguiente punto de acceso", tft.width() / 2, tft.height() / 2 - 10);
  tft.drawString("para introducir los datos de red:", tft.width() / 2, tft.height() / 2 + 10);
  tft.drawString(String(ACCESS_POINT), tft.width() / 2, tft.height() / 2 + 30);
  wifiManager.autoConnect(ACCESS_POINT); // Nombre del punto de acceso si no encuentra credenciales guardadas

  // Enviar los datos al ESP32 configurado como "cabin"
  sendCredentials(WiFi.SSID(), WiFi.psk());
}

void ESPNowSetup(){
    // Inicializar WiFi en modo estación (STA)
  WiFi.mode(WIFI_AP_STA);

  // Inicializar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    //"Error al inicializar ESP-NOW
    espnow_status = -1;
  }
  else{
    // Setting the PMK key
    if(esp_now_set_pmk((uint8_t *)PMK_KEY_STR)!= ESP_OK){
      //Primary key not set
      espnow_status = -6;
    };
    // Once ESPNow is successfully Init, we will register for Send CB to
    // Registrar el nodo remoto (ESP32 cabin)
    memcpy(peerInfo.peer_addr, CABIN_MAC, 6);
    peerInfo.channel = 0; 
    // Setting the master device LMK key
    for (uint8_t i = 0; i < 16; i++) {
      peerInfo.lmk[i] = LMK_KEY_STR[i];
    }
    peerInfo.encrypt = true;
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      //Failed to add peer
      espnow_status = -2;
    }
    else{
      // get the status of Trasnmitted packet
      if(esp_now_register_send_cb(OnDataSent)!= ESP_OK){
        //Sen functions not registered
        espnow_status = -10;
      }
      // get the status of Trasnmitted packet
      if(esp_now_register_recv_cb(OnDataRecv)!= ESP_OK){
        //Sen functions not registered
        espnow_status = -9;
      }
    }
  }
}

void OTASetup(){
    // Puerto de programacion, defecto 3232
  // ArduinoOTA.setPort(3232);

  // Nombre en Red, defecto esp3232-[MAC]
   ArduinoOTA.setHostname("ESP32 Printer");

  // Controaseña, defecto no contraseña
   ArduinoOTA.setPassword("ESP,printer");
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    Serial.println("Iniciando Programación " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nTerminando");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progreso: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  ArduinoOTA.begin();
}

void EEPROMSetup(){
  //Reading persistent values
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM");
  }
  else{
    EEPROM_read();
    sendData(1);
  }
}
