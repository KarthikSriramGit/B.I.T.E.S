#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>



#include "AudioSampleE0.h"

#include "AudioSampleG0.h"

#include "AudioSampleA0.h"

#include "AudioSampleB1.h"

#include "AudioSampleF1.h"

#include "AudioSampleE6.h"

AudioPlayMemory    Note1;
AudioPlayMemory    Note2;
AudioPlayMemory    Note3;
AudioPlayMemory    Note4;
AudioPlayMemory    Note5;
AudioPlayMemory    Note6;
AudioMixer4        mixer1;  
AudioOutputI2S     Headjack;

AudioConnection c1(Note1, 0, mixer1, 0);
AudioConnection c2(Note2, 0, mixer1, 1);
AudioConnection c3(Note3, 0, mixer1, 2);
AudioConnection c4(Note4, 0, mixer1, 3);
AudioConnection c5(Note5, 0, mixer1, 4);
AudioConnection c6(Note6, 0, mixer1, 5);
AudioConnection c7(mixer1, 0, Headjack, 0);
AudioConnection c8(mixer1, 0, Headjack, 1);
AudioControlSGTL5000 sgtl5000;

void setup() {
  AudioMemory(15);
  sgtl5000.enable();
  sgtl5000.volume(0.8);
  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);
  mixer1.gain(2, 0.5);
  mixer1.gain(3, 0.5);
  analogReadRes(16);
}

void loop() {
   delay(200);
    long val1,val2,val3,val4,val5,val6 = 0;
    long start = millis();
    val1 =  analogRead(A0);
    val2 =  analogRead(A1);
    val3 =  analogRead(A2);
    val4 =  analogRead(A3);
    val5 =  analogRead(A4);
    val6 =  analogRead(A5);
if (val1<10000) {
      Serial.print("Note E0");
      delay(50);
      Note1.play(AudioSampleE0);
      }
if (val2<10000) {
      Serial.print("Note G0");
      delay(50);
      Note2.play(AudioSampleG0);
      }
if (val3<10000) {
      Serial.print("Note A0");
      delay(50);
      Note3.play(AudioSampleA0);
      }
if (val4<10000) {
      Serial.print("Note B1");
      delay(50);
      Note4.play(AudioSampleB1);
      }
if (val5<10000) {
      Serial.print("NoteF1");
      delay(50);
      Note5.play(AudioSampleF1);
      }
if (val6<10000) {
      Serial.print("Note E6");
      delay(50);
      Note6.play(AudioSampleE6);
      }
      Serial.println(val2);
  delay(10); 
}
