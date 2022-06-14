/* 
 *  IDEO EYEO FLOWER COMMUNICATION EXAMPLE
 *  
 *  Flower Genetics:
 *  If I dont hear any notes in the scale for a range of time play my note
 *  If I hear my key, play my note (after a delay?), change my key to a random note in the scale
 *  
*/
#define DEBUG 1

#include "lights.h"
#include "voice.h"
#include "microphone.h"
#include "petals.h"

///////////USER VARIABLES//////////////////////////////////////////////////////////////////////////////////

#define PETAL_COUNT 3           // how many petals i have 
#define LISTEN_DELAY 1000       // the delay between hearing a sound and emitting a sound
#define DEFAULT_IDLE_TIME 10000 // how long should i wait to sing when everyone is quiet


//what i react to
#if PETAL_COUNT == 3
int myTriggers[] = { NOTE_D6, NOTE_A5, NOTE_C6 };
#elif PETAL_COUNT == 4
int myTriggers[] = { NOTE_F5, NOTE_B5, NOTE_D6 };
#else
int myTriggers[] = { NOTE_F5, NOTE_A5 };
#endif


//what i emit
#if PETAL_COUNT == 3
#define NUM_NOTES 3
int myNotes[] = { NOTE_F5, NOTE_B5, NOTE_D6 };
#elif PETAL_COUNT == 4
#define NUM_NOTES 3
int myNotes[] = { NOTE_D6, NOTE_A5, NOTE_C6 };
#else
#define NUM_NOTES 6
int myNotes[] = { NOTE_D6, NOTE_A5, NOTE_C6, NOTE_F5, NOTE_B5, NOTE_D6 };
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////

#define NUM_SCALE_NOTES 6
const int scaleNotes[] = { NOTE_D5, NOTE_F5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_D6 }; // define my musical language

boolean isListening = false;                              //This keeps track of whether the device is listening for sounds or playing sounds
byte noteStep;                                            //State machine state byte for playing the notes and triggering listening
long lastTime = 0;
int idleTime = DEFAULT_IDLE_TIME;


void setup(){
  //Start serial communication (only needed for debugging)
#if DEBUG
  Serial.begin(115200);
#endif
  analogReadResolution(12);
  randomSeed(analogRead(ADC_PIN));
  
  //Set the petals as outputs and turn them off
  for(int i=0; i<8; i++){
    pinMode(allPetals[i], OUTPUT);
    digitalWrite(allPetals[i], LOW);
  }

  //Initialize RGB leds
  pixels.begin(); 

  //Set petals to half-closed
  setAllPetals(90);

  //Initialize microphone
  initMicrophone();

  //Set the amp enable as an output and turn it off
  pinMode(AMP_PIN, OUTPUT);
  digitalWrite(AMP_PIN, LOW);

  //Set the voice state machine to the beginning, give it a second to start and start the sound engine
  noteStep = 0;
  noteDelay.set(1000); // 1 second countdown
  startMozzi();
}


void loop(){
  //If we are listening - do FFT stuff, if not, do audio stuff
  if(isListening){
    if(adc_buffer_filled) {
      adc_buffer_filled = false;
      int freqIdx = updateFFT((int16_t*)active_adc_buffer);
      if(freqIdx != -1) {
        checkNote(FFT_BIN(freqIdx, FS, FFT_DATA_SIZE));
      }
    }
  }
  updatePetals();
  audioHook(); // Send my voice to the speaker
}


// This function is called by Mozzi (audioHook()) at CONTROL_RATE (64) times a second
void updateControl(){
  if(!isListening) {
    playNote();
  } else if(millis() - lastTime > idleTime) {
    lastTime = millis();
    idleTime = DEFAULT_IDLE_TIME + random(3000);
    isListening = false;
    noteDelay.start(2000);
  }
}

void playNote() {
  if(!noteDelay.ready()) return;
  int a,d,s,r;
  int note;
  
  switch (noteStep) {
   //This happens right after it hears its trigger sounds
   case 0:
      digitalWrite(AMP_PIN, HIGH); //turn on the amp
      pixels.fill(pixels.Color(90,20,20));
      pixels.show();   // Send the updated pixel colors to the hardware.
      setAllPetals(120);
      noteDelay.start(LISTEN_DELAY);   //wait
      noteStep=1;
   break;
  
   case 1:
      note = myNotes[random(NUM_NOTES)];
      pixels.fill(pixels.ColorHSV(map(note, NOTE_D5, NOTE_D6, 0, 65535), 255, 50));
      pixels.show();   // Send the updated pixel colors to the hardware.
      setAllPetals(0);
      aCos1.setFreq(note);
      envelope.setADLevels(150,100); //Volume of attack and decay
      a = random(400) + 400;  // attack time
      d = 200;                // decay time
      s = 250;                // sustain time
      r = random(1000) + 500; // release time
      envelope.setTimes(a,d,s,r); //Length of time of A,D,S,R
      envelope.noteOn();
      noteDelay.start(a+d+s+r); //Time before the next note
      noteStep=2;
   break;
   
   //Turn off the amp and start listening
   case 2:
      digitalWrite(AMP_PIN, LOW);
      isListening=true;
      noteStep=0;
   break;
  }
}


void checkNote(int freq){
#if DEBUG
  Serial.println(freq);
#endif
  // If I hear my triggers play one of my notes...
  for(int i=0; i<3; i++) {
    if (freq >= myTriggers[i] - FREQ_MARGIN && freq <= myTriggers[i] + FREQ_MARGIN) {
      isListening = false;
      noteDelay.start(random(500) + 100);
    }
  }
  
  // If I hear any sounds in my language continue to wait for my turn
  for(int i=0; i<NUM_SCALE_NOTES; i++) {
    if (freq >= scaleNotes[i] - FREQ_MARGIN && freq <= scaleNotes[i] + FREQ_MARGIN) {
      lastTime = millis();
      break;
    }
  }
}
