#include "Adafruit_TLC5947.h"

// How many boards do you have chained?
#define NUM_TLC5974 2

#define data   23
#define clock   22
#define latch   20
#define oe -1  // set to -1 to not use the enable pin (its optional)

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

int notePin(byte note) {
  if (note > 35 && note < 84) {
    return (note - 36);
  } else {
    return -1;
  }
}

void onNoteOn(byte channel, byte note, byte velocity) {
//  Serial.println("-- ON --");
//  Serial.print("channel: ");
//  Serial.println(channel);
  Serial.print("note: ");
  Serial.println(note);
//  Serial.print("velocity: ");
//  Serial.println(velocity);
  int lightVelocity = velocity * 4096 / 127;
  Serial.print("lightVelocity: ");
  Serial.println(lightVelocity);
//  digitalWrite(LED_BUILTIN, HIGH);
//  analogWrite(LED_BUILTIN, lightVelocity);
//  analogWrite(notePin(note), 255);
  tlc.setPWM(convertIndex(notePin(note)), lightVelocity);
  tlc.write();
}

void onNoteOff(byte channel, byte note, byte velocity) {
//  digitalWrite(LED_BUILTIN, LOW);
//  analogWrite(notePin(note), LOW);
  tlc.setPWM(convertIndex(notePin(note)), LOW);
  tlc.write();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("TLC5974 Midi Blink");
  
  usbMIDI.setHandleNoteOn(onNoteOn);
  usbMIDI.setHandleNoteOff(onNoteOff);
  
  tlc.begin();

  for (int i = 0; i<48; i++) {
    Serial.println("new diod");
    tlc.setPWM(convertIndex(i), 4096);
    tlc.write();
    delay(30);
    tlc.setPWM(convertIndex(i),0);
    tlc.write();
    delay(20);
  }

  for (int i = 0; i<48; i++) {
    tlc.setPWM(convertIndex(i), 4096);
  }
  tlc.write();
  delay(200);
  for (int i = 0; i<48; i++) {
    tlc.setPWM(convertIndex(i), 0);
  }
  tlc.write();
}

void loop() {
  // put your main code here, to run repeatedly:
  usbMIDI.read();
}

int convertIndex(int index) {
  if (index > 11 && index < 24) {
    return 12 + (23 - index);
  } else if (index > 35) {
    return 36 +(47 - index);
  } else {
    return index;
  }
}
