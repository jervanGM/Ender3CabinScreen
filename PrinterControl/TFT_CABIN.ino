/********************************************************************
                            LOGIC FUNCTIONS
 *******************************************************************/
void cabinScreenSM() {

  int direction = readDirection();
  switch (StateCAB) {
    case UPCAB:
      if(((myButton.state() ==SHORTCLICK )  || (myButton.state() ==LONGCLICK ))) cabin.fan_cabin++;
      if(cabin.fan_cabin>100) cabin.fan_cabin = 100;
      if(direction == DOWN)StateCAB = DOWNCAB;
      else if(direction == RIGHT) StateCAB = ONOFFCAB;
      else StateCAB = UPCAB;
      break;
    case DOWNCAB:
      if(((myButton.state() ==SHORTCLICK )  || (myButton.state() ==LONGCLICK ))) cabin.fan_cabin--;
      if(cabin.fan_cabin <0) cabin.fan_cabin = 0;
      if(direction == UP)StateCAB = UPCAB;
      else if(direction == DOWN) onScreen = 0;
      else if(direction == RIGHT) StateCAB = LMINUSCAB;
      else StateCAB = DOWNCAB;
      break;
    case ONOFFCAB:
      if(myButton.state() == SHORTCLICK) cabin.on_off = !cabin.on_off;
      if(direction == DOWN)StateCAB = LPLUSCAB;
      else if(direction == LEFT) StateCAB = UPCAB;
      else if(direction == RIGHT) StateCAB = COLORCAB;
      else StateCAB = ONOFFCAB;
      break;
    case LPLUSCAB:
      if(myButton.state() ==SHORTCLICK ) cabin.brightness++;
      if(direction == UP )StateCAB = ONOFFCAB;
      else if(direction == DOWN)StateCAB = LMINUSCAB;
      else if(direction == LEFT) StateCAB = UPCAB;
      else if(direction == RIGHT) StateCAB = COLORCAB;
      else StateCAB = LPLUSCAB;
      break;
    case LMINUSCAB:
      if(myButton.state() ==SHORTCLICK ) cabin.brightness--;
      if(direction == UP )StateCAB = LPLUSCAB;
      else if(direction == DOWN) onScreen = 0;
      else if(direction == LEFT) StateCAB = DOWNCAB;
      else if(direction == RIGHT) StateCAB = COLORCAB;
      else StateCAB = LMINUSCAB;
      break;
    case COLORCAB:
      if(myButton.state() ==SHORTCLICK) cabin.colorSelection++;  
      if(direction == UP )StateCAB = ONOFFCAB;
      else if(direction == DOWN) onScreen = 0;
      else if(direction == LEFT) StateCAB = LPLUSCAB;
      else StateCAB = COLORCAB;
      break;
    default:
      //onScreen = 0;
      break;
  }

}

/********************************************************************
                            DRAWING FUNCTIONS
 *******************************************************************/
void drawCabinScreen(int &init)
{
  if(!init){
    drawBackground();
    //tft.fillRect(0, TOPBARHEIGHT, tft.width(), tft.height() - (TOPBARHEIGHT + BOTTOMBARHEIGHT), TFT_BLACK);
    tft.drawRect(24,44, 48,186, TFT_LIGHTGREY);
    initcolorSelector();
    init = 1;
  }
  int buttonState[6] = {OFF,OFF,OFF,OFF,OFF,0};
  buttonState[StateCAB] = onScreen;
  drawXBMButton(ic_up_cab, 120, 44, 72, 72, buttonState[0]);
  drawXBMButton(ic_downcab, 120, 158, 72, 72, buttonState[1]);
  drawXBMButton(ic_onoff, 264, 44, 48, 48, buttonState[2]);
  drawXBMButton(ic_bright, 264, 113, 48, 48, buttonState[3]);
  drawXBMButton(ic_dark, 264, 182, 48, 48, buttonState[4]);
  linearMeter(String(cabin.fan_cabin)); // Draw analogue meter
  colorSelector();
}

// #########################################################################
//  Specific cabin screen drawing functions
// #########################################################################
void linearMeter(String percentage)
{
  int value = percentage.toInt();
  // Range here is 0-100 so value is scaled to an angle 30-330
  int val_progress = map(value, 0, 100, 0, 184);

  drawTextField(percentage + "%", RADIUSTMP / 3.0, TFT_GOLD, TFT_BLACK ,"44444", 25 ,24, 236);

  // Update the arc, only the zone between last_angle and new val_angle is updated
  tft.fillRect(25, 45, 46, (184 - val_progress), DARKER_GREY);
  tft.fillRect(25 ,229-val_progress, 46, val_progress, TFT_SKYBLUE);
}

void initcolorSelector(){
    tft.fillCircle(408, 137, 48, DARKER_GREY);
    tft.drawSmoothCircle(408, 137, 48, TFT_SILVER, DARKER_GREY);
}
void colorSelector()
{
  static uint16_t rgb565 = 0;
  if(StateCAB == COLORCAB){
    tft.drawSmoothCircle(408, 137, 50, TFT_BLUE, TFT_BLACK);
  }
  else{
    tft.drawSmoothCircle(408, 137, 50, TFT_BLACK, TFT_BLACK);
  }
  rgb565 = get_rgb565(&cabin.colorSelection, &cabin.brightness);
  tft.fillCircle(408, 137, 47, rgb565*cabin.on_off);
}
