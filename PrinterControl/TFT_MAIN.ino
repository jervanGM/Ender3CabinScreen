/********************************************************************
                            LOGIC FUNCTIONS
 *******************************************************************/
void updateDateTime() {
  // Actualizar la hora del cliente NTP
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  // Formatear la fecha y hora usando strftime()
  strftime(currentDate, sizeof(currentDate), "%d-%B-%Y", &timeinfo);
  strftime(currentDay, sizeof(currentDay), "%A", &timeinfo);
  strftime(currentTime, sizeof(currentTime), "%H:%M:%S", &timeinfo);
}

void mainMenuSM() {
  int direction = readDirection();
  static int initCT = 0;
  static int initCAB = 0;
  static int initDIAG = 0;
  switch (State) {
    case CONTROL:
      initCAB = 0;
      initDIAG = 0;
      temperature=readTemp(analogRead(TEMP_PIN));
      if(auto_mode) reading = calculatePIDOutput(TEMP_AUTO, temperature);
      drawControlScreen(initCT);
      if(!onScreen){
        if(direction == UP){
            onScreen = 1;
            StateCT = AUTOCT;
        }
        else if(direction == RIGHT){
            State = CABIN;
        }
        else{ 
          State = CONTROL;
        }
      }
      else{
        controlScreenSM();
      }
      analogWrite(OC1A_PIN,reading*2.55);
      break;
    case CABIN:
      initCT = 0;
      initDIAG = 0;
      drawCabinScreen(initCAB);
      if(!onScreen){
        if(direction == UP){
            onScreen = 1;
            StateCAB = DOWNCAB;
        }
        else if(direction == LEFT){
            State = CONTROL;
        }
        else if(direction == RIGHT){
            State = DIAG;
        }
        else{ 
          State = CABIN;
        }
      }
      else{
        cabinScreenSM();
      }
      break;
      case DIAG:
      initCT = 0;
      initCAB= 0;
      drawDiagScreen(initDIAG);
      if(!onScreen){
        if(direction == UP){
            onScreen = 1;
            StateDIAG = RST_CRTL;
        }
        else if(direction == LEFT){
            State = CABIN;
        }
        else{ 
          State = DIAG;
        }
      }
      else{
        diagScreenSM();
      }
      break;
    default:
      break;
  }
  #define ESPNOW 0
  diagSTS[CTRL][TINT] = intTempDiagnosis();
  diagSTS[CTRL][WIFI] = wifiDiagnosis();
  diagSTS[CTRL][ESPNOW] = espNowDiagnosis(espnow_status);
  cabinDiagnosis();
  drawBottomBar();
}

/********************************************************************
                            DRAWING FUNCTIONS
 *******************************************************************/
void initiateScreen()
{
  tft.fillRect(0, 0, tft.width(), tft.height(), TFT_LIGHTGREY);
  tft.setTextColor(TFT_WHITE, TFT_LIGHTGREY);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Please wait...", tft.width() / 2, tft.height() / 2);
}

void drawTopBar() {

  topBarSprite.setTextColor(TFT_WHITE, TFT_DARKGREY);
  topBarSprite.setTextSize(1);
  topBarSprite.setTextDatum(TL_DATUM);
  topBarSprite.drawString(currentDate, 5, 5);

  int dayOfWeekWidth = topBarSprite.textWidth(currentDay);
  int centerX = topBarSprite.width() / 2;
  int centerY = 5;
  topBarSprite.drawString(currentDay, centerX - dayOfWeekWidth / 2, centerY);

  int timeWidth = topBarSprite.textWidth(currentTime);
  int rightX = topBarSprite.width() - 5;
  topBarSprite.drawString(currentTime, rightX - timeWidth, 5);

  topBarSprite.pushSprite(0, 0);
}

void writeTopBar() {
  topBarSprite.fillRect(0, 0, topBarSprite.width(), 20, TFT_DARKGREY);
  
  topBarSprite.setTextColor(TFT_WHITE, TFT_DARKGREY);
  topBarSprite.setTextSize(1);
  topBarSprite.setTextDatum(TL_DATUM);
  topBarSprite.drawString(currentDate, 5, 5);

  int dayOfWeekWidth = topBarSprite.textWidth(currentDay);
  int centerX = topBarSprite.width() / 2;
  int centerY = 5;
  topBarSprite.drawString(currentDay, centerX - dayOfWeekWidth / 2, centerY);

  int timeWidth = topBarSprite.textWidth(currentTime);
  int rightX = topBarSprite.width() - 5;
  topBarSprite.drawString(currentTime, rightX - timeWidth, 5);

  topBarSprite.pushSprite(0, 0);
}

void drawBottomBar() { 
  String btn_names[BOTBAR_NUMBER] = {"CONTROL","CABIN","DIAG"};
  int backColor[BOTBAR_NUMBER] = {TFT_LIGHTGREY,TFT_LIGHTGREY,TFT_LIGHTGREY};
  int barWidth = tft.width() / BOTBAR_NUMBER;
  int barX = tft.width() / BOTBAR_NUMBER;
  int barY = tft.height() - BOTTOMBARHEIGHT;

  ofr.setDrawer(bottomBarSprite);
  backColor[State] = TFT_BLUE;
  bottomBarSprite.fillSprite(TFT_DARKGREY);
  for(int i = 0; i < BOTBAR_NUMBER; i++) {
    bottomBarSprite.fillRect(barX*i, 0, barWidth, BOTTOMBARHEIGHT, backColor[i]);
    bottomBarSprite.drawRect(barX*i, 0, barWidth, BOTTOMBARHEIGHT, TFT_DARKGREY);
    ofr.setFontColor(TFT_WHITE, backColor[0]);
    ofr.setFontSize(40);
    ofr.setCursor(barX*i + barWidth / 2 , 20);
    ofr.cprintf("%s", btn_names[i].c_str());
  }
  bottomBarSprite.pushSprite(0, barY);
}
