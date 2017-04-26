// AH Counter Simulation

#include <LiquidCrystal.h>
#include <mcp_can.h>
#include <SPI.h>
unsigned char Flag_Recv = 0;
unsigned char len = 0;
byte buf1[8];
int rxId;

// Adjustable parameters
const float charge_correct = 0.9; // reverse current charing efficiency factor
const int fuel_tank = 30; // total avail capacity in Ah



LiquidCrystal lcd(2, 9, 4, 5, 6, 7);
byte batt1[8] = { // These two together make a nice battery symbol
B01100,
B11111,
B10000,
B10010,
B10111,
B10010,
B11111,
};
byte batt2[8] = {
B00110,
B11111,
B00001,
B00001,
B11101,
B00001,
B11111,
};

int battVolt = 0;
float kwatts = 0;
int sample = 0;
int time = 0;
float raw_amps = 0;
float amps = 0;
float totCharge = 0;
float avg_amps = 0;
float ampSec = 0;
float ampHour = 0;
float kwHour = 0;
float fuel_level = 0;

void setup() {
  
lcd.begin(20, 4);
lcd.createChar(0, batt1);
lcd.createChar(1, batt2);

///*
lcd.setCursor(0, 0);
lcd.print("WWW.EVBMW.COM");
delay(2000);
lcd.setCursor(0, 1);
lcd.print("Battery Monitoring");
lcd.setCursor(0, 2);
lcd.print("System");
delay(2000);
//*/

      Serial.begin(9600);
  if(CAN.begin(CAN_500KBPS) ==CAN_OK) Serial.print("can init ok!!\r\n");
  else Serial.print("Can init fail!!\r\n");
    attachInterrupt(1, MCP2515_ISR, FALLING);     // start interrupt



}

void MCP2515_ISR()
{
    Flag_Recv = 1;
}


void loop() {
      if(Flag_Recv)                           // check if get data
    {
      Flag_Recv = 0;                        // clear flag
      rxId = CAN.getCanId();                    // Get message ID
    //  Serial.println(rxId,HEX);
      if(rxId = 0x101){
      CAN.readMsgBuf(&len, buf1);            // read data,  len: data length, buf: data buf

  //    Serial.println("CAN_BUS GET DATA!");
   //   Serial.print("data len = ");
   //   Serial.println(len);
      
  //    for(int i = 0; i<len; i++)            // print the data
   //   {
     //   Serial.print(buf[i]);Serial.print("\t");
   //   }
   //   Serial.println();
    }
    }



battVolt = word(buf1[0],buf1[1]);  
raw_amps = word(buf1[2],buf1[3]);
int tempc = word(buf1[4],buf1[5]);
if(raw_amps < 0) {
amps = raw_amps * charge_correct; // Charging efficiency factor
} else {
amps = raw_amps;
}
kwatts = raw_amps * battVolt / 1000; //kW for display only
sample = sample + 1;
time = millis() / 1000;
totCharge = totCharge + amps;
avg_amps = totCharge / sample;
ampSec = avg_amps * time;
ampHour = ampSec / 3600;
kwHour = battVolt * ampHour / 1000;
fuel_level = (fuel_tank - ampHour) / fuel_tank * 100;

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Volt: ");
lcd.print(battVolt, 1);
lcd.setCursor(11, 0);
lcd.print("Amp: ");
lcd.print(raw_amps, 0);
lcd.setCursor(0, 1);
lcd.print("kW: ");
lcd.print(kwatts, 1);
lcd.setCursor(12, 1);
//lcd.print("Sec: ");
//lcd.print(time);
lcd.setCursor(0, 2);
lcd.print("Ah: ");
lcd.print(ampHour, 1);
lcd.setCursor(10, 1);
lcd.print("kWh: ");
lcd.print(kwHour, 2);
lcd.setCursor(10, 2);
lcd.print("Temp: ");
lcd.print(tempc);
lcd.setCursor(4, 3);
lcd.write(byte(0));
lcd.write(byte(1));
lcd.print(" ");
lcd.print(fuel_level, 1);
lcd.print((char)37);
delay(200);
} 
