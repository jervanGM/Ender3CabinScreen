/********************************************************************
                            LOGIC FUNCTIONS
 *******************************************************************/
void diagScreenSM() {
  int direction = readDirection();
  switch (StateDIAG) {
    case RST_CRTL:
      if(direction == UP) StateDIAG = RST_CAB;
      else if(direction == DOWN) onScreen = 0; 
      else  StateDIAG = RST_CRTL; 
      if(myButton.state() ==SHORTCLICK ) ESP.restart();
      break;
    case RST_CAB:
      if(direction == UP) StateDIAG = SEND_WIFI;
      else if(direction == DOWN) StateDIAG = RST_CRTL;
      else  StateDIAG = RST_CAB;
      
      if(myButton.state() ==SHORTCLICK ){
        //Mandar mensaje para hacer reset al ESP de cabina
      }
      if(reading<0) reading = 0;
      break;
    case SEND_WIFI:
      if(direction == UP) StateDIAG = MORE_BTN;
      else if(direction == DOWN) StateDIAG = RST_CAB; 
      else  StateDIAG = SEND_WIFI; 

      if(myButton.state() == SHORTCLICK){
            // Enviar los datos al ESP32 configurado como "cabin"
            sendCredentials(WiFi.SSID(), WiFi.psk());
      }
      break;
    case MORE_BTN:
        if(direction == DOWN) StateDIAG = SEND_WIFI; 
        else  StateDIAG = MORE_BTN; 
        //Acceder a la otra pantalla sin bottom bar
        break;
    default:
      break;
  }

}

uint8_t espNowDiagnosis(){
  
}

uint8_t wifiDiagnosis(){
  
}

enum diag_sts intTempDiagnosis(){
    enum diag_sts tdiag = STSOK;
    float internal_temp = ((temprature_sens_read() - 32) / 1.8);
    if(internal_temp < -35 || internal_temp > 80 ) tdiag = STSFAIL;
    else if (internal_temp < -20 || internal_temp > 65 ) tdiag = STSWARN;
    else if (internal_temp >= -20 && internal_temp <= 65 ) tdiag = STSOK;
    else tdiag = STSUNKN;

    return tdiag;
}

/********************************************************************
                            DRAWING FUNCTIONS
 *******************************************************************/
void drawDiagScreen(int &init)
{
   if(!init){
    drawBackground();
    drawDiagTable(init);
    init = 1;
  }
  int buttonState[5] = {OFF,OFF,OFF,OFF,0};
  buttonState[ StateDIAG ] = onScreen;
  uint8_t buttonWidth = 45;
  uint8_t buttonHeight = 45;
  uint16_t buttonStart = 415;
  static bool status = 0;
  uint16_t color = TFT_GREEN;
  String text = "Devices status : OK";
  drawDiagBox(text, color);
  drawDiagTable(init);
  drawXBMButton(left_arrow, buttonStart, 30, buttonWidth, buttonHeight, buttonState[3]);
  drawXBMButton(ic_cred, buttonStart, 90, buttonWidth, buttonHeight, buttonState[2]);
  drawXBMButton(ic_rst_cabin, buttonStart, 140, buttonWidth, buttonHeight, buttonState[1]);
  drawXBMButton(ic_rst_ctrl, buttonStart, 190, buttonWidth, buttonHeight, buttonState[0]);
}

void drawDiagBox(String text, uint16_t diag_color){
  diagBoxSprite.setColorDepth(8);
  diagBoxSprite.createSprite(370, 45);
  diagBoxSprite.fillSprite(TFT_BLACK);
  diagBoxSprite.drawRect(0,0,diagBoxSprite.width(),diagBoxSprite.height(), TFT_LIGHTGREY);
  diagBoxSprite.setTextColor(diag_color);
  diagBoxSprite.setTextSize(2);
  diagBoxSprite.setTextDatum(ML_DATUM);
  diagBoxSprite.drawString(text.c_str(), 10, diagBoxSprite.height() / 2);
  diagBoxSprite.pushSprite(20, 30); // Push Sprite containing the val number
  diagBoxSprite.deleteSprite(); 
  
}

void drawDiagTable(int init){
    int tableWidth = 370;
    int tableHeight = 145;
    int cellWidth = tableWidth / NUMCOL;
    int cellHeight = tableHeight / NUMROW;
    int X_start = 20;
    int Y_start = 90;
    String text_data[NUMROW][NUMCOL] = { {"","ESPN","OTA","Wifi","Tint"},
                                        {"CTRL","D","D","D","D"},
                                        {"CABIN","D","D","D","D"}};
  if(!init){
    ofr.setDrawer(tft);   
    // Dibuja bordes de la tabla
    tft.drawRect(X_start, Y_start, tableWidth, tableHeight, TFT_WHITE);
    // Dibuja celdas y llena con datos
    for (int row = 0; row < NUMROW; row++) {
      for (int col = 0; col < NUMCOL; col++) {
        int x = X_start + col * cellWidth;
        int y = Y_start + row * cellHeight;
        tft.drawRect(x, y, cellWidth, cellHeight, TFT_WHITE);
        if(!text_data[row][col].equals("") && !text_data[row][col].equals("D")){
          ofr.setFontColor(TFT_WHITE, TFT_BLACK);
          ofr.setFontSize(40);
          ofr.setCursor(x + cellWidth/2, y + cellHeight/6);
          ofr.cprintf("%s",text_data[row][col]);
        }
      }
    }
  }
  else{
    for (int row = 1; row < NUMROW; row++) {
      for (int col = 1; col < NUMCOL; col++) {
        int x = X_start + col * cellWidth;
        int y = Y_start + row * cellHeight;
        drawDevDiangosis(diagSTS[CTRL][TINT], x , y ,cellWidth,cellHeight);
      }
    }
  }
}

void drawDevDiangosis(enum diag_sts State, int x, int y, int w, int h){
  String text = "OK";
  uint16_t color = TFT_GREEN;

  switch(State){
    case STSOK:
      text = "OK";
      color = TFT_GREEN;
    break;
    case STSWARN:
      text = "WARN";
      color = TFT_ORANGE;
    break;
    case STSFAIL:
      text = "FAIL";
      color = TFT_RED;
    break;
    case STSUNKN:
      text = "X";
      color = TFT_WHITE;
    break;  
    default:
    break;  
  }
  uint16_t backcolor  = tft.color24to16(0x222222);
  ofr.setDrawer(diagCellSprite);
  diagCellSprite.setColorDepth(8);
  diagCellSprite.createSprite(w-2, h-2);
  diagCellSprite.fillSprite(backcolor);
  ofr.setFontColor(color,backcolor);
  ofr.setFontSize(40);
  ofr.setCursor(w/2, h/5);
  ofr.cprintf("%s",text);
  diagCellSprite.pushSprite(x, y); // Push Sprite containing the val number
  diagCellSprite.deleteSprite(); 


}
