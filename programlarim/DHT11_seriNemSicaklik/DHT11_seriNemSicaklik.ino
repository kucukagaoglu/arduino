// Arduinoturkiye.com DHT11 Test Programı
// Yazar: Joseph Dattilo (Virtuabotix LLC) - Version 0.4.5 (11/11/11)
// Düzenleme: İsmail BUÇGÜN

/*
D2 pinine DHT11 'in data ucu bağlanır,
2 saniye de bir seri kanaldan siıvaklik ve nem bilgisi yollar


*/

#include <dht11.h> // dht11 kütüphanesini ekliyoruz.
#define DHT11PIN 2 // DHT11PIN olarak Dijital 2'yi belirliyoruz.

dht11 DHT11;

void setup()
{
  Serial.begin(9600); // Seri iletişimi başlatıyoruz.

}

int i=0;
unsigned long int onceki_sure=0;
int kontrol;
void loop()
{
  // Bir satır boşluk bırakıyoruz serial monitörde.
//  Serial.println("\n");
  // Sensörün okunup okunmadığını konrol ediyoruz. 
  // chk 0 ise sorunsuz okunuyor demektir. Sorun yaşarsanız
  // chk değerini serial monitörde yazdırıp kontrol edebilirsiniz.
  int chk = DHT11.read(DHT11PIN);
  
    while(Serial.available()==0) 
  { // Wait for User to Input Data  
  }
  
  kontrol=Serial.parseInt(); 
  
  if(kontrol==55)
  {
  Serial.print("Nem:");
  Serial.print((float)DHT11.humidity, 2);
  Serial.print(" Sic:");
  Serial.println((float)DHT11.temperature, 2);
  }
  
  // 2 saniye bekliyoruz. 2 saniyede bir veriler ekrana yazdırılacak.
  //delay(2000);
  
}

