void sendData(bool init) {
  // Comprobar si algún valor en la estructura cabin ha cambiado

  static CabinData lastSentCabin;

    if (cabin.on_off != lastSentCabin.on_off ||
      cabin.brightness != lastSentCabin.brightness ||
      cabin.colorSelection != lastSentCabin.colorSelection ||
      cabin.fan_cabin != lastSentCabin.fan_cabin ||
      init ) {
    // Enviar los datos al ESP32 receptor (cabin)
    esp_err_t result = esp_now_send(CABIN_MAC, (uint8_t *)&cabin, sizeof(cabin));
    if (result == ESP_OK) {
      //Datos enviados con éxito
      espnow_status = 0;
      // Actualizar lastSentCabin con los valores actuales de cabin
      lastSentCabin.on_off = cabin.on_off;
      lastSentCabin.brightness = cabin.brightness;
      lastSentCabin.colorSelection = cabin.colorSelection;
      lastSentCabin.fan_cabin = cabin.fan_cabin;
    } else {
      //Error al enviar los datos
      espnow_status = -11;
    }
  }
}

// Función para enviar los datos del SSID y contraseña
void sendCredentials(String ssid, String password) {
  WifiData credentials;
  // Obtener los datos del SSID y contraseña desde WiFi
  strncpy(credentials.ssid, ssid.c_str(), sizeof(credentials.ssid) - 1);
  strncpy(credentials.password, password.c_str(), sizeof(credentials.password) - 1);

  // Enviar los datos al ESP32 cabin
  esp_err_t result = esp_now_send(CABIN_MAC, (uint8_t *)&credentials, sizeof(credentials));
  if (result == ESP_OK) {
    //Datos enviados con éxito
    espnow_status = 0;
  } else {
    //Error al enviar los datos
    espnow_status = -11;
  }
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if(status != ESP_NOW_SEND_SUCCESS){
    ack_send = true;
  }
  else{
    ack_send = false;
  }
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  if (data_len == sizeof(StatusData)) {
    memcpy(&cabinSTS, data, sizeof(cabinSTS));
    ack_ok = true;
  }
}