/*
Bu versiyodna sistem Arduino tarafında set edilen saat
gecikmeleri olan "kontrol peryodu" ile toprak nemini kontrol edecek, eğer nem 
az ise "sulama suresi" saniye sulama motoru çalıştrılacaktır. Tam 1 saat
sonra nem seviyesi yine kontrol edilecek ve bu şekilde devam
edecektir.

- Sürekli sulama ve sürekli kontrol en ilkel şekilde korunmuştur.

- Eğer 3. defa üstüste sulama yeterli nemi sağlayamazsa hata ledi
yanacaktır. Böylece de toprağın kuruması engellenecektir.

- Sistem 9600 baudrate ile seri kanaldan verileri alır ve basar

*/

//#define kontrol_peryodu 10 //3600 bir saat demek
//#define sulama_suresi 10 // 10 saniye boyunca sadece sula

unsigned char kontrol_peryodu; //dakikada bir nemi kontrol et
unsigned char sulama_suresi; // ... saniye boyunca sadece sula


//GLOBALLER

int incomingByte = 0;   // for incoming serial data
unsigned long int sayac=0; // saniyeleri sayacak sayac
unsigned char kontrol_sayac=0;
unsigned suladigi_zaman_sayaci=0;
char str_sensor[10],str4[10],str1[10],str_sayac[10];

//NEM OKUYACAK ADC için A0 kanalı kullanılacak

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to


// MOTORU TETİKLEYEN PİN
const int motor = 8; // nem yetersiz ise motoru çalıştır.


int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

 





void setup(void) {

 
  Serial.begin(9600); // Seri iletişimi başlatıyoruz.
   
    //motor pini output de
    pinMode(motor, OUTPUT); 
    
    //
    
  
}


void seri_yolla()
{
  Serial.println("---------------------");
  Serial.println("SET EDILEN DEGERLER:");
  
  Serial.print("Kontrol Peryodu(dk):");
  Serial.print(kontrol_peryodu);
  Serial.print(" | ");
  //
  Serial.print("Sulama Suresi(sn):");
  Serial.print(sulama_suresi);
  Serial.println("");
  
  //
  
  Serial.print("Toprak Nemi:");
  Serial.print(sensorValue);
  Serial.println("");
  //
  
  Serial.print("Kontrole Kalan Saniye:");
  Serial.print((kontrol_peryodu*60)-sayac);
  Serial.println("");
  //
  Serial.print("Suladigi Sayac:");
  Serial.print(suladigi_zaman_sayaci);
  Serial.println("");
  //

  
  
  Serial.println("////////////////////////");
  
  
}

void sulama_basla()
{
digitalWrite(motor, HIGH);


while(suladigi_zaman_sayaci<sulama_suresi)
  {
  delay(1000);
  suladigi_zaman_sayaci++;
  
  seri_yolla();
  
  }

  sulama_bitir();

  }



void sulama_bitir()
{
digitalWrite(motor, LOW);
suladigi_zaman_sayaci=0;
sayac=0;

}

void nem_ogren()
{
sensorValue = analogRead(analogInPin); 
}

void sulama_kontrol()
{

  if(sayac>=kontrol_peryodu*60) //dakikadan saniyeye cevirdim
  {
    nem_ogren();   
    if(sensorValue<=700) sulama_basla();
  }
//  else sulama_bitir();
  
  
}

void loop(void) {

  
  Serial.println("Merhaba! Sistem basitce; girdiginiz kontrol peryodu");
  Serial.println("kadar surede bir nemi kontrol eder, nem az ise sulama suresi ");
  Serial.println("kadar sulama motorunu calistirir!");
  Serial.println("***********");
  
  
  
  Serial.print("Kontrol Peryodunu Giriniz(dakika):");  Serial.println("");
  
while(kontrol_peryodu<15)
{
  while(Serial.available()==0) 
  { // Wait for User to Input Data  
  }
  kontrol_peryodu=Serial.parseInt();  

                Serial.print("Kontrol Peryodu ");
                Serial.print((int)kontrol_peryodu);
                Serial.println(" dakika girildi!");  
                Serial.println(" __________________");

if(kontrol_peryodu<15) 
{
  Serial.println("HATA!Peryod en az 15 dk. olmali!Tekrar giriniz:");
}
}
/////////////////

  Serial.print("Sulama Suresini Giriniz(saniye):");  Serial.println("");
    
while(sulama_suresi<15)
{    
  while(Serial.available()==0) 
  { // Wait for User to Input Data  
  }
  sulama_suresi=Serial.parseInt();  

                Serial.print("Sulama Suresi ");
                Serial.print((int)sulama_suresi);
                Serial.println(" saniye girildi!");
                Serial.println(" __________________");

if(sulama_suresi<15) 
{
  Serial.println("HATA!Sure en az 15 sn. olmali!Tekrar giriniz:");
}

}
/////////////////


  
  while(Serial.read()!='x')
  
  {
  
  
  
  // rebuild the picture after some delay
  delay(1000);
  sayac=sayac+1;
  nem_ogren();
  sulama_kontrol();
  
  seri_yolla();
 
  

  }
  
  //ÇIKIŞ KOMUTU VERİLDİ, DEĞERLERİ RESETLE!
  
  Serial.println(" **********************");
  Serial.println("Sistem disaridan komutla('x') yeniden baslatildi");
  Serial.println("***********************");
  
  sulama_suresi=0;kontrol_peryodu=0;
  
  

}
