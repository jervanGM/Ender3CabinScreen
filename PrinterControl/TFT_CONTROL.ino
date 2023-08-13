/********************************************************************
                            LOGIC FUNCTIONS
 *******************************************************************/
void controlScreenSM() {
  int direction = readDirection();
  switch (StateCT) {
    case UPCT:
      if(direction == DOWN){
            StateCT = DOWNCT;
      }
      else{ StateCT = UPCT; }
      if(((myButton.state() ==SHORTCLICK )  || (myButton.state() ==LONGCLICK ))) 
      {
        reading++;
        auto_mode = 0;
      }
      if(reading>100) reading = 100;
      break;
    case DOWNCT:
      if(direction == UP)StateCT = UPCT;
      else if(direction == DOWN)StateCT = AUTOCT;
      else StateCT = DOWNCT;
      
      if(((myButton.state() ==SHORTCLICK )  || (myButton.state() ==LONGCLICK ))) {
        reading--;
        auto_mode = 0;
      }
      if(reading<0) reading = 0;
      break;
    case AUTOCT:
      if(direction == UP)StateCT = DOWNCT;
      else if(direction == DOWN) onScreen = 0; 
      else StateCT = AUTOCT; 

      if(myButton.state() == SHORTCLICK) auto_mode = !auto_mode;
      break;
    default:
      break;
  }

}

/********************************************************************
                            DRAWING FUNCTIONS
 *******************************************************************/
void drawControlScreen(int &init)
{
   if(!init){
    drawBackground();
    //tft.fillRect(0, TOPBARHEIGHT, tft.width(), tft.height() - (TOPBARHEIGHT + BOTTOMBARHEIGHT), TFT_BLACK);
    initmeter();
    init = 1;
  }
  int buttonState[4] = {OFF,OFF,OFF,0};
  buttonState[StateCT] = onScreen;
  drawButton("UP", 30, 45, 180, 45, buttonState[0]);
  drawButton("DOWN", 30, 115, 180, 45, buttonState[1]);
  drawButton("AUTO", 30, 185, 180, 45, buttonState[2]);
  ringMeter(String(temperature), String(reading)); // Draw analogue meter
}

// #########################################################################
//  Specific control screen drawing functions
// #########################################################################
void initmeter(){
    tft.fillCircle(ARCTFTXPOS, ARCTFTYPOS, RADIUS, DARKER_GREY);
    tft.drawSmoothCircle(ARCTFTXPOS, ARCTFTYPOS, RADIUS, TFT_SILVER, DARKER_GREY);
    tft.drawArc(ARCTFTXPOS, ARCTFTYPOS, RADIUSTMP, RADIUSTMP - RADIUSTMP / 5, 30, 330, TFT_BLACK, DARKER_GREY);
}
void ringMeter(String val, String percentage)
{
  int value = percentage.toInt();
  // Range here is 0-100 so value is scaled to an angle 30-330
  int val_angle = map(value, 0, 100, 30, 330);

  drawTextField(val, (6 * RADIUSTMP) / 4, TFT_WHITE, DARKER_GREY ,"44", 40, ARCTFTXPOS - 45, ARCTFTYPOS - 60 / 2);
  drawTextField("C", RADIUSTMP / 2.0, TFT_WHITE, DARKER_GREY ,"4", 6, ARCTFTXPOS + 45, ARCTFTYPOS - 60 / 2);
  drawTextField(percentage + "%", RADIUSTMP / 2.0, TFT_GOLD, DARKER_GREY ,"44444", 30,ARCTFTXPOS - 30, ARCTFTYPOS + 63);

  // Allocate a value to the arc thickness dependant of RADIUS
  uint8_t thickness = RADIUSTMP / 5;
  int ring_color = TFT_SKYBLUE;
  if(auto_mode) ring_color = TFT_RED;
  // Update the arc, only the zone between last_angle and new val_angle is updated
  tft.drawArc(ARCTFTXPOS, ARCTFTYPOS, RADIUSTMP, RADIUSTMP - thickness, 30, val_angle, ring_color, TFT_BLACK); // TFT_SKYBLUE random(0x10000)
  tft.drawArc(ARCTFTXPOS, ARCTFTYPOS, RADIUSTMP, RADIUSTMP - thickness, val_angle, 330, TFT_BLACK, TFT_BLACK);
}