#include <ADC.h>
#include <SPI.h>
#include <SD.h>
File f;
ADC *adc = new ADC();
const byte vals[] = {B1001, B10010, B100100, B1, B10, B100};
int c = 0;

void sendBit(byte pack) {
  GPIOA_PDOR |= 1<<12; //pin 3
  if(pack) {
    GPIOD_PDOR |= 1<<4; //pin 6
  }
  else {
    GPIOD_PDOR &= ~(1<<4);
  }
  GPIOA_PDOR &= ~(1<<12); //pin 3
}
void latch() {
  GPIOA_PDOR &= ~(1<<13); //pin 4
  GPIOA_PDOR |= 1<<13; //pin 4
}
void sendByte(byte pack) {
  for(byte i=0; i<8; i++) {
    //low end bit first, i.e. IN then ENABLE
    sendBit((pack>>i)&1);
  }
  latch();
}

void setup() {
  Serial.begin(9600);
  for(byte i=0; i<15; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(18, INPUT);
  
  while(!SD.begin(10))
    sendByte(0);
    
  SD.remove("LOGGER6.csv");
  f=SD.open("LOGGER6.csv", O_WRITE | O_CREAT);
  f.println("t,a,b,c,d,e,f");
  
  adc->setResolution(12, ADC_0);
  adc->setResolution(12, ADC_1);
  
  digitalWrite(14, 0);
  digitalWrite(2, 0);
  sendByte(B010010);
  delay(3000);
  sendByte(B011111);
  delayMicroseconds(7000);
}
void loop() {
  /*for(byte i=3; i>0; i--) {
    GPIOD_PDOR=(GPIOD_PDOR&(~B11))|i;
    //digitalWrite(2, i&B10);
    //digitalWrite(14, i&B1);
    delayMicroseconds(10);
    ADC::Sync_result r = adc->analogSyncRead(2, 3);
    Serial.print(r.result_adc0*((GPIOD_PDIR&B1000)*2-1));
    Serial.print('\t');
    Serial.print(r.result_adc1*((GPIOD_PDIR&B1000)*2-1));
    Serial.print('\t');
  }
  Serial.print('\n');
  delayMicroseconds(5000);*/
  
  sendByte(0);
  byte s = 0;
  signed int results[6];
  boolean prev = false;
  boolean flag = false;
  signed int minimum = 4096;
  signed int maximum = -4096;
  byte m[2];
  for(byte i=3; i>0; i--) {
    GPIOD_PDOR=(GPIOD_PDOR&(~B11))|i;
    delayMicroseconds(10);
    
    ADC::Sync_result r = adc->analogSyncRead(2, 3);
    boolean d[2] = { (GPIOD_PDIR>>3)&1, (GPIOC_PDIR>>3)&1 }; //pins 8 and 9 used for reading sign
    signed int val = r.result_adc0*(((int)d[0])*2-1);
    // results[i*2-2] = r.result_adc0*(((int)d[0])*2-1);
    // results[i*2-1] = r.result_adc1*(((int)d[0])*2-1);
    if(i!=3 && d[0]!=prev) {
      flag = true;
    }
    else {
      prev = d[0];
    }
    if(val<minimum) {
      minimum = val;
      m[0] = 3-i;
    }
    else if(val>maximum) {
      maximum = val;
      m[1] = 3-i;
    }
  }
  if(!flag)
    s = (prev<<(m[prev]+3))|(1<<m[prev]);
  else
    s = (1<<m[0])|(B1001<<m[1]);
  Serial.println(s, BIN);
  sendByte(s);
  if(digitalRead(18)) {
    f.print(micros());
    for(byte i=0; i<6; i++) {
      f.print(',');
      f.print(results[i]);
    }
    f.print('\n');
    c++;
    if(c>100) {
      f.flush();
      c = 0;
    }
  }
  delayMicroseconds(800);
}
  
