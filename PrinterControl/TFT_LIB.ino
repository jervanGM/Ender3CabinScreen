/********************************************************************
                            LOGIC FUNCTIONS
 *******************************************************************/
// Función para mapear valores del joystick al rango deseado
int mapJoystickValue(int value) {
  return map(value, joystickMin, joystickMax, desiredMin, desiredMax);
}

bool joyLeftPressed(int joyX) {
  static bool pressed_prev = 0;
  bool pressed = (joyX > 60);
  bool isLeftPressed = 0;
  FL_UP(isLeftPressed,1,pressed_prev,pressed);
  pressed_prev = pressed;
  return isLeftPressed;
}

bool joyRightPressed(int joyX) {
  static bool pressed_prev = 0;
  bool pressed = (joyX < 40);
  bool isRightPressed = 0;
  FL_UP(isRightPressed,1,pressed_prev,pressed);
  pressed_prev = pressed;
  return isRightPressed;
}

bool joyUpPressed(int joyY) {
  static bool pressed_prev = 0;
  bool pressed = (joyY < 40);
  bool isUpPressed = 0;
  FL_UP(isUpPressed,1,pressed_prev,pressed);
  pressed_prev = pressed;
  return isUpPressed;
}

bool joyDownPressed(int joyY) {
  static bool pressed_prev = 0;
  bool pressed = (joyY > 60);
  bool isDownPressed = 0;
  FL_UP(isDownPressed,1,pressed_prev,pressed);
  pressed_prev = pressed;
  return isDownPressed;
}

int readDirection() {

  // Leer los valores del joystick en los pines analógicos
  int xValue = analogRead(JOY_X_PIN);
  int yValue = analogRead(JOY_Y_PIN);
  // Mapear los valores a un rango de 0 a 100
  int mappedX = mapJoystickValue(xValue);
  int mappedY = mapJoystickValue(yValue);
  static int direction = CENTER;
  if(joyLeftPressed(mappedX) && (!joyUpPressed(mappedY) || !joyDownPressed(mappedY))){
    direction = LEFT;
  }
  else if(joyRightPressed(mappedX) && (!joyUpPressed(mappedY) || !joyDownPressed(mappedY))){
    direction = RIGHT;
  }
  else if(joyUpPressed(mappedY) && (!joyLeftPressed(mappedX) || !joyRightPressed(mappedX))){
    direction = UP;
  }
  else if(joyDownPressed(mappedY) && (!joyLeftPressed(mappedX) || !joyRightPressed(mappedX))){
    direction = DOWN;
  }
  else{
    direction = CENTER;
  }
  // Imprimir los valores mapeados
//  Serial.print("Direction: ");
//  Serial.print(direction);
//  Serial.print(", PRESSED: ");
//  Serial.println(myButton.state());
  return direction;
}
// #########################################################################
//  General drawing functions
// #########################################################################
void drawButton(String text, int x, int y, int w, int h, int State){
  int backcolor = TFT_DARKGREY;
  if(State){
    backcolor = TFT_BLUE;
  }
  ofr.setDrawer(btnSprite);
  btnSprite.createSprite(w, h);
  btnSprite.fillRoundRect(0, 0, w, h,ROUNDRADIUS, backcolor);
  btnSprite.drawRoundRect(0, 0, w, h,ROUNDRADIUS, TFT_LIGHTGREY);
  ofr.setFontColor(TFT_WHITE, backcolor);
  ofr.setFontSize(40);
  ofr.setCursor(w / 2 , h / 6);
  ofr.cprintf("%s",text);
  btnSprite.pushSprite(x, y); // Push Sprite containing the val number
  btnSprite.deleteSprite();                   // Recover used memory
  
}

void drawXBMButton(const XBMLogo logo, int x, int y, int w, int h, int State){
  int backcolor = TFT_DARKGREY;
  if(State){
    backcolor = TFT_BLUE;
  }
  btnSprite.createSprite(w, h);
  btnSprite.fillRoundRect(0, 0, w, h,ROUNDRADIUS, backcolor);
  btnSprite.drawRoundRect(0, 0, w, h,ROUNDRADIUS, TFT_LIGHTGREY);
  btnSprite.drawXBitmap(w/2 - logo.width/2, h/2- logo.height/2, logo.data, logo.width, logo.height, logo.color, backcolor);
  btnSprite.pushSprite(x, y); // Push Sprite containing the val number
  btnSprite.deleteSprite();                   // Recover used memory
  
}

void drawTextField(String text, int fontsize, int textcolor, int backcolor,const char *format, int xCursor, int xPos, int yPos){
      ofr.setDrawer(testFieldSprite);
      ofr.setFontSize(fontsize);
      uint8_t w = ofr.getTextWidth(format);
      uint8_t h = ofr.getTextHeight("4") + 4;
      testFieldSprite.createSprite(w , h + 2);
      testFieldSprite.fillSprite(backcolor); // (TFT_BLUE); // (DARKER_GREY);
      ofr.setFontColor(textcolor, backcolor);
      ofr.setCursor(xCursor , -h / 2.5);
      ofr.cprintf("%s",text);
      testFieldSprite.pushSprite(xPos, yPos); // Push Sprite containing the val number
      testFieldSprite.deleteSprite();                   // Recover used memory
}

void drawBackground(){
  int16_t rc = png.openFLASH((uint8_t *)panda, sizeof(panda), pngDraw);
  if (rc == PNG_SUCCESS) {
    tft.startWrite();
    rc = png.decode(NULL, 0);
    tft.endWrite();
  }
}

void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
