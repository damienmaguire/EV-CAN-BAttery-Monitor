

#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <mcp_can.h>
#include <SPI.h>








int Volts = 0;
int Amps = 0;
int Temp = 0;
int AH = 0;
int bah = 0;
int sample = 0;
int time = 0;
float totCharge = 0;
float avg_amps = 0;
float ampSec = 0;
float ampHour = 0;

        


Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if(CAN.begin(CAN_500KBPS) ==CAN_OK) Serial.print("can init ok!!\r\n");
  else Serial.print("Can init fail!!\r\n");
  
    // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
 //  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
   ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
 ads.begin(); 

}



void loop() {

Volts = readBattVolts();  //read the battery voltage from analog 0

Amps = readBattAmps();   //read the battery current from analog 1

Temp = readBattTemp();   //read the battery temp from analog 2

AH = calcAH();         //calculate amp hours current in the battery


//sendBattData();   // send the data to bluetooth every 500ms
sendCanData();  //send battery data on can bus
delay(500);

 

  }

  



int readBattVolts(){
  int16_t bv;
 bv = ads.readADC_Differential_2_3();
  
 // int bv = as100.analogReadSmooth(0);
   // int bv = analogRead(0);
   bv = map(bv,-87,1895,0,30);
    return bv;
  
}


  



int readBattAmps(){
  int16_t ba;
ba = ads.readADC_Differential_0_1();

  
//  int ba = as101.analogReadSmooth(1);
  //int ba = analogRead(1);
  ba = map(ba,13,2517,0,50);
    return ba;
}


  


int readBattTemp(){

  int bt = analogRead(2);
    return bt;
  
}

  


int calcAH(){


  
sample = sample + 1;
time = millis() / 1000;
totCharge = totCharge + Amps;
avg_amps = totCharge / sample;
ampSec = avg_amps * time;
ampHour = ampSec / 3600;
//kwHour = battVolt * ampHour / 1000;


  return ampHour;

}


  


void sendBattData(){

 Serial.print("Batt Volts:"); 
 Serial.println(Volts);
 Serial.print("Batt Temp:"); 
 Serial.println(Temp);
 Serial.print("Batt Current:"); 
 Serial.println(Amps);
Serial.print("Batt Amp/Hrs:");
 Serial.println(AH);
  
}

void sendCanData(){

byte vl=lowByte(Volts);
byte vh=highByte(Volts);
byte al=lowByte(Amps);
byte ah=highByte(Amps);
byte tl=lowByte(Temp);
byte th=highByte(Temp);
byte ahl=lowByte(AH);
byte ahh=highByte(AH);

byte data[8] = {vh, vl, ah, al, th, tl, ahl, ahh};

  // send data:  id = 0x100, standrad flame, data len = 8, stmp: data buf
  CAN.sendMsgBuf(0x101, 0, 8, data);
  
}
  


