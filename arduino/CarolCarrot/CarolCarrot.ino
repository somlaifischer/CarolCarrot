#include <MIDI.h>
#include "pitches.h"

MIDI_CREATE_DEFAULT_INSTANCE();
#define LED 13

int carolLow[] = {70, 69, 70, 67};
int carolHigh[] = {73, 72, 73, 70};
int bongos[] = {60, 64, 61};
int cymbals[] = {52, 42, 37, 76};

int frame = 0;
int smoothNum = 6;
int pin0Values[] = {0, 0, 0, 0, 0, 0};
int pin1Values[] = {0, 0, 0, 0, 0, 0};
int pin2Values[] = {0, 0, 0, 0, 0, 0};
int pin3Values[] = {0, 0, 0, 0, 0, 0};
int averagePins[] = {0, 0, 0, 0};
boolean pinPlaying[] = {false, false, false, false};
int pinPlayheads[] = {0, 0, 0, 0};
int pinPlayedAt[] = {0, 0, 0, 0};

int treshold = 700;
boolean debug = false;

void setup()
{
  pinMode(LED, OUTPUT);
  // Launch MIDI and listen to channel 4
  MIDI.begin(5);
  Serial.begin(9600);
}

void loop()
{
  frame++;

  pin0Values[frame % smoothNum] = analogRead(0);
  pin1Values[frame % smoothNum] = analogRead(3);
  pin2Values[frame % smoothNum] = analogRead(2);
  pin3Values[frame % smoothNum] = analogRead(1);
  calcAverages();
  if (debug)
  {
    switch (frame % 4)
    {
      case 0:
        Serial.println(averagePins[0]);
        break;
      case 1:
        Serial.print("    ");
        Serial.println(averagePins[1]);
        break;
      case 2:
        Serial.print("        ");
        Serial.println(averagePins[2]);
        break;
      case 3:
        Serial.print("            ");
        Serial.println(averagePins[3]);
        break;
    }

  }

  playSequence(0, carolLow, 4, 1, treshold, 20);
  playSequence(1, carolHigh, 4, 1, treshold, 20);
  playSequence(2, bongos, 3, 2, treshold, 500);
  playSequence(3, cymbals, 4, 3, treshold, 500);

  delay(50);
}

void calcAverages()
{
  int add = 0;
  for (int i = 0; i < smoothNum; i++)
  {
    add += pin0Values[i];
  }
  averagePins[0] = add / smoothNum;

  add = 0;
  for (int i = 0; i < smoothNum; i++)
  {
    add += pin1Values[i];
  }
  averagePins[1] = add / smoothNum;

  add = 0;
  for (int i = 0; i < smoothNum; i++)
  {
    add += pin2Values[i];
  }
  averagePins[2] = add / smoothNum;

  add = 0;
  for (int i = 0; i < smoothNum; i++)
  {
    add += pin3Values[i];
  }
  averagePins[3] = add / smoothNum;
}

void playSequence(int pin, int sequence[], int seqLength, int channel, int sensorLimit, int timeLimit)
{
  if (debug)
  {
    //Serial.print("               ");
    //Serial.println(frame - pinPlayedAt[pin]);
  }
  if (!pinPlaying[pin] && averagePins[pin] < sensorLimit)
  {
    if (frame - pinPlayedAt[pin] > timeLimit) pinPlayheads[pin] = 0; // reset to start if tapped long ago
    MIDI.sendNoteOn(sequence[pinPlayheads[pin] % seqLength], 127, channel);
    pinPlaying[pin] = true;
    pinPlayedAt[pin] = frame;
  }
  else if (pinPlaying[pin] && averagePins[pin] > sensorLimit)
  {
    MIDI.sendNoteOff(sequence[pinPlayheads[pin] % seqLength], 127, channel);
    pinPlaying[pin] = false;
    pinPlayheads[pin]++;
  }
}


// drum part
/*
 * http://www.voidaudio.net/percussion.html
 * http://soundprogramming.net/file-formats/general-midi-drum-note-numbers/
// tr8080
c1 bass drum
a1 808 Mid Tom 2
e2 Reverse Cymbal
c3+ Low Bongo
c3 high Bongo
f4 Low Woodblock
b4 Jingle Bell
a4+ Shaker
*/




