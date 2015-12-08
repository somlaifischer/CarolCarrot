#include <MIDI.h>
#include "pitches.h"

MIDI_CREATE_DEFAULT_INSTANCE();
#define LED 13

int carolLow[] = {70, 69, 70, 67};
int carolHigh[] = {73, 72, 73, 70};
int frame = 0;


int pinValues[] = {0, 0};
boolean pinPlaying[] = {false, false};
int pinPlayheads[] = {0, 0};

void setup()
{
  pinMode(LED, OUTPUT);
  // Launch MIDI and listen to channel 4
  MIDI.begin(4);
  Serial.begin(9600);
}

void loop()
{
  frame++;

  pinValues[0] = analogRead(1);
  pinValues[1] = analogRead(2);
  //Serial.println(val);

  playSequence(0, carolLow, 1);
  playSequence(1, carolHigh, 1);

  delay(50);
}



void playSequence(int pin, int sequence[], int channel)
{
  int seqLength = sequence.length;
  if (!pinPlaying[pin] && pinValues[pin] < 500)
  {
    MIDI.sendNoteOn(sequence[pinPlayheads[pin] % seqLength], 127, channel);
    pinPlaying[pin] = true;
  }
  else if (pinPlaying[pin] && pinValues[pin] > 500)
  {
    MIDI.sendNoteOff(sequence[pinPlayheads[pin] % seqLength], 127, channel);
    pinPlaying[pin] = false;
    pinPlayheads[pin]++;
  }
}



void playNote(uint16_t note, int duration, int channel)
{
  MIDI.sendNoteOn(note, 127, channel);
  delay(duration);
  MIDI.sendNoteOff(note, 127, channel);
}


void playCarol()
{
  int octave = 12;

  int i = 0;

  while (i < 4)
  {

    playNote(58 + octave, 400, 1);
    playNote(57 + octave, 200, 1);
    playNote(58 + octave, 200, 1);
    playNote(55 + octave, 400, 1);
    i++;
  }
  while (i < 8)
  {
    playNote(61 + octave, 400, 1);
    playNote(60 + octave, 200, 1);
    playNote(61 + octave, 200, 1);
    playNote(58 + octave, 400, 1);
    i++;
  }

}



// drum part
/*
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






