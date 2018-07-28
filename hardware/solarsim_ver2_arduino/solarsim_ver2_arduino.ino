// SolarSim
// screening by using console
// screen /dev/tty.usbserial-A400hvfT 9600
// set heater thermostat to 60C for safety

//------------------
#include <OneWire.h>
// OneWire by Jim Studt, Tom Pollard, Robin James,
// Glenn Trewitt, Jason Dangel, Guillermo Lovato,
// Paul Stoffregen, Scott Roberts, Bertrik Sikken,
// Mark Tillotson, Ken Butcher, Roger Clark, Love Nystrom
//------------------

//------------------
#include <SD.h>
// sdfatlib by William Greiman
#define SD_CARD_CD_DIO 4
//------------------
 
#include <Wire.h> //built-In Arduino

//------------------
#include <LiquidCrystal_I2C.h>
// by joaopedrosgs and fdebrabander
//------------------

LiquidCrystal_I2C lcd(0x27,16,2);
boolean kisirdon=true;
File SDFileData;
File SDFileDataWrite;
char k,kk;
int b;
float bb,settt;
long nekadar=360000;//milisec interval value
long sayac=0;
float tmp2temp;
float tmp2;
OneWire  ow(3);// digital pin 3 for temp
byte sensor[8] = {0x10, 0xE5, 0xD9, 0xD7, 0x01, 0x08, 0x00, 0xCE};

//------------------to control relay
int mindelay = 100;
int minrun = 3000;
int maxdelay = 8000;
int maxrun = 150;//try old value 200
int maxdiff = 20;//try old value 10
float bekle,calis,fark,curr,sett;
//------------------------

void setup() 
{                
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(7, OUTPUT);
  lcd.init(); 
  lcd.backlight();
  lcd.setCursor(0,0); 
  lcd.print("Solar Simulator");
  lcd.setCursor(0,1); 
  lcd.print("uozyilmaz v1");
  delay(1000);
}

void loop() {
  if (!SD.begin(SD_CARD_CD_DIO)) 
    {
      //Serial.println("SD card problem");
      //while(1);
    }
    else
    {
      Serial.println("");
      Serial.println("SD OK");
      lcd.setCursor(0,1);      
      lcd.print("SD OK          ");
      delay(1000);
    }
if(SD.exists("data.txt")) 
{
Serial.println("");
Serial.println("data.txt exists, attempting to read file..."); 
lcd.setCursor(0,1);        
lcd.print("DATA.TXT found   ");
delay(1000);
SDFileData = SD.open("log.txt");
int sayilan=0;
while (SDFileData.available()) 
  {
  k=SDFileData.read();
  sayilan=sayilan+1;
  }
SDFileData.close();
sayilan=sayilan/4;
Serial.print("Record count: ");
Serial.println(sayilan);
lcd.setCursor(0,0);         
  lcd.print("Old record      ");
  lcd.setCursor(0,1); 
  lcd.print("                ");
  lcd.setCursor(0,1); 
  lcd.print(sayilan);
delay(1000);
lcd.setCursor(0,1);       
  lcd.print("current     ");
  lcd.setCursor(0,0); 
  lcd.print("Expected ");
SDFileData = SD.open("data.txt");
for (int iii=1; iii <= sayilan*2; iii++)
    {
      k=SDFileData.read();
    } 
SDFileDataWrite = SD.open("log.txt", FILE_WRITE);
while (SDFileData.available()) 
  {
  k=SDFileData.read();
  b=k+0;
  kk=SDFileData.read();
  bb=kk+0;
  settt=b+(bb/100);
  sett=settt;//sd den okudugu ataniyor
  Serial.print(">>>>>>>>>>>New Set: "); 
  Serial.println (sett);
  while (millis()<=nekadar*sayac)
    {
    tmp2temp = getTemperature(sensor);
    curr=tmp2;
      if (curr >= (sett - maxdiff))
      {
      fark = sett - curr;
      bekle = ((maxdelay - mindelay) / maxdiff) * (maxdiff - fark);
      if (bekle < mindelay)
        {bekle = 100;}
      calis = ((minrun - maxrun) / maxdiff) * fark;
      if (calis < maxrun)
        {calis = maxrun;}
      }
      if (curr < sett - maxdiff)
        {calis = minrun; bekle = 0;}
      if (curr >= sett)
        {calis = 0; bekle = maxdelay;}
      Serial.print("Pulse: ");
      if (calis!=0)
      {
        Serial.print("ON ");
        lcd.setCursor(15,0); 
        lcd.print("*");
      }
      else
      {
      Serial.print("OFF");
      lcd.setCursor(15,0); 
      lcd.print(" ");
      }
      Serial.print(" , Current: "); 
      Serial.print (curr);
      lcd.setCursor(8,1); 
      lcd.print(curr);
      lcd.setCursor(8,0); 
      lcd.print(sett);
      Serial.print(" , Set: "); 
      Serial.println (sett);
      if (calis!=0)
      {
        digitalWrite(7, HIGH);  
        delay(calis); 
      }
      if (bekle!=0)
      {
        digitalWrite(7, LOW); 
        delay(bekle);
      }
   }
  char yazilacak;
  int ccc,ddd; 
  ccc=curr;
  ddd=100*(curr-ccc);
  yazilacak=ccc;
  SDFileDataWrite.print(yazilacak);
  yazilacak=ddd;
  SDFileDataWrite.print(yazilacak);
  SDFileDataWrite.print(k);
  SDFileDataWrite.print(kk);
  SDFileDataWrite.flush();
  sayac=sayac+1;
  }

SDFileData.close(); 
SDFileDataWrite.close();
digitalWrite(7, LOW);
lcd.setCursor(0,0);          
lcd.print("END             ");
lcd.setCursor(0,1); 
lcd.print("                ");

while (kisirdon)
{
//END
}
}
}

//calculating temp-----------------------------
float getTemperature(byte* address)
{
  int tr;
  byte data[12];
  ow.reset();
  ow.select(address);
  ow.write(0xBE);
  for (byte i=0;i<9;i++)
  {
    data[i] = ow.read();
  }
  tr = data[0];
  if (data[1] > 0x80){
    tr = !tr + 1;
    tr = tr * -1;
  }
  int cpc = data[7];
  int cr = data[6];
  tr = tr >> 1;
  tmp2=tr - (float)0.25 + (cpc - cr)/(float)cpc;
  ow.reset();
  ow.select(address);
  ow.write(0x44,1);
}
