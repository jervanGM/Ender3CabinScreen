#ifndef _TFT_MENU_SM_ENUM
#define	_TFT_MENU_SM_ENUM

// PMK and LMK keys
static const char* PMK_KEY_STR = "C8nSJlxlsg4NJM+Bv+RF7A";
static const char* LMK_KEY_STR = "OJbj14wukgzaqibYILLOpA";
//MEMORY ADDRESS
#define EEPROM_SIZE 64

#define FAN_CTRL_ADDR 0
#define MODE_CTRL_ADDR 1
#define FAN_CAB_ADDR 2
#define COLOR_CAB_ADDR 3
#define BRIGH_CAB_ADDR 4
//CONSTANTS DECLARATION
#define CENTER 0
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

#define ON 1
#define OFF 0

#define BOTBAR_NUMBER 3

#define ACCESS_POINT "MiESP32AP"
#define DARKER_GREY 0x18E3
#define MAX_IMAGE_WDITH 480 // Adjust for your images
// Configuración del botón
#define BUTTON_PIN 17
#define JOY_X_PIN 35
#define JOY_Y_PIN 34

#define TEMP_PIN 33
#define OC1A_PIN 18

#define PWM_FREQUENCY 1000 // Frecuencia PWM en Hz
#define PWM_RESOLUTION 8 // Resolución del PWM (8 bits)

#define TOPBARHEIGHT 20 //Minimum 20px for top bar
#define BOTTOMBARHEIGHT 65 //Minimum 65px for bottom bar

#define ROUNDRADIUS 5

//ArcMeter
#define ARCTFTXPOS 360
#define ARCTFTYPOS 140
#define RADIUS 100
#define RADIUSTMP 97 //Radius - 3

//Button states
#define STARTCLICK 1
#define SHORTCLICK 2
#define ENDSHORTCLICK 3
#define LONGCLICK 6
#define ENDLONGCLICK 7

#define TEMP_AUTO 25

//DIAGNOSIS TABLE DECLARATIONS
#define NUMCOL 5
#define NUMROW 3

#define ESPNOW 0
#define OTA 1
#define WIFI 2
#define TINT 3

#define CTRL 0
#define CAB 1

//============================================================================
//== Devuelve flanco de subida si se da en la entrada con enable activo 
//== (2 posibles entradas)
//============================================================================
#define			 FL_UP(sal, en1, ant1, act1) \
					{sal =  ((en1) && (!ant1) && (act1));} 

// Status download
enum main_sm 
{
  CONTROL = 0, 
  CABIN,
  DIAG
};

enum control_sm 
{
    UPCT = 0, 
	DOWNCT,
    AUTOCT
};

enum cabin_sm 
{
    UPCAB = 0, 
	DOWNCAB = 1,
    ONOFFCAB = 2,
    LPLUSCAB = 3,
    LMINUSCAB = 4,
    COLORCAB = 5
};

enum diag_sm 
{
    RST_CRTL = 0, 
	RST_CAB = 1,
    SEND_WIFI = 2,
    MORE_BTN = 3
};

//Table diagnosis status
enum diag_sts 
{
    STSOK = 0, 
    STSWARN,
    STSFAIL,
    STSUNKN
};

enum diagPlus_sm 
{
    LESS_BTN = 0, 
	UPDIAG = 1,
    DOWNDIAG = 2,
    CLEAR_LOG = 3
};

#endif
