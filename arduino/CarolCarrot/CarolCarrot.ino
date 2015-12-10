#include <MIDI.h>
#include "pitches.h"

MIDI_CREATE_DEFAULT_INSTANCE();
#define LED 13

int carolLow[] = {70, 69, 70, 67};
int carolHigh[] = {73, 72, 73, 70};
int bongos[] = {60, 64, 61};
int cymbals[] = {52, 42, 37, 76};

int frame = 0;
int pinValues[] = {0, 0, 0, 0};
boolean pinPlaying[] = {false, false, false, false};
int pinPlayheads[] = {0, 0, 0, 0};
int pinPlayedAt[] = {0, 0, 0, 0};

int treshold = 800;

boolean debug = true;

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

  pinValues[0] = analogRead(0);
  pinValues[1] = analogRead(1);
  pinValues[2] = analogRead(2);
  pinValues[3] = analogRead(3);

  if (false)
  {
    switch (frame % 4)
    {
      case 0:
        Serial.println(pinValues[0]);
        break;
      case 1:
        Serial.print("    ");
        Serial.println(pinValues[1]);
        break;
      case 2:
        Serial.print("        ");
        Serial.println(pinValues[2]);
        break;
      case 3:
        Serial.print("            ");
        Serial.println(pinValues[3]);
        break;
    }

  }

  playSequence(0, carolLow, 4, 1, 800, 20);
  playSequence(1, carolHigh, 4, 1, 800, 20);
  playSequence(2, bongos, 3, 2, 800, 500);
  playSequence(3, cymbals, 4, 3, 800, 500);

  delay(50);
}



void playSequence(int pin, int sequence[], int seqLength, int channel, int sensorLimit, int timeLimit)
{
  if (debug)
  {
    //Serial.print("               ");
    //Serial.println(frame - pinPlayedAt[pin]);
  }
  if (!pinPlaying[pin] && pinValues[pin] < sensorLimit)
  {
    if (frame - pinPlayedAt[pin] > timeLimit) pinPlayheads[pin] = 0; // reset to start if tapped long ago
    MIDI.sendNoteOn(sequence[pinPlayheads[pin] % seqLength], 127, channel);
    pinPlaying[pin] = true;
    pinPlayedAt[pin] = frame;
  }
  else if (pinPlaying[pin] && pinValues[pin] > sensorLimit)
  {
    MIDI.sendNoteOff(sequence[pinPlayheads[pin] % seqLength], 127, channel);
    pinPlaying[pin] = false;
    pinPlayheads[pin]++;
  }
}


// drum part
/*
 * http://www.voidaudio.net/percussion.html
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




