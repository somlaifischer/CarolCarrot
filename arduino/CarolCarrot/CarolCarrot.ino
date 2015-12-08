#include <MIDI.h>
#include "pitches.h"

MIDI_CREATE_DEFAULT_INSTANCE();
#define LED 13

int carolLow[] = {70, 69, 70, 67};
int carolLowPlayhead = 0;
int carolHigh[] = {73, 72, 73, 70};
int carolHighPlayhead = 0;

int frame = 0;

int szoloPin = 1;
boolean szoloPinOn = false;
int szolo2Pin = 2;
boolean szolo2PinOn = false;

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
      int szoloRead = analogRead(szoloPin);
      int szolorRead = analogRead(szolo2Pin);
      //Serial.println(val);
    
      if (!szoloPinOn && szoloRead < 500)
      {
        MIDI.sendNoteOn(carolLow[carolLowPlayhead % 4], 127, 1);
        szoloPinOn = true;
      }
      else if (szoloPinOn && szoloRead > 500)
      {
        MIDI.sendNoteOff(carolLow[carolLowPlayhead % 4], 127, 1);
        szoloPinOn = false;
        carolLowPlayhead++;
      }
    
    
    
      if (!szolo2PinOn && szolo2Read < 500)
      {
        MIDI.sendNoteOn(carolHigh[carolHighPlayhead % 4], 127, 2);
        szolo2PinOn = true;
      }
      else if (szolo2PinOn && szolo2Read > 500)
      {
        MIDI.sendNoteOff(carolHigh[carolHighPlayhead % 4], 127, 2);
        szolo2PinOn = false;
        carolHighPlayhead++;
      }
    
      // delay(50);
      // MIDI.sendNoteOn(NOTE_C5,127,2);
      //MIDI.sendNoteOff(NOTE_E5,0,2);
    
    
      delay(50);
}



void play(uint16_t note, int duration, int channel)
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

    play(58 + octave, 400, 1);
    play(57 + octave, 200, 1);
    play(58 + octave, 200, 1);
    play(55 + octave, 400, 1);
    i ++;
  }
  while (i < 8)
  {
    play(61 + octave, 400, 1);
    play(60 + octave, 200, 1);
    play(61 + octave, 200, 1);
    play(58 + octave, 400, 1);
    i++;
  }

}

void loopUP()
{

  for (int i = 5; i < 30; i += 2)
  {
    MIDI.sendNoteOn(sNotePitches[i], 127, 1);
    delay(100);
    MIDI.sendNoteOff(sNotePitches[i], 127, 1);
  }
}
// notes
/*
a3+
a3
a3+
g3

d4
c4
d4
a3+
*/



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






