#include "Arduino.h"
#include <Wire.h>
#include <MI0283QT9.h>
#define ADDRESS 0x52

//Declare only one display !
MI0283QT9 lcd;  //MI0283QT9 Adapter v1

uint8_t curr_fact=0;
  int analogX;
  int analogY;
  int accelX;
  int accelY;
  int accelZ;
  int zButton;
  int cButton;
  char tmp[128];
  static uint16_t last_x=0, last_y=0;
  static uint16_t current_x=160, current_y=115;
  volatile uint8_t timer2_counter; 
  int xLocation = 160;
  int yLocation = 115;
  
//Data vesturen via i2c
void AN_sendByte(byte data, byte location)
{
  Wire.beginTransmission(ADDRESS);

  Wire.write(location);
  Wire.write(data);

  Wire.endTransmission();

  _delay_ms(10);
}

//Begin met het uitlezen van de nunchuk
void ANinit()
{
  Wire.begin();

  AN_sendByte(0x55, 0xF0);
  AN_sendByte(0x00, 0xFB);

  ANupdate();
}


//Herlaad de waarden die de nunchuk doorgeeft.
void ANupdate()
{
  int count = 0;
  int values[6];

  Wire.requestFrom(ADDRESS, 6);

  while(Wire.available())
  {
    values[count] = Wire.read();
    count++;
  }

  analogX = values[0];
  analogY = values[1];
  accelX = (values[2] << 2) | ((values[5] >> 2) & 3);
  accelY = (values[3] << 2) | ((values[5] >> 4) & 3);
  accelZ = (values[4] << 2) | ((values[5] >> 6) & 3);
  zButton = !((values[5] >> 0) & 1);
  cButton = !((values[5] >> 1) & 1);

  AN_sendByte(0x00, 0x00);
}

void timer2_init(){
  TCNT2   = 0;
  TCCR2B |= (1 << WGM21); // Configure timer 2 for CTC mode
  TCCR2B |= (1 << CS22); // Start timer at Fcpu/64
  TIMSK2 |= (1 << OCIE2A); // Enable CTC interrupt
  OCR2A   = 120; // Set CTC compare value with a prescaler of 64
  sei();
}
int main(void){
  init();
  //init display
  lcd.begin(); //spi-clk=SPI_CLOCK_DIV4
  
  //clear screen
  lcd.fillScreen(RGB(255,255,255));
  
  //Setup communication with nunchuk
  Serial.begin(9600);
  ANinit();

  timer2_init();

  

  while(1){
    ANupdate();
    sprintf(tmp, "X:%03i Y:%03i", current_x, current_y);
    
    if((last_x != current_x) || (last_y != current_y)){
       lcd.drawText(50, 2, tmp, RGB(0,0,0), RGB(255,255,255), 1);
       lcd.drawCircle(last_x, last_y, 3, RGB(255,255,255));
       lcd.drawCircle(current_x, current_y, 3, RGB(0,0,0));
       last_x = current_x;
       last_y = current_y;
    }

    if(zButton != 0){
        lcd.fillScreen(RGB(255,255,255));
      }
  }
  
}

ISR(TIMER2_COMPA_vect)        // interrupt service routine 
{
  TCNT2 = timer2_counter;   // preload timer
  if(analogX>186 && current_x<300){
    current_x+=1;
    //Serial.println("RECHTS");
  } else if(analogX<82 && current_x>0){
    current_x-=1;
    //Serial.println("LINKS");
  }

  if(analogY<85 && current_y<230){
    current_y+=1;
    //Serial.println("DOWN");
  }else if(analogY>174 && current_y>0){
      current_y-=1;
      //Serial.println("UP");
  }
}
