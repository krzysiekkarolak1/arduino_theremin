// Optical Theremin

//Matrix Led Panel Libraries
#include "LedControlMS.h"
LedControl lc=LedControl(5,3,4,1);


//definicje pinów
#define PHONES 9   // headphones connected to digital pin 9
#define PHOTOCELL 0 //photocell analog in pin 0

long val = 0;        //przechowuje wartość z fotorezystora
long maxread = 0;    //maksymalna wartość z kalibracji
long minread = 1000;  // minimalna wartość z kalibracji
double f = 0;         // częstotliwosc dzwieku
double normf = 0;    // normalizowana częstotliwosc 
double logfreq = 0;      // logarytm normalizacji częstotliwości
int ilogf = 0;        // zakrzywiony logarytm
int i = 0;            // zmienna do wykorzystania w pętli
double factor = 0;    // czynnik skalowania do kalibracji
double shift = 0;     // przesuniecie dla kalibracji
long maxfreq = 1048;  // maksymalna zdefiniowana czestotliwosc po kalibracji
long minfreq = 131;   // minimalna zdefiniowana czestotliwosc po kalibracji
int tonecount = 0;
int lcdvolume=100;


//liczby sprawiające, że dźwięk jest barwny
double gap = 1.148698355;  //ratio of consecutive notes (pentatonic)
                             // it's the 5th root of 2
//double gap = 1.059463094;  //ratio of consecutive notes (chromatic)
                              // its the 12th root of 2

void cleanled() {
      for(i=0;i<=7;i++){
      lc.setLed(0,0,i,false);
      for(int j=0;j<=7;j++) {
       lc.setLed(0,j,i,false);
      }
    }
}
                              
void setup()
{
  pinMode(PHONES, OUTPUT);    // ustawia pin cyfrowy jako wyjście
  Serial.begin(9600);
// kalibracja sluzaca wykryciu skali padania swiatla (minread to maxread)
// i dopasowaniu do czestotliowsci (maxfreq, minfreq)
  for (i = 0; i< 500; i++) {  // kalibracja trwa 5 sekund
    val = analogRead(PHOTOCELL);   // odczyt z fotorezystora
    tone(PHONES, val);          // odtwarzaj nieprzetworzony dzwiek
    if (val > maxread) {        // jezeli wartosc wzrosnie, zachowaj najwiekszą
      maxread = val;
    }
    if (val < minread) {        // jezeli wartość spadnie, zachowaj najmniejszą
      minread = val;
    }  
    delay(10);                  // opóźnienie
  } 
  
  //Użycie kalibracji do obliczenia skali i przesunięcia częstotliwości
  factor = (double)(maxfreq - minfreq) / (double)(maxread - minread); // parametr skali
  
  shift = factor * minread - minfreq;  //parametr przesuniecia

  /* Inicjalizacja panelu LED */
  lc.shutdown(0,false);  
  /* Jasnosc diod */
  lc.setIntensity(0,8);
  /* czyszczenie ekranu */
  lc.clearDisplay(0);
}

void loop()
{
  val = analogRead(PHOTOCELL);   // odczyt z fotorezystora 
  Serial.println(val);
 
  f = factor * val - shift;     // przypisuje czestotliwosc do wartosci
                                // pomiedzy minfreq i maxfreq
                                // zgodnie z wynikami kalibracji
  
  normf = f / (double) minfreq;  // Dividing an exponential function by the min value
  logfreq = log(normf) / log(gap); // allows us to take the log (base gap) and the result 
  ilogf = round(logfreq);           // is the number of notes above the lowest, once we round it.
  f = minfreq * pow(gap,ilogf);  // we better "unlog" it.
  
  /* Wyłącz dźwięk, jeżeli wartość wyższa niż podana */
  int valmax=maxread-4;
  if(val>valmax) {
    if(tonecount>500) { 
    noTone(PHONES);                 // wylacza sygnal dzwiekowy
    }
    else { 
      tone(PHONES, val);            // wytwarza sygnal dźwiękowy
      tonecount++;
    }
  }
  else {
      tone(PHONES, val);              // wytwarza sygnal dźwiękowy
      tonecount=0;
  }

  /* Ustawia panel diod w zależności od odczytu z fotorezystora */
  if ((val>valmax-100) && (val<=valmax-87)) {
       if(lcdvolume!=1){
       for(i=0;i<=1;i++){
      lc.setLed(0,0,i,true);
      for(int j=0;j<=7;j++) {
       lc.setLed(0,j,i,true);
      }
    }
        //clear unused
      for(i=1;i<=7;i++){
      lc.setLed(0,0,i,false);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,false);
      }
    }
    }
    lcdvolume=1;
  }

  if ((val>valmax-87) && (val<=valmax-77)) {
       if(lcdvolume!=2){
       for(i=0;i<=2;i++){
      lc.setLed(0,0,i,true);
      for(int j=0;j<=7;j++) {
       lc.setLed(0,j,i,true);
      }
    }
    
        //clear unused
      for(i=2;i<=7;i++){
      lc.setLed(0,0,i,false);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,false);
      }
    }
    
    }
    lcdvolume=2;
  }

  if ((val>valmax-77) && (val<=valmax-66)) {
       if(lcdvolume!=3){
       for(i=0;i<=3;i++){
      lc.setLed(0,0,i,true);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,true);
      }
    }
    //clear unused
      for(i=3;i<=7;i++){
      lc.setLed(0,0,i,false);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,false);
      }
    }
    }
    lcdvolume=3;
  }
    if ((val>valmax-66) && (val<=valmax-55)) {
       if(lcdvolume!=4){
       for(i=0;i<=6;i++){
      lc.setLed(0,0,i,true);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,true);
      }
    }
    //clear unused
      for(i=4;i<=7;i++){
      lc.setLed(0,0,i,false);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,false);
      }
    }
    }
    lcdvolume=4;
  }
    if ((val>valmax-55) && (val<=valmax-44)) {
       if(lcdvolume!=5){
       for(i=0;i<=6;i++){
      lc.setLed(0,0,i,true);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,true);
      }
    }
    //clear unused
      for(i=5;i<=7;i++){
      lc.setLed(0,0,i,false);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,false);
      }
    }
    }
    lcdvolume=5;
  }

    if ((val>valmax-44) && (val<=valmax-33)) {
       if(lcdvolume!=6){
       for(i=0;i<=6;i++){
      lc.setLed(0,0,i,true);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,true);
      }
    }
    //clear unused
      for(i=6;i<=7;i++){
      lc.setLed(0,0,i,false);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,false);
      }
    }
    }
    lcdvolume=6;
  }

    if ((val>valmax-33) && (val<=valmax-23)) {
       if(lcdvolume!=7){
       for(i=0;i<=6;i++){
      lc.setLed(0,0,i,true);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,true);
      }
    }
    //clear unused
      for(i=7;i<=7;i++){
      lc.setLed(0,0,i,false);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,false);
      }
    }
    }
    lcdvolume=7;
  }
      if ((val>valmax-23) && (val<=valmax-7)) {
       if(lcdvolume!=8){
       for(i=0;i<=8;i++){
      lc.setLed(0,0,i,true);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,true);
      }
    }
    //clear unused
      for(i=8;i<=7;i++){
      lc.setLed(0,0,i,false);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,false);
      }
    }
    }
    lcdvolume=8;
  }
  /* Wartość maksymalna */
  if(val>valmax-6) {
    if(lcdvolume!=0) {
    cleanled();
    }
    lcdvolume=0;
  }
  /*
    if(val>valmax-15) {
       if(lcdvolume!=9){
       for(i=0;i<=7;i++){
      lc.setLed(0,0,i,false);
         for(int j=0;j<=7;j++) {
           lc.setLed(0,j,i,false);
      }
    }
    }
    lcdvolume=9;
  }
  */
}

