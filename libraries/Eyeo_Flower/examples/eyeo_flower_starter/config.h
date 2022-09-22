#ifndef CONFIG_H_
#define CONFIG_H_
///////////USER VARIABLES//////////////////////////////////////////////////////////////////////////////////

#define PETAL_COUNT 3           // how many petals i have 
#define LISTEN_DELAY 1000       // the delay between hearing a sound and emitting a sound
#define DEFAULT_IDLE_TIME 10000 // how long should i wait to sing when everyone is quiet


// Note Frequencies
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245

// what is my musical language
#define NUM_SCALE_NOTES 6
const int scaleNotes[] = { NOTE_D5, NOTE_F5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_D6 }; 


// what i react to
#if PETAL_COUNT == 3
int myTriggers[] = { NOTE_D6, NOTE_A5, NOTE_C6 };
#elif PETAL_COUNT == 4
int myTriggers[] = { NOTE_F5, NOTE_B5, NOTE_D6 };
#else
int myTriggers[] = { NOTE_F5, NOTE_A5 };
#endif


// what i emit
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
#endif /* CONFIG_H_ */
