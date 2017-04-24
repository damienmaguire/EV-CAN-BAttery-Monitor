/*
CAN Display v`1.

  The circuit:
 * LCD RS pin to digital pin 2
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * LCD VSS pin to ground

 */

// include the library code:
#include <LiquidCrystal.h>
#include <mcp_can.h>
#include <SPI.h>

unsigned char Flag_Recv = 0;
unsigned char len = 0;
byte buf[8];
//char str[20];

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 9, 4, 5, 6, 7);

void setup() {
    Serial.begin(9600);
  if(CAN.begin(CAN_500KBPS) ==CAN_OK) Serial.print("can init ok!!\r\n");
  else Serial.print("Can init fail!!\r\n");
    attachInterrupt(1, MCP2515_ISR, FALLING);     // start interrupt
  
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.print("EVBMW Batt Monitor");
  lcd.setCursor(0,1);
  lcd.print("Voltage=");
  lcd.setCursor(0,2);
  lcd.print("Current=");
  lcd.setCursor(0,3);
  lcd.print("AH=");
  lcd.setCursor(8,3);
  lcd.print("Temp=");
  
}

void MCP2515_ISR()
{
    Flag_Recv = 1;
}

void loop() {

      if(Flag_Recv)                           // check if get data
    {
      Flag_Recv = 0;                        // clear flag
      CAN.readMsgBuf(&len, buf);            // read data,  len: data length, buf: data buf
      Serial.println("CAN_BUS GET DATA!");
      Serial.print("data len = ");
      Serial.println(len);
      
      for(int i = 0; i<len; i++)            // print the data
      {
        Serial.print(buf[i]);Serial.print("\t");
      }
      Serial.println();
    }
  lcd.setCursor(8,1);
 int volts = word(buf[0],buf[1]);
  lcd.print(volts);
  lcd.print("Volts");
  lcd.setCursor(8,2);  
 int amps = word(buf[2],buf[3]);
  lcd.print(amps);
    lcd.print("Amps");
    lcd.setCursor(3,3);  
 int ah = word(buf[7],buf[6]);
  lcd.print(ah);
  lcd.setCursor(13,3);  
 int temp = word(buf[4],buf[5]);
  lcd.print(temp);

}

