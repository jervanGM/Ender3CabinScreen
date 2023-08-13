#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Arduino.h>
#include <analogWrite.h>
#include <esp_now.h>
#include <FastLED.h>

//LED Strip config
#define LED_PIN     13
#define NUM_LEDS    10
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

CRGB previousColor = CRGB::Black;
CRGB currentColor = CRGB::Black;


// Define los colores que deseas utilizar
DEFINE_GRADIENT_PALETTE(heatmap_gp) {
    0,  255,255,255,//CRGB::White,
    28, 39,148,201,//CRGB::Purple,
    56, 0,0,255,//CRGB::Blue,
    84, 255,0,0,//CRGB::Green,
    112, 0,255,0,//CRGB::Red,
    140, 255,255,0,//CRGB::Yellow,
    168, 184,255,0,//CRGB::Orange,
    196, 0,255,255,//CRGB::Pink,
    224, 118,193,27,//CRGB::Brown,
    255, 229,100,255,//CRGB::SkyBlue
};



// PMK and LMK keys
static const char* PMK_KEY_STR = "C8nSJlxlsg4NJM+Bv+RF7A";
static const char* LMK_KEY_STR = "OJbj14wukgzaqibYILLOpA";
uint8_t CTRL_MAC[] = {0xEC, 0x62, 0x60, 0x9C, 0x4E, 0x50};
#define  OC1B_PIN 19
//uint8_t CTRL_MAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
typedef struct {
  // Agrega aquí las variables que quieras enviar
  bool on_off = false;
  uint8_t brightness = 1;
  uint8_t colorSelection = 0;
  int fan_cabin = 50; // Value to be displayed
} CabinData;

typedef struct {
  // Agrega aquí las variables que quieras enviar
  char ssid[32];
  char password[64];
} WifiData;

bool wifi_received = 0;
WifiData credentials;
CabinData cabin;
// Registrar el nodo remoto (ESP32 controlador)
esp_now_peer_info_t peerInfo;
CRGBPalette16 targetPalette = heatmap_gp;
TBlendType    currentBlending;

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {

  if (data_len == sizeof(CabinData)) {
    Serial.println("RECIBO MENSAJES A");
    memcpy(&cabin, data, sizeof(cabin));
  }
  else if(data_len == sizeof(WifiData)){
    Serial.println("RECIBO MENSAJES B");
    memcpy(&credentials, data, sizeof(credentials));
    wifi_received = 1;
  }
}

void OTASetup(){
    // Nombre en Red, defecto esp3232-[MAC]
   ArduinoOTA.setHostname("ESP32 Cabin");

  // Controaseña, defecto no contraseña
   ArduinoOTA.setPassword("ESP,cabin");
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

void WifiConnect(){
  while(!wifi_received){
      Serial.println("Wifi credentials not received yet"); 
      delay(500);
  }
  
  WiFi.begin(credentials.ssid, credentials.password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void ESPNowSetup(){
    // Inicializar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al inicializar ESP-NOW");
  }
  // Setting the PMK key
  esp_now_set_pmk((uint8_t *)PMK_KEY_STR);

  memcpy(peerInfo.peer_addr, CTRL_MAC, 6);
  peerInfo.channel = 0;
  // Setting the master device LMK key
  for (uint8_t i = 0; i < 16; i++) {
    peerInfo.lmk[i] = LMK_KEY_STR[i];
  }
  peerInfo.encrypt = true;
  esp_now_add_peer(&peerInfo);


  // Registrar la función de recepción de datos
  esp_now_register_recv_cb(OnDataRecv);
}

void setup() {
  Serial.begin(9600);
  pinMode(OC1B_PIN, OUTPUT);
  analogWriteResolution(OC1B_PIN, 8);
  // Inicializar WiFi en modo estación (STA)
  WiFi.mode(WIFI_AP_STA);

  //ESP NOW SETUP
  ESPNowSetup();
  //Wifi Connection
  WifiConnect();
  //Configure OTA
  OTASetup();

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
    
  currentBlending = NOBLEND;  

}

void loop() {
  ArduinoOTA.handle();
  analogWrite(OC1B_PIN,cabin.fan_cabin*2.55);
  FillLEDsFromPaletteColors(cabin.colorSelection);
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
    uint8_t brightness = map(cabin.brightness, 0, 3, 64, 255);
    uint8_t color = map(colorIndex, 0, 9, 0, 255);

    CRGB targetColor = ColorFromPalette(targetPalette, color, brightness * cabin.on_off, currentBlending);

    // Realiza una transición suave entre el color actual y el nuevo color seleccionado
    for (uint8_t i = 0; i <= 128; i++) {
        currentColor = blend(previousColor, targetColor, i);
        for (int j = 0; j < NUM_LEDS; j++) {
            leds[j] = currentColor;
        }
        FastLED.show();
        delay(1); // Ajusta este valor para controlar la velocidad de la transición
    }

    // Almacena el color actual como el color previo para la próxima transición
    previousColor = currentColor;
}
