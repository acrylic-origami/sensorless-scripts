#include <ADC.h>
ADC *adc = new ADC();
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
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(byte i=0; i<15; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  
  adc->setResolution(12, ADC_0);
  adc->setResolution(12, ADC_1);
  
  digitalWrite(14, 0);
  digitalWrite(2, 0);
  sendByte(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte s = B111; //full enables
  signed int results[6];
  for(byte i=3; i>0; i--) {
    GPIOD_PDOR=(GPIOD_PDOR&(~B11))|i;
    delayMicroseconds(100);
    
    ADC::Sync_result r = adc->analogSyncRead(2, 3);
    boolean d[2] = { (GPIOD_PDIR>>3)&1, (GPIOC_PDIR>>3)&1 }; //pins 8 and 9 used for reading sign
    results[i*2-2] = r.result_adc0*(((int)d[0])*2-1);
    results[i*2-1] = r.result_adc1*(((int)d[0])*2-1);
    if(r.result_adc0>r.result_adc1)
      s|=d[0]<<(6-i); //since we're counting from 3
    else
      s|=d[1]<<(6-i); //since we're counting from 3
  }
  for(byte i=0; i<6; i++) {
    Serial.print(results[i]);
    Serial.print('\t');
  }
  Serial.print('\n');
}
