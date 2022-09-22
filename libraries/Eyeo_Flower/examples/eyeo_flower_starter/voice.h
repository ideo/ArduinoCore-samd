/* 
 *  IDEO EYEO FLOWER COMMUNICATION EXAMPLE
 *  
 *  C# Minor Pentatonic
 *  C#,     E,      F#,     G#,     B,      C#,     E
 *  554.37  659.26  739.99  830.61  987.77  1108.7  1318.5
 *  
*/

//For making sounds
#include "config.h"
#include <MozziGuts.h>
#include <Oscil.h>
#include <EventDelay.h>
#include <ADSR.h>
#include <tables/cos4096_int8.h>
//#include <mozzi_midi.h>

// Defines for the Audio Out
#define AMP_PIN 0 // The enable pin for the amplifier
#define CONTROL_RATE 64

Oscil<COS4096_NUM_CELLS, AUDIO_RATE> aCos1(COS4096_DATA); //The Cosine oscillator data for the audio
EventDelay noteDelay;                                     //Delay object for timing before playing sounds and between notes
ADSR <AUDIO_RATE, AUDIO_RATE> envelope;                   //The envelope object for the sound


//This is where the high frequency audio magic happens we are stepping the DAC through the levels of the sine wave data 
// and applying the envelope to the amplitude
AudioOutput_t updateAudio(){
  envelope.update(); 
  return MonoOutput::fromAlmostNBit(16, envelope.next() * aCos1.next()).clip();
}
