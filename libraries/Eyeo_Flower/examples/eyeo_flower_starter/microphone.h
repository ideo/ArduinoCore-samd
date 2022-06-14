#ifndef MICROPHONE_H_
#define MICROPHONE_H_

#include <Adafruit_ZeroDMA.h>
#include "Adafruit_ZeroFFT.h" //For listening to sounds

// Defines for listening
#define ADC_PIN A1
#define FFT_DATA_SIZE 1024
#define FS 25000
#define SCALE_FACTOR 32
#define FREQSCALE 24.38
#define FREQOFFSET 0

#define FREQ_MIN 500
#define FREQ_MAX 2000

#define MIN_INDEX FFT_INDEX(FREQ_MIN, FS, FFT_DATA_SIZE)
#define MAX_INDEX FFT_INDEX(FREQ_MAX, FS, FFT_DATA_SIZE)

#define DOUBLE_BUFFER 0
#define AUTO_DC_OFFSET 0

#define FREQ_MARGIN 18       //margin is how much wiggle room there is in the frequency filter
#define NOISE_THRESHOLD 60  //higher number is fewer false positives, lower is better sensitivity

//Variables for the Microphone FFT code
Adafruit_ZeroDMA ADC_DMA;
DmacDescriptor *dmac_descriptor_1;
#if DOUBLE_BUFFER
DmacDescriptor *dmac_descriptor_2;
#endif

int16_t adc_buffer1[FFT_DATA_SIZE];
#if DOUBLE_BUFFER
int16_t adc_buffer2[FFT_DATA_SIZE];
volatile bool filling_first_half = true;
#endif
//int16_t fftData[FFT_DATA_SIZE];

volatile int16_t *active_adc_buffer;
volatile bool adc_buffer_filled = false;

// Helper to wait for ADC clock to sync with system clock
static void ADCsync() {
  while (ADC->STATUS.bit.SYNCBUSY == 1);
}

// Setup analog digital converter to free-run in the background 
void adc_init() {
  analogRead(ADC_PIN);
  
  ADC->CTRLA.bit.ENABLE = 0;
  ADCsync();
  ADC->INPUTCTRL.bit.GAIN = ADC_INPUTCTRL_GAIN_DIV2_Val;     // Set gain reference to 1/2 VCC
  ADC->REFCTRL.bit.REFCOMP = 1;                              // Enable reference compensation (not sure if this helps)
  ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INTVCC1_Val;  // Set reference to internal VCC
  ADCsync();
  ADC->INPUTCTRL.bit.MUXPOS = g_APinDescription[ADC_PIN].ulADCChannelNumber;  // Translate arduino pin to internal SAMD pin
  ADC->INPUTCTRL.bit.MUXNEG = ADC_INPUTCTRL_MUXNEG_GND_Val;                   // Use ground as the ADC reference floor
  ADCsync();
  ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1 | ADC_AVGCTRL_ADJRES(0);         // Only take one sample (no internal averaging)
  ADC->SAMPCTRL.reg = ADC_SAMPCTRL_SAMPLEN(1);                                // Sample time is 1 cycle
  ADCsync();
  ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV256 | ADC_CTRLB_FREERUN | ADC_CTRLB_RESSEL_12BIT; // Set ADC clock to 48Mhz / 256 = 187.5Khz, set ADC to FREERUN and 12bit resolution ~ 31,250 samples per second
  ADCsync();
  ADC->CTRLA.bit.ENABLE = 1;
  ADCsync();
}

void dma_callback(Adafruit_ZeroDMA *dma) {
  (void) dma;
#if DOUBLE_BUFFER
  if (filling_first_half) {
    // DMA is filling the first half of the buffer, use data from the second half
    active_adc_buffer = adc_buffer2;
    filling_first_half = false;
  } else {
    // DMA is filling the second half of the buffer, use data from the first half
    active_adc_buffer = adc_buffer1;
    filling_first_half = true;
  }
#else
  active_adc_buffer = adc_buffer1;
#endif
  adc_buffer_filled = true;
}

// Setup DMA to listen to "Ready" signals from ADC
static void dma_init() {
  ADC_DMA.allocate();
  ADC_DMA.setTrigger(ADC_DMAC_ID_RESRDY);
  ADC_DMA.setAction(DMA_TRIGGER_ACTON_BEAT);
  dmac_descriptor_1 = ADC_DMA.addDescriptor(
           (void *)(&ADC->RESULT.reg),
           adc_buffer1,
           FFT_DATA_SIZE,
           DMA_BEAT_SIZE_HWORD,
           false,
           true);
  dmac_descriptor_1->BTCTRL.bit.BLOCKACT = DMA_BLOCK_ACTION_INT;

#if DOUBLE_BUFFER
  dmac_descriptor_2 = ADC_DMA.addDescriptor(
           (void *)(&ADC->RESULT.reg),
           adc_buffer2,
           FFT_DATA_SIZE,
           DMA_BEAT_SIZE_HWORD,
           false,
           true);
  dmac_descriptor_2->BTCTRL.bit.BLOCKACT = DMA_BLOCK_ACTION_INT;
#endif

  ADC_DMA.loop(true);
  ADC_DMA.setCallback(dma_callback);
}

void initMicrophone() {
  pinMode(PIN_LED, OUTPUT);
  adc_init();
  dma_init();
  ADC_DMA.startJob();
}

// Fast Fouier Transform (FFT) code - this samples the microphone data and transforms it from the time domain to the frequency domain
// so we can look at the pitch of the sounds we are hearing. Think of this like a audio graphic equalizer readout.
int updateFFT(int16_t *fftData) {
    int32_t avg = 2048;  // transpose data by (12bit / 2) to move into signed integer

#if AUTO_DC_OFFSET
//    arm_mean_q15(fftData, FFT_DATA_SIZE, &avg);
    avg = 0;
    for(int i=0; i<FFT_DATA_SIZE; i++) avg += fftData[i];
    avg = avg / FFT_DATA_SIZE;
#endif

    // Let's use some fast ARM math...
//    arm_offset_q15(fftData, -avg, fftData, FFT_DATA_SIZE);
//    arm_shift_q15(fftData, 5, fftData, FFT_DATA_SIZE);

    for(int i=0; i<FFT_DATA_SIZE; i++) {
      fftData[i] -= avg;
      fftData[i] = fftData[i] << 5;
    }
    
    ZeroFFT(fftData, FFT_DATA_SIZE);
    int maxVal = 0;
    int freqIdx = -1;
    for(int i = MIN_INDEX; i < MAX_INDEX; i++) {
      if (fftData[i] > maxVal) {
        maxVal = fftData[i];
        if(maxVal > NOISE_THRESHOLD) {
          freqIdx = i;
        }
      }
    }
    
    analogWrite(PIN_LED, map(maxVal, 0, 2048, 0, 255)); // try to map the fft data to the red led...
    return freqIdx;
}


#endif /* MICROPHONE_H_ */
