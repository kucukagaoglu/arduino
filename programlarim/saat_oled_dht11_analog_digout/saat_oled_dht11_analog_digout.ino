

#include <dht11.h> // dht11 kütüphanesini ekliyoruz.
#define DHT11PIN 2 // DHT11PIN olarak Dijital 2'yi belirliyoruz.
dht11 DHT11;

#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);	// Display which does not send AC

#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68

//GLOBALLER
int saat,dakika,saniye;

//ADC için olanlar
// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

const int motor = 8; // nem yetersiz ise motoru çalıştır.


int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)



byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}


void draw(int sensor,int saat,int dakika,int saniye,int int_isi,int int_nem) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  //int aInt = 368;
    char str_isi[15],str[10],str_nem[10], str2[10],str_saat[10];
    char str3[20],str_dk[10],str_sn[10];
    char str_sensor[10],str4[10];
    //sicaklik satırın oluşturmakta
    sprintf(str_isi, "%d", int_isi);
    strcpy(str, "Isi: ");
    strcat(str,str_isi);
    strcat(str," C.");
    
    //nem satırını ouşturmakta
    sprintf(str_nem, "%d", int_nem);
    strcpy(str2, "Nem: ");
    strcat(str2,str_nem);
    strcat(str2," %RH");
    
    //tarih saat satırı
    sprintf(str_saat, "%02d", saat);
    sprintf(str_dk, "%02d", dakika);
   sprintf(str_sn, "%02d", saniye);
 
 Serial.println("");
 Serial.print(dakika);
 Serial.print("-");
 Serial.print(str_dk);
    
    strcpy(str3,"[");
    strcat(str3,str_saat);
    strcat(str3,":");
    strcat(str3,str_dk);
//    strcat(str3,":");
//    strcat(str3,str_sn);
    strcat(str3,"]");

    //analog sensör bilgileri için
    
    sprintf(str_sensor, "%d", sensor);
    strcpy(str4,"Toprk:");
    strcat(str4,str_sensor);
    if(sensor>=700) strcat(str4,"-Kuru!");
   else  strcat(str4,"-Nemli!");
      
      
      
    //hepsini çiz  
    u8g.drawStr( 0, 11, str3);
    u8g.drawStr( 0, 22, str2);
//  u8g.drawStr( 0, 22, "KUCUKAGAOGLU...");

    u8g.drawStr( 0, 33, str);
    u8g.drawStr( 0, 44, str4);
    
//   Serial.println(str3);
//   Serial.println(str2);
//   Serial.println(str);
//   Serial.println(str4);

}

void setup(void) {
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  
  Serial.begin(9600); // Seri iletişimi başlatıyoruz.

  
  //saat kısmı
    Wire.begin();
    //setDS3231time(0,54,0,1,24,8,15);
    
    //motor pini output de
    pinMode(motor, OUTPUT); 
  
}

//saat
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  //Serial.print(hour, DEC);
  saat=(int) hour;
  Serial.print(saat);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
 //   Serial.print("0");
  }
  //Serial.print(minute, DEC);
  dakika=(int) minute;
   Serial.print(dakika);
   Serial.print(":");
  if (second<10)
  {
   // Serial.print("0");
  }
 // Serial.print(second, DEC);
  saniye=(int) second;
  Serial.print(saniye);
  
  Serial.println(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
 // Serial.print(" Day of week: ");
 
 /* switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }*/
}
//-saat

void loop(void) {
  
  int chk = DHT11.read(DHT11PIN);
  float sicaklik=(float)DHT11.temperature;
  float nem=(float)DHT11.humidity;
  
   displayTime();
   

   
  Serial.println("");
  Serial.print("Isi : ");
//  Serial.println(sicaklik, 2);
  Serial.print((int)sicaklik);
  
  Serial.println("");
  Serial.print("Nem (%): ");
//  Serial.println((float)DHT11.humidity, 2);
  Serial.print((int)nem);
  Serial.println("");
  Serial.println("---------------------");

  delay(900);
  // picture loop
  u8g.firstPage();  
  do {
    draw(sensorValue,saat,dakika,saniye,(int)sicaklik,(int)nem);
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(50);
  
  //ADC için
    // read the analog in value:
  sensorValue = analogRead(analogInPin);    
  if(sensorValue<=700)  digitalWrite(motor, HIGH);
  else digitalWrite(motor, LOW);

  
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);  
  // change the analog out value:
  //analogWrite(analogOutPin, outputValue);           

  // print the results to the serial monitor:
 /*
  Serial.print("sensor = " );                       
  Serial.print(sensorValue);      
  Serial.print("\t output = ");      
  Serial.println(outputValue);   
*/
  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
 // delay(2); 
  //-ADC için
  
  
}

