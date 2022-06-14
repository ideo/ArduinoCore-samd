/* 
 *  IDEO EYEO FLOWER COMMUNICATION EXAMPLE
 *  
 *  C# Minor Pentatonic
 *  C#,     E,      F#,     G#,     B,      C#,     E
 *  554.37  659.26  739.99  830.61  987.77  1108.7  1318.5
 *  
*/

//For making sounds
#include <MozziGuts.h>
#include <Oscil.h>
#include <EventDelay.h>
#include <ADSR.h>
#include <tables/cos4096_int8.h>
//#include <mozzi_midi.h>

// Defines for the Audio Out
#define AMP_PIN 0 // The enable pin for the amplifier
#define CONTROL_RATE 64

#define NOTE_C5  523
//#define NOTE_CS5 554
#define NOTE_D5  587
//#define NOTE_DS5 622
//#define NOTE_E5  659
#define NOTE_F5  698
//#define NOTE_FS5 740
//#define NOTE_G5  784
//#define NOTE_GS5 831
#define NOTE_A5  880
//#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
//#define NOTE_CS6 1109
#define NOTE_D6  1175
//#define NOTE_DS6 1245

Oscil<COS4096_NUM_CELLS, AUDIO_RATE> aCos1(COS4096_DATA); //The Cosine oscillator data for the audio
EventDelay noteDelay;                                     //Delay object for timing before playing sounds and between notes
ADSR <AUDIO_RATE, AUDIO_RATE> envelope;                   //The envelope object for the sound


//This is where the high frequency audio magic happens we are stepping the DAC through the levels of the sine wave data 
// and applying the envelope to the amplitude
AudioOutput_t updateAudio(){
  envelope.update(); 
  return MonoOutput::fromAlmostNBit(16, envelope.next() * aCos1.next()).clip();
}
