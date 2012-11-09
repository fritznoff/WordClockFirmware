#include <avr/pgmspace.h>

#define EE_WC_BR 0x20

#define ON   0b00000000
#define OFF  0b00000001
#define PWM0 0b00000010
#define PWM1 0b00000011

uint8_t wordClockBrightness = 255;

/*
  there are actually 7bit needed, to describe the position of each led
  3bit are the ic identifier and 4bit describe the actual led connected to the ic 
  example:     
   progmembyte     n/a   ic-id  led-id
                   \_/   \___/  \____/
   0b01010111:  0b  0     101    0111
  
  
                original
  
  *                                   *
     E  S  K  I  S  T  L  F  Ü  N  F
     Z  E  H  N  Z  W  A  N  Z  I  G
     D  R  E  I  V  I  E  R  T  E  L
     T  G  N  A  C  H  V  O  R  J  M
     H  A  L  B  Q  Z  W  Ö  L  F  P
     Z  W  E  I  N  S  I  E  B  E  N
     K  D  R  E  I  R  H  F  Ü  N  F
     E  L  F  N  E  U  N  V  I  E  R
     W  A  C  H  T  Z  E  H  N  R  S       
     B  S  E  C  H  S  F  M  U  H  R       
  *                                   *    
*/

const uint8_t show_es[] PROGMEM = {0x00, 0x01};
const uint8_t show_ist[] PROGMEM = {0x03, 0x04, 0x05};
const uint8_t show_vor[] PROGMEM = {0x36, 0x37, 0x38};
const uint8_t show_nach[] PROGMEM = {0x32, 0x33, 0x34, 0x35};
const uint8_t show_uhr[] PROGMEM = {0x98, 0x99, 0x9A};
const uint8_t show_fuenf[] PROGMEM = {0x07, 0x08, 0x09, 0x0A};
const uint8_t show_zehn[] PROGMEM = {0x10, 0x11, 0x12, 0x13};
const uint8_t show_viertel[] PROGMEM = {0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A};
const uint8_t show_zwanzig[] PROGMEM = {0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A};
const uint8_t show_halb[] PROGMEM = {0x40, 0x41, 0x42, 0x43};
const uint8_t show_hour_ein[] PROGMEM = {0x52, 0x53, 0x54};
const uint8_t show_hour_eins[] PROGMEM = {0x52, 0x53, 0x54, 0x55};
const uint8_t show_hour_zwei[] PROGMEM = {0x50, 0x51, 0x52, 0x53};
const uint8_t show_hour_drei[] PROGMEM = {0x61, 0x62, 0x63, 0x64};
const uint8_t show_hour_vier[] PROGMEM = {0x77, 0x78, 0x79, 0x7A};
const uint8_t show_hour_fuenf[] PROGMEM = {0x67, 0x68, 0x69, 0x6A};
const uint8_t show_hour_sechs[] PROGMEM = {0x91, 0x92, 0x93, 0x94, 0x95};
const uint8_t show_hour_sieben[] PROGMEM = {0x55, 0x56, 0x57, 0x58, 0x59, 0x5A};
const uint8_t show_hour_acht[] PROGMEM = {0x81, 0x82, 0x83, 0x84};
const uint8_t show_hour_neun[] PROGMEM = {0x73, 0x74, 0x75, 0x76};
const uint8_t show_hour_zehn[] PROGMEM = {0x85, 0x86, 0x87, 0x88};
const uint8_t show_hour_elf[] PROGMEM = {0x70, 0x71, 0x72};
const uint8_t show_hour_zwoelf[] PROGMEM = {0x45, 0x46, 0x47, 0x48, 0x49};



const uint8_t led_map_matrix[10][11] PROGMEM = {
  {0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x44, 0x43, 0x42, 0x41, 0x40},
  {0x07, 0x06, 0x05, 0x04, 0x03, 0x08, 0x4A, 0x49, 0x48, 0x46, 0x47},
  {0x00, 0x01, 0x02, 0x1E, 0x1D, 0x1C, 0x4D, 0x4E, 0x52, 0x51, 0x50},
  {0x17, 0x16, 0x18, 0x19, 0x1A, 0x1B, 0x53, 0x54, 0x55, 0x56, 0x57},
  {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E},
  {0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x64, 0x63, 0x62, 0x61, 0x60},
  {0x27, 0x26, 0x25, 0x24, 0x23, 0x28, 0x6A, 0x69, 0x68, 0x66, 0x67},
  {0x20, 0x21, 0x22, 0x3E, 0x3D, 0x3C, 0x6D, 0x6E, 0x72, 0x71, 0x70},
  {0x37, 0x36, 0x38, 0x39, 0x3A, 0x3B, 0x73, 0x74, 0x75, 0x76, 0x77},
  {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E}
};

const uint8_t led_map_dots[] PROGMEM = {
  0x0F, 0x4F, 0x7F, 0x3F
};

const uint8_t log_pwm[32] PROGMEM =
{
    0, 1, 2, 2, 2, 3, 3, 4, 5, 6, 7, 8, 10, 11, 13, 16, 19, 23,
    27, 32, 38, 45, 54, 64, 76, 91, 108, 128, 152, 181, 215, 255
};

struct Display {
  uint8_t led_matrix[10][11];
  uint8_t led_dots[4];
  uint8_t led_pwm0;
  uint8_t led_pwm1;
} currentDisplay;

struct Display fadeOut(struct Display currDisp, struct Display newDisp) {
  Display resultDisp = currDisp;
  for(uint8_t row = 0; row < 10; row++){
    for(uint8_t col = 0; col < 11; col++){
      if(currDisp.led_matrix[row][col] == PWM0 && newDisp.led_matrix[row][col] == OFF)
        resultDisp.led_matrix[row][col] = PWM1;
    }
  }
  for(uint8_t dot = 0; dot < 4; dot++) {
    if(currDisp.led_dots[dot] == PWM0 && newDisp.led_dots[dot] == OFF)
        resultDisp.led_dots[dot] = PWM1;
  }
  return resultDisp;
}

struct Display fadeIn(struct Display currDisp, struct Display newDisp) {
  Display resultDisp = currDisp;
  for(uint8_t row = 0; row < 10; row++){
    for(uint8_t col = 0; col < 11; col++){
      if(currDisp.led_matrix[row][col] == PWM0 && newDisp.led_matrix[row][col] == OFF)
        resultDisp.led_matrix[row][col] = OFF;
      if(currDisp.led_matrix[row][col] == OFF && newDisp.led_matrix[row][col] == PWM0)
        resultDisp.led_matrix[row][col] = PWM1;
    }
  }
  for(uint8_t dot = 0; dot < 4; dot++) {
    if(currDisp.led_dots[dot] == PWM0 && newDisp.led_dots[dot] == OFF)
        resultDisp.led_dots[dot] = OFF;
    if(currDisp.led_dots[dot] == OFF && newDisp.led_dots[dot] == PWM0)
        resultDisp.led_dots[dot] = PWM1;
  }
  return resultDisp;
}

boolean needsFading(struct Display currDisp, struct Display newDisp) {
  for(uint8_t row = 0; row < 10; row++){
    for(uint8_t col = 0; col < 11; col++){
      if(currDisp.led_matrix[row][col] != newDisp.led_matrix[row][col])
        return true;
    }
  }
  for(uint8_t dot = 0; dot < 4; dot++) {
    if(currDisp.led_dots[dot] != newDisp.led_dots[dot])
        return true;
  }
  return false;
}

void wordClockDisplay(struct Display disp){
  uint8_t state[8][4] = {0}; 
  
  for(uint8_t row = 0; row < 10; row++){
    for(uint8_t col = 0; col < 11; col++){
      uint8_t mapping = pgm_read_byte(&led_map_matrix[row][col]);
      uint8_t ic = (mapping & 0b01110000) >> 4;
      uint8_t led = mapping & 0b00001111;
      
      uint8_t reg = led / 4;
      uint8_t regPos = led % 4;
      
      state[ic][reg] &= ~(0b00000011 << 2*regPos);
      state[ic][reg] |= (disp.led_matrix[row][col] << 2*regPos);
    }
  }
  
  for(uint8_t dot = 0; dot < 4; dot++){
    uint8_t mapping = pgm_read_byte(&led_map_dots[dot]);
    uint8_t ic = (mapping & 0b01110000) >> 4;
    uint8_t led = mapping & 0b00001111;
    
    uint8_t reg = led / 4;
    uint8_t regPos = led % 4;
 
    state[ic][reg] &= ~(0b00000011 << 2*regPos);
    state[ic][reg] |= (disp.led_dots[dot] << 2*regPos);
  }
  
  uint8_t ls[] = { 0b00000110, 0b00000111, 0b00001000, 0b00001001 };

  for(uint8_t ic = 0; ic < 8; ic++){
    uint8_t address = 0b01100000 ^ ic;
    for(uint8_t reg = 0; reg < 4; reg++){
      Wire.beginTransmission(address);
      Wire.write(ls[reg]);
      Wire.write(state[ic][reg]);
      Wire.endTransmission();
    }
  }
}

void wordClockInit() {
  Wire.begin();
  delay(200);
  Display disp;
  
  memset(disp.led_matrix, OFF, sizeof(disp.led_matrix[0][0]) * 11 * 10);
  memset(disp.led_dots, OFF, sizeof(disp.led_dots[0]) * 4);
  wordClockDisplay(disp);
  //wordClockBrightness = eeprom_read_byte((uint8_t *) EE_WC_BR); eeprom_busy_wait();
  setPwm(wordClockBrightness, PWM0);
}


void wordClockSetBrightness(uint8_t brightness){
  //wordClockBrightness = brightness;
  //setPwm(wordClockBrightness, 0x00);
}

void wordClockSaveBrightness(){
  //eeprom_write_byte((uint8_t *)  EE_WC_BR, wordClockBrightness);  eeprom_busy_wait();
}




void setPwm (uint8_t brightness, uint8_t pwmNo) {
  uint8_t addressMask = 0b01100000;
  
  uint8_t pwmReg;
  if(pwmNo == PWM0)
    pwmReg = 0b00000011;
  else if(pwmNo == PWM1)
    pwmReg = 0b00000101;
  
  for (uint8_t i=0; i<8; i++) {
    uint8_t address = addressMask ^ i;
    
    Wire.beginTransmission(address);
    Wire.write(pwmReg);
    Wire.write(255-brightness);
    Wire.endTransmission();
  }
}

void wordClockDisplayTime(time_t time) {
  struct Display timeDisp;
  
  memset(timeDisp.led_matrix, OFF, sizeof(timeDisp.led_matrix[0][0]) * 11 * 10);
  memset(timeDisp.led_dots, OFF, sizeof(timeDisp.led_dots[0]) * 4);
  
  uint8_t minutes = minute(time);
  uint8_t hours = hour(time);
  if(minutes >= 25) hours++;
  hours = hours % 12;
  uint8_t dots = minutes % 5;
  minutes -= dots;
  
  
  setLedDisplayMatrix(&timeDisp, PWM0, show_es, sizeof(show_es));
  setLedDisplayMatrix(&timeDisp, PWM0, show_ist, sizeof(show_ist));
  
  switch (hours) {
    case 0: setLedDisplayMatrix(&timeDisp, PWM0, show_hour_zwoelf, sizeof(show_hour_zwoelf)); break;
    case 1: {
      if(minutes != 0) setLedDisplayMatrix(&timeDisp, PWM0, show_hour_eins, sizeof(show_hour_eins));
      else setLedDisplayMatrix(&timeDisp, PWM0, show_hour_ein, sizeof(show_hour_ein));
    } break;
    case 2: setLedDisplayMatrix(&timeDisp, PWM0, show_hour_zwei, sizeof(show_hour_zwei)); break;
    case 3: setLedDisplayMatrix(&timeDisp, PWM0, show_hour_drei, sizeof(show_hour_drei)); break;
    case 4: setLedDisplayMatrix(&timeDisp, PWM0, show_hour_vier, sizeof(show_hour_vier)); break;
    case 5: setLedDisplayMatrix(&timeDisp, PWM0, show_hour_fuenf, sizeof(show_hour_fuenf)); break;
    case 6: setLedDisplayMatrix(&timeDisp, PWM0, show_hour_sechs, sizeof(show_hour_sechs)); break;
    case 7: setLedDisplayMatrix(&timeDisp, PWM0, show_hour_sieben, sizeof(show_hour_sieben)); break; 
    case 8: setLedDisplayMatrix(&timeDisp, PWM0, show_hour_acht, sizeof(show_hour_acht)); break;
    case 9: setLedDisplayMatrix(&timeDisp, PWM0, show_hour_neun, sizeof(show_hour_neun)); break;
    case 10: setLedDisplayMatrix(&timeDisp, PWM0, show_hour_zehn, sizeof(show_hour_zehn)); break;
    case 11: setLedDisplayMatrix(&timeDisp, PWM0, show_hour_elf, sizeof(show_hour_elf)); break;
    break;
  }
  
  switch (minutes) {
    case 0: setLedDisplayMatrix(&timeDisp, PWM0, show_uhr, sizeof(show_uhr)); break;
    case 5: {
      setLedDisplayMatrix(&timeDisp, PWM0, show_fuenf, sizeof(show_fuenf));
      setLedDisplayMatrix(&timeDisp, PWM0, show_nach, sizeof(show_nach));
      break;
    }
    case 10: {
      setLedDisplayMatrix(&timeDisp, PWM0, show_zehn, sizeof(show_zehn));
      setLedDisplayMatrix(&timeDisp, PWM0, show_nach, sizeof(show_nach));
      break;
    }
    case 15: {
      setLedDisplayMatrix(&timeDisp, PWM0, show_viertel, sizeof(show_viertel));
      setLedDisplayMatrix(&timeDisp, PWM0, show_nach, sizeof(show_nach));
      break;
    }
    case 20: {
      setLedDisplayMatrix(&timeDisp, PWM0, show_zwanzig, sizeof(show_zwanzig));
      setLedDisplayMatrix(&timeDisp, PWM0, show_nach, sizeof(show_nach));
      break;
    }
    case 25: {
      setLedDisplayMatrix(&timeDisp, PWM0, show_fuenf, sizeof(show_fuenf));
      setLedDisplayMatrix(&timeDisp, PWM0, show_vor, sizeof(show_vor));
      setLedDisplayMatrix(&timeDisp, PWM0, show_halb, sizeof(show_halb));
      break;
    }
    case 30: setLedDisplayMatrix(&timeDisp, PWM0, show_halb, sizeof(show_halb)); break;
    case 35: {
      setLedDisplayMatrix(&timeDisp, PWM0, show_fuenf, sizeof(show_fuenf));
      setLedDisplayMatrix(&timeDisp, PWM0, show_nach, sizeof(show_nach));
      setLedDisplayMatrix(&timeDisp, PWM0, show_halb, sizeof(show_halb));
      break;
    }
    case 40: {
      setLedDisplayMatrix(&timeDisp, PWM0, show_zwanzig, sizeof(show_zwanzig));
      setLedDisplayMatrix(&timeDisp, PWM0, show_vor, sizeof(show_vor));
      break;
    }
    case 45: {
      setLedDisplayMatrix(&timeDisp, PWM0, show_viertel, sizeof(show_viertel));
      setLedDisplayMatrix(&timeDisp, PWM0, show_vor, sizeof(show_vor));
      break;
    }
    case 50: {
      setLedDisplayMatrix(&timeDisp, PWM0, show_zehn, sizeof(show_zehn));
      setLedDisplayMatrix(&timeDisp, PWM0, show_vor, sizeof(show_vor));
      break;
    }
    case 55: {
      setLedDisplayMatrix(&timeDisp, PWM0, show_fuenf, sizeof(show_fuenf));
      setLedDisplayMatrix(&timeDisp, PWM0, show_vor, sizeof(show_vor));
      break;
    }
  }
  
  switch (dots) {
    case 0: memset(timeDisp.led_dots, OFF, sizeof(timeDisp.led_dots[0]) * 4); break;
    case 4: timeDisp.led_dots[3] = PWM0;
    case 3: timeDisp.led_dots[2] = PWM0;
    case 2: timeDisp.led_dots[1] = PWM0;
    case 1: timeDisp.led_dots[0] = PWM0; break;
  }
  
  
  boolean fading = needsFading(currentDisplay, timeDisp);
  
  if(fading) {
    Display fadeDisp = fadeOut(currentDisplay, timeDisp);
    wordClockDisplay(fadeDisp);

    for(uint8_t i=32;i>0;i--) {
      if(pgm_read_byte(&log_pwm[i-1]) > wordClockBrightness) {
        continue;
      }
      setPwm(pgm_read_byte(&log_pwm[i-1]), PWM1);
      delay(25);
    }

    fadeDisp = fadeIn(currentDisplay, timeDisp);
    wordClockDisplay(fadeDisp);

    for(uint8_t i=0;i<=31;i++) {
      if(pgm_read_byte(&log_pwm[i]) > wordClockBrightness) {
        setPwm(wordClockBrightness, PWM1);
        break;
      }
      setPwm(pgm_read_byte(&log_pwm[i]), PWM1);
      delay(25);
    }
  }
  
  wordClockDisplay(timeDisp);
  currentDisplay = timeDisp;
}

//generic led manipulation 
void setLedDisplayMatrix(struct Display * disp, uint8_t value, const uint8_t mani[], uint8_t manisize){
  for(uint8_t i = 0; i < manisize; i++){
    uint8_t led = pgm_read_byte(&mani[i]);
    uint8_t row = (led & 0b11110000) >> 4;
    uint8_t col = led & 0b00001111;
    disp->led_matrix[row][col] = value;
  }  
}

void testWords() {
  uint16_t i = 0;
  while(1) {
    wordClockDisplayTime(i);
    i++;
    delay(10);
  }
}
