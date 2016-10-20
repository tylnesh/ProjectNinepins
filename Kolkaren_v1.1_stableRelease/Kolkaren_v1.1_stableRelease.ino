/* 
This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


  #include <nRF24L01.h>
  #include <printf.h>
  #include <RF24.h>
  #include <RF24_config.h>
  #include <SPI.h>
  
  
  #define PXSENSOR_HORE 34
  #define PXSENSOR_DOLE 32
  #define PXSENSOR_PRETAZENIE 30
  #define PRESLAP_RAMP 26
  #define PRECHOD_RAMP 24
  #define DELIC_NASTAVENIE 22
  #define MOT_PRAVO 29 ////35 zmenili sme kvoli komunikácii 
  #define MOT_VLAVO 33
  #define MOT_SPOMAL 31
  #define LED_start 12
  #define LED_chyba 13
  #define LED_preslap 2
  
  #define RF_CE    37
  #define RF_CSN   35// lebo su PWM
  #define LED_PIN  23
  #define WIRE 450  // pairing the Arduino with RPI 150 pre lavu, 450 pre pravu
  
  #define PRETAZ 35
  
  int brightness = 80;
  int cakanieHore = 1200;
  int spustanie = 3000;
  int zrychlovanie = 3000;
  int casrozkruh = 75;
  int zavolatObsluhu;
  int pocitadloPretazenie;
  int pocetPadnutych = 0;
  bool prvykrat = true;
  int ret;
  
  int PXSENSOR[9] = {52, 50, 48, 46, 44, 42, 40, 38, 36};
  int MAG[9] = {53, 51, 49, 47, 45, 43, 41, 39, 25}; // treba magnet pripojit na pi 25 z pinu 37
  int LED[9] = {3, 4, 5, 6, 7, 8, 9, 10, 11};
  int kolky[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  int padnute[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  int posledneBody = 0;
  int body = 0;
  int typHry = 0;
  int pocetPrechodov = 0;
  int senzorRampa = 0;

  
  bool last = 0;
  bool novyStav = false;
  bool isStavanie = false;
  
  
  //*********************Komunikacia *******************************************************************
  
  RF24 radio(RF_CE, RF_CSN);
  
  const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };
  
  struct message {
    int cmd;
    bool pins[9];
    int wire;
    int rounds;
    //int points;
    //int score;
  
  };
  
  message outgoing;
  message incoming;
  
  void sndMsg()
  
  { radio.stopListening();
    Serial.println(F("Sending message..."));
    outgoing.wire = WIRE;
    for (int i =0; i<3; i++) { radio.write(&outgoing,sizeof(message)); delay(100);}
    last = outgoing.cmd; radio.startListening();
    vynulovatOutgoing();
  }
  
  void rcvMsg()
  
  { 
    radio.startListening();
    while (radio.available()) {
      Serial.println(F("Receiving message..."));
      radio.read(&incoming, sizeof(message));
      if(incoming.wire != WIRE) vynulovatIncoming();
      Serial.print(incoming.cmd);
    }
  }
  
  void setup()
  {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    outgoing.rounds = 0;
    outgoing.wire = WIRE;
    
    printf_begin();
  
    radio.begin();                           // Setup and configure rf radio
    radio.setChannel(1);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);
    radio.setAutoAck(true);                     // Ensure autoACK is enabled
    radio.setRetries(5, 30);                 // Optionally, increase the delay between retries & # of retries
  
    radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
  
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1, pipes[0]);
  
    radio.stopListening();                 // Start listening
    radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  
  
  
    pinMode(DELIC_NASTAVENIE, OUTPUT);
    pinMode(PXSENSOR[0], INPUT);
    pinMode(PXSENSOR[1], INPUT);
    pinMode(PXSENSOR[2], INPUT);
    pinMode(PXSENSOR[3], INPUT);
    pinMode(PXSENSOR[4], INPUT);
    pinMode(PXSENSOR[5], INPUT);
    pinMode(PXSENSOR[6], INPUT);
    pinMode(PXSENSOR[7], INPUT);
    pinMode(PXSENSOR[8], INPUT);
    pinMode(PXSENSOR_HORE, INPUT);
    pinMode(PXSENSOR_DOLE, INPUT);
    pinMode(PXSENSOR_PRETAZENIE, INPUT);
    pinMode(PRESLAP_RAMP, INPUT);
    pinMode(PRECHOD_RAMP, INPUT);
  
    pinMode(DELIC_NASTAVENIE, OUTPUT);
  
    pinMode(MAG[0], OUTPUT) ;
    pinMode(MAG[1], OUTPUT) ;
    pinMode(MAG[2], OUTPUT) ;
    pinMode(MAG[3], OUTPUT) ;
    pinMode(MAG[4], OUTPUT) ;
    pinMode(MAG[5], OUTPUT) ;
    pinMode(MAG[6], OUTPUT) ;
    pinMode(MAG[7], OUTPUT) ;
    pinMode(MAG[8], OUTPUT) ;
  
  
    pinMode(MOT_PRAVO, OUTPUT) ;
    pinMode(MOT_VLAVO, OUTPUT) ;
    pinMode(MOT_SPOMAL, OUTPUT) ;
    pinMode(LED[0], OUTPUT);
    pinMode(LED[1], OUTPUT) ;
    pinMode(LED[2], OUTPUT);
    pinMode(LED[3], OUTPUT);
    pinMode(LED[4], OUTPUT) ;
    pinMode(LED[5], OUTPUT) ;
    pinMode(LED[6], OUTPUT) ;
    pinMode(LED[7], OUTPUT) ;
    pinMode(LED[8], OUTPUT) ;
    pinMode(LED_start, OUTPUT);
    pinMode(LED_chyba, OUTPUT) ;
  
  
    for (int i = 0; i < 9; i++) {
      analogWrite(LED[i], 0);
    }
  
    analogWrite(LED_start, 0);
    analogWrite(LED_chyba, 0);
  
  
    digitalWrite(MOT_VLAVO, LOW);
    digitalWrite(MOT_PRAVO, LOW);
    digitalWrite(MOT_SPOMAL, LOW);
  
    for (int i = 0; i < 9; i++) {
      digitalWrite(MAG[i], LOW);
    }
  
  }
  //*****************************Ovladanie ***************************************************
  
  void komunikacia(void)
  
  {
  Serial.println("*****************Komunikacia******************");
  vynulovatIncoming();
    while (true) {
      radio.startListening();
      if (radio.available())
      {
        radio.read(&incoming, sizeof(message));
        
        //if (incoming.wire != WIRE) vynulovatIncoming();
        //rcvMsg();
        Serial.println(incoming.cmd);
  if (incoming.wire == WIRE)
  {
        switch (incoming.cmd){
          case 21:
          stavanie(); 
          Serial.print(" Stavanie kolkov . \n");
          break;
          
          case 22:
          kontrolaKolkov();
          Serial.print(" Kontrola kolkov. \n\n");
          break;
  
          case 23:
          kontrolaLED();
          Serial.print(" Kontrola led.\n \n");
          break;
  
          case 24:
          hraPlna();
          Serial.print(" Zaciatok hry plne . \n\n");
          break;
  
          case 25:
          Serial.print(" Zaciatok hry dorazka .\n \n");
          hraDorazka();        
          break;
  
          case 26:
          spustit();
          Serial.print(" Spustenie kolkov .\n \n");
          break;
          }
  }
      delay(100);
      } 
    }
  }
  
  //********************************************************************************************************
  //********************************************************************************************************
  //***************************LOOOOOOOOOOOOOOOOOOOOOOOOOPPPP*****************************************************************************
  //***************************LOOOOOOOOOOOOOOOOOOOOOOOOOOPPP***********************************************
  //********************************************************************************************************
  //********************************************************************************************************
  
  void loop() {
  
    body = 0; pocetPrechodov = 0;
    
    kontrolaLED();
    rozsvietVsetko();
    komunikacia();
  
  }
  //********************************************************************************************************
  //********************************************************************************************************
  //***************************LOOOOOOOOOOOOOOOOOOOOOOOOOPPPP*****************************************************************************
  //***************************LOOOOOOOOOOOOOOOOOOOOOOOOOOPPP***********************************************
  //********************************************************************************************************
  //********************************************************************************************************
  
  
  
  //********************************************************PLNA HRA*****************************************************
  
  void hraPlna() {
    vynulovatKolky();
    typHry = 1;
    bool isChyba = false;
    for (int i = 0; i < 9; i++) {
      analogWrite(LED[i], 0);
    }
  
    digitalWrite(DELIC_NASTAVENIE, HIGH);  // resetujeme na plosnom spoji ttl obvod 4793
    vynulovatKolky();
  
    if (stavanie() == 0 && kontrolaKolkov() == 0); else return; // Skontroluje podsystemy a ak funguju, spusti hru
  
    while (true) {
      analogWrite(LED_chyba, 0);
      isStavanie = false;
      vynulovatKolky();
      digitalWrite(DELIC_NASTAVENIE, HIGH);
       
      Serial.println("Pocet bodov: "); Serial.println(body);
      Serial.println("Pocet prechodov: "); Serial.println(pocetPrechodov);
  
      for (int i = 0; i < 9; i++) {
        analogWrite(LED[i], 0);
      } vynulovatKolky();
      Serial.println("Zacina hra");
      analogWrite(LED_start, brightness);
  
  
  
      senzorRampa = 0;
      int senzorPreslap = 0;
      bool ktoStlacilToCerveneTlacitko = false;
      while (senzorRampa < 2 && !ktoStlacilToCerveneTlacitko && !isStavanie) {    // cakame, kym budu viac ako 2 impulzy - eliminujeme chybu snímač
        if (digitalRead(PRESLAP_RAMP) == LOW) {    //pozor, tento senzor ma opacnu logiku
          senzorPreslap++;
          if (senzorPreslap >= 500)
            Serial.println("PRESLAP!"); // analogWrite(LED_chyba, brightness);delay(100);analogWrite(LED_chyba, 0);delay(100);analogWrite(LED_chyba, brightness);delay(100);analogWrite(LED_chyba, 0); //rozsvietime "zltu kartu"
        }
  
        if (digitalRead(PRECHOD_RAMP) == LOW) {  //pozor, tento senzor ma opacnu logiku
          senzorRampa++;
  
          Serial.println("Presla gula!"); analogWrite(LED_start, 0); break; //ak presla gula, pokracujeme kontrolovanim padu kolkov
        }
        if (digitalRead(PRECHOD_RAMP) == HIGH) { //pozor, tento senzor ma opacnu logiku ## ak nepride opakovany signal prechodu gule, tak mohla nastat chyba senzora, udaje vynulujeme a snimame dalej
          senzorRampa = 0;
  
        }
  
      if (radio.available()) { //vyskocenie do loop
        rcvMsg();
        Serial.print(incoming.cmd);
        
        switch (incoming.cmd){
          
          
          case -100:
          analogWrite(LED_start, LOW);
          loop();
          break;

          case -999:
          analogWrite(LED_start, LOW);
          novyStav = upravaStavu();
          analogWrite(LED_start, brightness);
          break;
  
          case 21:
          isStavanie = true;
          break;
  
          case 26:
          spustit();
          break;
  
          case -1:
          analogWrite(LED_start, LOW);
          analogWrite(LED_chyba, HIGH); 
          isChyba = potvrdenieChyby();
          ktoStlacilToCerveneTlacitko = true;
          break;
          
          default: novyStav = false;
          break;
        }
        
      }
      } 
      
      // if(!novyStav){
if(!isStavanie){
      pocetPrechodov++;
      
      
      //rozsvietPadnute(kontrolaKolkov());  //TODO - pridat funkciu rozsvietPadnute, ktora bude brat pole z kontrolaKolkov)
  
  
      unsigned long time;
      time = millis();
      while (millis() < time + 4000 && !ktoStlacilToCerveneTlacitko) {     //ak gula presla dalej, pockame 6  sekund (6 000 ms), pocas ktorych snimame a zapisujeme padnute kolky
         
        kontrolaKolkov(); //podprogram na kontrolu snimanie kolkov
        rozsvietPadnute(); 
  
      }
    if (!isChyba) pocitanieBodov();
    outgoing.cmd = 1;
    Serial.print("posielam prikaz: ");
  Serial.println(outgoing.cmd);
  outgoing.rounds = pocetPrechodov;
  Serial.print("posielam kolo: ");
  Serial.println(outgoing.rounds);
      outgoing.wire=WIRE;
         Serial.print("posielam cez wire: ");
  Serial.println(outgoing.wire);
     // if (isChyba) body = posledneBody;
      //if (!isChyba) outgoing.score = body;
      //Serial.print("posielam skore: ");
  //Serial.println(outgoing.score);
  Serial.println("posielam spravu...");
     // outgoing.points = 
      Serial.print("posielam body: ");
      //if (isChyba) outgoing.points = 0;
  //Serial.println(outgoing.points);
      sndMsg();
      //vynulovatOutgoing();
      isChyba = false;
      ktoStlacilToCerveneTlacitko = false; novyStav = false;
}
      stavanie();
      
  
      //TODO stavanie po padnuti kolkov , vynuluje kolky + vypne ledky + zapne led_start
  
    
    //} else {ktoStlacilToCerveneTlacitko = false; novyStav = false; stavanie();}
    }
  
  }
  //*********************************************************************************************************************
  
  
  //**************************************Hra Dorazka******************************************************************
  
  
  void hraDorazka() {
    bool isChyba= false;
    vynulovatKolky();
    typHry=2;
  
    for (int i = 0; i < 9; i++) {
      analogWrite(LED[i], 0);
    }
  
    digitalWrite(DELIC_NASTAVENIE, HIGH);  // resetujeme na plosnom spoji ttl obvod 4793
  
   if (stavanie() == 0 && kontrolaKolkov() == 0) ; 
   else return; // Skontroluje podsystemy a ak funguju, spusti hru
  
  while (true) {
      isStavanie = false;
      if (pocetPadnutych == 9) rozsvietVsetko();
      analogWrite(LED_chyba, LOW);

      Serial.println("Pocet bodov: "); Serial.println(body);
      Serial.println("Pocet prechodov: "); Serial.println(pocetPrechodov);
  
      Serial.println("Zacina hra");
      analogWrite(LED_start, brightness);
  
      senzorRampa = 0;
      int senzorPreslap = 0;
      bool ktoStlacilToCerveneTlacitko = false;
      while (senzorRampa < 2 && !ktoStlacilToCerveneTlacitko && !isStavanie) {    // cakame, kym budu viac ako 2 impulzy - eliminujeme chybu snímač
        if (digitalRead(PRESLAP_RAMP) == LOW) {    //pozor, tento senzor ma opacnu logiku
          senzorPreslap++;
          if (senzorPreslap >= 500)
            Serial.println("PRESLAP!"); // analogWrite(LED_chyba, brightness);delay(100);analogWrite(LED_chyba, 0);delay(100);analogWrite(LED_chyba, brightness);delay(100);analogWrite(LED_chyba, 0); //rozsvietime "zltu kartu"
        }
  
        if (digitalRead(PRECHOD_RAMP) == LOW) {  //pozor, tento senzor ma opacnu logiku
          senzorRampa++;
  
          Serial.println("Presla gula!"); analogWrite(LED_start, 0); break; //ak presla gula, pokracujeme kontrolovanim padu kolkov
        }
        if (digitalRead(PRECHOD_RAMP) == HIGH) { //pozor, tento senzor ma opacnu logiku ## ak nepride opakovany signal prechodu gule, tak mohla nastat chyba senzora, udaje vynulujeme a snimame dalej
          senzorRampa = 0;
  
        }
  
      if (radio.available()) { //vyskocenie do loop
        rcvMsg();
        Serial.print(incoming.cmd);
        
        switch (incoming.cmd){
          
          
          case -100:
          analogWrite(LED_start, LOW);
          loop();
          break;

          case -999:
          analogWrite(LED_start, LOW);
          novyStav = upravaStavu();
          analogWrite(LED_start, brightness);
          break;
  
          case 21:
          isStavanie = true;
          break;
  
          case 26:
          spustit();
          break;
  
          case -1:
          analogWrite(LED_start, LOW);
          analogWrite(LED_chyba, HIGH); 
          isChyba = potvrdenieChyby();
          ktoStlacilToCerveneTlacitko = true;
          break;
          
          default: novyStav = false;
          break;
        }
        
      }
      }
  if(!isStavanie){
  pocetPrechodov++;
  
  unsigned long time;
      time = millis();
      while (millis() < time + 4000 && !ktoStlacilToCerveneTlacitko) {
        kontrolaKolkov(); 
        rozsvietPadnute(); 
      }
      pocitanieBodov();
  
      outgoing.cmd = 2;
    Serial.print("posielam prikaz: ");
  Serial.println(outgoing.cmd);
      outgoing.rounds = pocetPrechodov;
  Serial.print("posielam kolo: ");
  Serial.println(outgoing.rounds);
    

         Serial.print("posielam wire: ");
  Serial.println(outgoing.wire);
  Serial.println("posielam spravu...");
   sndMsg();
      vynulovatOutgoing();
      ktoStlacilToCerveneTlacitko = false; novyStav = false;
  }
      stavanieDorazka();
  
  //} else { ktoStlacilToCerveneTlacitko = false; novyStav = false; stavanieDorazka();  } 
  
  }
  }
  
  //                                                          Stavanie
  //**********************************************************************************************************************************************
  
  
  int stavanie() {
  
    int pocitadloKolkyHore = 0;
    pocitadloPretazenie = 0;
    zavolatObsluhu = 0; //pokial bude viac ako 4, zavolat obsluhu
  analogWrite(LED_start, 0);
  
    while (pocitadloKolkyHore < 5)
    {
  
  
  
      unsigned long time;
      time = millis();
      int senzorHore = 0; bool kolyHore = false;
      while (millis() < time + 15000) {
  
  
       // Serial.println("stavanie");
  
        digitalWrite(MOT_PRAVO, HIGH);
        
        if (digitalRead(PXSENSOR_PRETAZENIE) == HIGH) {
          pocitadloPretazenie++;
          Serial.println("Pretazenie");
        }
  
  
        if (pocitadloPretazenie > PRETAZ ) {
          Serial.println("Nastalo pretazenie");
          zastavMotory(); pretazenie(); 
          digitalWrite(MOT_PRAVO, HIGH);
  
        }
        if (zavolatObsluhu > 8) {
          zastavMotory();
          Serial.print("Prid mi rozchlpit kolky");
          zachlpenie();
          pocitadloPretazenie = 0;
          zavolatObsluhu = 0;
        }
  
        if (digitalRead(PXSENSOR_HORE) == HIGH) senzorHore++;
  
        if (senzorHore > 3) {
          zastavMotory();
  
          Serial.println("Koly su hore"); kolyHore = true; break;
  
        }
      }
  
  
  
      if (kolyHore) {
        zastavMotory();
        delay(1000);
        digitalWrite(DELIC_NASTAVENIE, HIGH); vynulovatKolky();
         Serial.println("Spustam kolky");
  
        digitalWrite(MOT_PRAVO, HIGH);
        delay(650);//spustanie dole odstopovat
        digitalWrite(MOT_SPOMAL, HIGH);
        delay(2550);
        digitalWrite(MOT_SPOMAL, LOW);
       
       
        
        
  
  
  
  
        unsigned int time3;
        time3 = millis();
        while (millis() < time3 + 100000) {
          digitalWrite(DELIC_NASTAVENIE, HIGH);
          if (kontrolaKolkov() != 0) stavanie();
          if (digitalRead(PXSENSOR_DOLE) == HIGH) {
            Serial.println("Kolky su dole"); zastavMotory();  // ak su kolky dole, nechame ich tam
            if (kontrolaKolkov() != 0) stavanie();
             digitalWrite(DELIC_NASTAVENIE, HIGH);
            Serial.println("Stavanie prebeho prebehlo v poriadku");
            analogWrite(LED_start, brightness);
            return 0;
          }
        }
      }
    }
  }
  
  //*********************************************************************************************************************************************
  //                                                       Stavanie Dorazka
  //**********************************************************************************************************************************************
  
  
  int stavanieDorazka() {
  
  int pocitadloKolkyHore = 0;
    pocitadloPretazenie = 0;
    zavolatObsluhu = 0; //pokial bude viac ako 4, zavolat obsluhu
  analogWrite(LED_start, 0);
  
  
    while (pocitadloKolkyHore < 5)
    {
      unsigned long time;
      time = millis();
      int senzorHore = 0; bool kolyHore = false;
      while (millis() < time + 15000) {
       digitalWrite(MOT_PRAVO, HIGH);
        digitalWrite(DELIC_NASTAVENIE, HIGH);
        
        if (digitalRead(PXSENSOR_PRETAZENIE) == HIGH) {
          pocitadloPretazenie++;
          Serial.println("Pretazenie");
        }
  
        if (pocitadloPretazenie > PRETAZ ) {
          Serial.println("Nastalo pretazenie");
          zastavMotory(); pretazenie(); 
          digitalWrite(MOT_PRAVO, HIGH);
  
        }
        if (zavolatObsluhu > 8) {
          zastavMotory();
          Serial.print("Prid mi rozchlpit kolky");
          zachlpenie();
          pocitadloPretazenie = 0;
          zavolatObsluhu = 0;
        }
  
        if (digitalRead(PXSENSOR_HORE) == HIGH) senzorHore++;
  
        if (senzorHore > 3) {
          zastavMotory();
          Serial.println("Koly su hore"); kolyHore = true; break;
      }
      }
  
  if (kolyHore) {
        zastavMotory();
        delay(1000);
        digitalWrite(DELIC_NASTAVENIE, HIGH);
        
        int zaseknutieMAG=0;
        for (int i  = 0; i<9; i++) if (kolky[i]>0) zaseknutieMAG++;
        Serial.print("Pocet spadnutych kolkov: ");
        Serial.println(zaseknutieMAG);
  if (zaseknutieMAG < 9){
        for (int i  = 0; i<9; i++) if (kolky[i]>0) digitalWrite(MAG[i],HIGH);
  
         digitalWrite(MOT_PRAVO, HIGH);
        delay(650);//spustanie dole odstopovat
        
        for (int i  = 0; i<9; i++) if (kolky[i]>0) digitalWrite(MAG[i],LOW);
        
        digitalWrite(MOT_SPOMAL, HIGH);
        delay(2550);
        digitalWrite(MOT_SPOMAL, LOW);
  } else {
    for (int i  = 0; i<9; i++) digitalWrite(MAG[i],LOW);
     for (int i = 0; i < 9; i++)
    {
     kolky[i] = 0;
    }
    
    digitalWrite(MOT_PRAVO, HIGH);
        delay(650);//spustanie dole 
  digitalWrite(MOT_SPOMAL, HIGH);
        delay(2550);
        digitalWrite(MOT_SPOMAL, LOW);
  //pocetPadnutych = 0;  
    }
  
  unsigned int time3;
        time3 = millis();
        while (millis() < time3 + 100000) {
          digitalWrite(DELIC_NASTAVENIE, HIGH);
          if (kontrolaKolkov() != 0) stavanieDorazka();
          if (digitalRead(PXSENSOR_DOLE) == HIGH) {
            Serial.println("Kolky su dole"); zastavMotory();  // ak su kolky dole, nechame ich tam
             digitalWrite(DELIC_NASTAVENIE, HIGH);
            Serial.println("Stavanie prebeho prebehlo v poriadku");
            analogWrite(LED_start,brightness);
            return 0;
          }
        }
  
  }
    }
  }
    
  
  
  //**************************************************************************************************************************************************
  //***********************spustanie******************************************************************************************************************
  //*************************************************************************************************************************************************
  
  
 /*void testDorazka() {
    Serial.println("Test Dorazka");
    while (true)
    {
      if (radio.available())  
      {
        vynulovatIncoming(); rcvMsg(); vyskocit(); 
        Serial.println(); for (int i = 0; i < 9; i++) {
          Serial.println(incoming.pins[i]);
          if (incoming.pins[i] == 1)
          {
            analogWrite(LED[i], brightness);
          }
          
        } 
      }
  }
  komunikacia();
  //testDorazka();
  }*/
  
  
  //***************************************** VYNULOVANIE KOLKOV*******************************************
  void vynulovatKolky()
  {
    for (int i = 0; i < 9; i++)
    {
      kolky[i] = 0;
    }
  }
  //***************************************************************************************************
  //***************************************** VYNULOVANIE MAG*******************************************
  void vynulovatMAG()
  {
    for (int i = 0; i < 9; i++)
    {
      MAG[i] = 0;
    }
  }
  //***************************************************************************************************
  
  
  
  void vynulovatPadnute()
  {
    for (int i = 0; i < 9; i++)
    {
      padnute[i] = 0;
    }
  }
  
  
  
  
  //***************************************** KONTROLA KOLKOV*******************************************
  int kontrolaKolkov() {
    //digitalWrite(DELIC_NASTAVENIE, HIGH);
    // digitalWrite(DELIC_NASTAVENIE, HIGH);
    //delay(20);
  
digitalWrite(DELIC_NASTAVENIE, LOW);
     
  
  
    //vynulovatKolky();
  
    int val = 0;
    for (int i = 0; i < 9; i++)
    {
      if (digitalRead(PXSENSOR[i]) != HIGH) {
        val++;
      } else {
        Serial.print("Kolok ");
        Serial.print(i + 1);
       Serial.print(" je zhodeny. \n");
        kolky[i]++;
      }
  
    }
  
  
  
    //Serial.println(val); Serial.println(" kolky nie su zhodene\n");
    if (val == 9) {
      //Serial.println("Všetky kolky stoja\n");
      //vynulovatKolky();
      return 0;
      digitalWrite(DELIC_NASTAVENIE, HIGH);
      
    }
  
    else  return 1;
  
  
  }
  //*******************************************************************************************************************
  
  
  
  
  //***************************************************KONTROLA LED ***************************************************
  int kontrolaLED()
  {
    for (int i = 0; i < 9; i++) {
      analogWrite(LED[i], 70);
      delay(casrozkruh);
      analogWrite(LED[i], 0);
  
  
    }
  
  
     analogWrite(LED_start, 70); delay(200); analogWrite(LED_start, 0);
     analogWrite(LED_chyba, 70); delay(200); analogWrite(LED_chyba, 0);
     analogWrite(LED_start, 70); delay(200); analogWrite(LED_start, 0);
  }
  //**********************************************************************************************************************
  
  
  
  
  
  //****************************************************ROZSVIETENIE PADNUTYCH KOLKOV*************************************
  void rozsvietPadnute() {
    int pocetPadnutych = 0;
  
    for (int i = 0; i < 9; i++)
    {
      if (kolky[i] > 0) {
        pocetPadnutych++;
        analogWrite(LED[i], brightness);
      } else analogWrite(LED[i], 0);
    }
    if (pocetPadnutych == 8 && kolky[4] == 0 && typHry == 1) {
      rozsvietkruh();
      rozsvietkruh();
    }
    if (pocetPadnutych == 9) rozsvietVsetko();
    //outgoing.points = pocetPadnutych;
  }
  
  //*********************************************************************************************************************
  
  
  
  //****************************rozsviet kurh ////////////////////
  
  
  void rozsvietkruh() {
  
    for (int i = 0; i < 9; i++)
    {
      if (kolky[i] > 0) analogWrite(LED[i], 25);
    }
    analogWrite(LED[0], brightness);
    delay(casrozkruh);
    analogWrite(LED[0], 0);
    delay(casrozkruh);
    analogWrite(LED[1], brightness);
    delay(casrozkruh);
    analogWrite(LED[1], 0);
    delay(casrozkruh);
    analogWrite(LED[3], brightness);
    delay(casrozkruh);
    analogWrite(LED[3], 0);
    delay(casrozkruh);
    analogWrite(LED[6], brightness);
    delay(casrozkruh);
    analogWrite(LED[6], 0);
    delay(casrozkruh);
    analogWrite(LED[8], brightness);
    delay(casrozkruh);
    analogWrite(LED[8], 0);
    delay(casrozkruh);
    analogWrite(LED[7], brightness);
    delay(casrozkruh);
    analogWrite(LED[7], 0);
    delay(casrozkruh);
    analogWrite(LED[5], brightness);
    delay(casrozkruh);
    analogWrite(LED[5], 0);
    delay(casrozkruh);
    analogWrite(LED[2], brightness);
    delay(casrozkruh);
    analogWrite(LED[2], 0);
    delay(casrozkruh);
  
  }
  //******************************************************************************************************************
  //****************************rozsvietVsetko*******************************************************************
  //************************************************************************************************************
  void rozsvietVsetko() {
    for (int i = 0; i < 9; i++)
    {
      analogWrite(LED[i], brightness );
    }
    delay(2*casrozkruh);
    for (int i = 0; i < 9; i++)
    {
      analogWrite(LED[i], 0 );
    }
    delay(2*casrozkruh);
  
    for (int i = 0; i < 9; i++)
    {
      analogWrite(LED[i], brightness );
    }
    delay(2*casrozkruh);
    for (int i = 0; i < 9; i++)
    {
      analogWrite(LED[i], 0 );
    }
  
  }
  
  
  
  
  
  void ovladanieKolkov() {if (radio.available())
      {
        vynulovatIncoming(); rcvMsg(); vyskocit(); 
        Serial.println(); for (int i = 0; i < 9; i++) {
          Serial.println(incoming.pins[i]);
          if (incoming.pins[i] == 1)
          {
            analogWrite(LED[i], brightness);
            
          }
          
        } 
      }
      }
  
  void spustit() {
  
    digitalWrite(MOT_VLAVO, HIGH);
  
    delay(2000);
  
    digitalWrite(MOT_VLAVO, LOW);
  }
  
  
  void zachlpenie()
  {
     vynulovatIncoming();
    Serial.print ("Ak je rozchlpene, stlac klavesu 5");
    while (true) {
   
        radio.startListening();
      if (radio.available())
      {
        radio.read(&incoming, sizeof(message));
       // Serial.println(incoming.cmd);
        // if (incoming.destination == myID)
        //{
  
        if (incoming.wire == WIRE && incoming.cmd == 21) break;
      }
        
      
      int val = Serial.parseInt();
      if (val  == 5) break;
    }
  }
    
  void pretazenie()
  {
    zavolatObsluhu++;
    zastavMotory();
   delay(1000);
    digitalWrite(MOT_VLAVO, HIGH);
  
    delay(800);
  
    digitalWrite(MOT_VLAVO, LOW);
    delay(400);
    pocitadloPretazenie = 0;
  
  }
  
  void zastavMotory()
  {
    digitalWrite(MOT_PRAVO, LOW);
    digitalWrite(MOT_SPOMAL, LOW);
    digitalWrite(MOT_VLAVO, LOW);
  }
  
  void pocitanieBodov()
  {
    //posledneBody = body;
    for (int i = 0; i < 9; i++)
      if (kolky[i] > 0) {
        //body++; 
        outgoing.pins[i] = true;}
  }
  
  void vynulovatIncoming()
  {
    incoming.cmd = NULL; for (int i = 0; i<9; i++) incoming.pins[i] = NULL; incoming.wire = NULL;
   incoming.rounds=NULL;
  //incoming.points=NULL;incoming.score=NULL;
  }
  
  void vynulovatOutgoing()
  {
    outgoing.cmd = NULL; for (int i = 0; i<9; i++) outgoing.pins[i] = NULL; outgoing.rounds=NULL; //outgoing.score=NULL;
  //outgoing.points=NULL;
  }
  
  void vyskocit() {
    if (incoming.cmd == -100) { vynulovatIncoming(); loop();}
    }
  
  
  
  bool potvrdenieChyby()
  {
    bool docasneKolky[9] = {false,false,false,false,false,false,false,false,false};
    for (int i = 0; i<9; i++) docasneKolky[i] = kolky[i];
    analogWrite(LED_chyba, brightness);
      while (true) {
        
        if (digitalRead(PRECHOD_RAMP) == LOW) {  //pozor, tento senzor ma opacnu logiku
          senzorRampa++;
  
          Serial.println("Presla gula!"); analogWrite(LED_start, 0); //break; //ak presla gula, pokracujeme kontrolovanim padu kolkov
        }
        if (digitalRead(PRECHOD_RAMP) == HIGH) { //pozor, tento senzor ma opacnu logiku ## ak nepride opakovany signal prechodu gule, tak mohla nastat chyba senzora, udaje vynulujeme a snimame dalej
          senzorRampa = 0;
  
        }

 kontrolaKolkov();
 rozsvietPadnute();       
        if(radio.available())  {
          radio.read(&incoming, sizeof(message));
       
        if (incoming.wire == WIRE) {
       
  
        if (incoming.cmd == -3) {
          Serial.println("chyba sa nestala");
          analogWrite(LED_chyba, LOW);
          return false;
        }
        if (incoming.cmd == -2) {
          Serial.println("chyba sa stala");
         if (typHry == 1) { vynulovatKolky(); } else  { for (int i =0; i<9; i++) kolky[i] = docasneKolky[i]; }
          for (int i =0; i<9; i++) analogWrite(LED[i], LOW);
          analogWrite(LED_chyba, brightness);
           rozsvietPadnute();
           //body = posledneBody;
          return true;
        }
        }
        }
        }
    }

    bool upravaStavu()
    {
      while (true)
      {
        if(radio.available())  
       {
          radio.read(&incoming, sizeof(message));
       
        if (incoming.wire == WIRE) 
        {
       
  
        if (incoming.cmd == -1000) 
          {
          //body = incoming.score;
          pocetPrechodov = incoming.rounds;
          for (int i =0; i<9; i++) {kolky[i] = incoming.pins[i]; rozsvietPadnute();}
          
          //pocetPadnutych = incoming.points;
          return true;
          }
        if (incoming.cmd == -1001) {
          
          
          //body = posledneBody;
          return false;
                                   }
        }
      }    
     }
    }
