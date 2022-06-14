/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "variant.h"

/*
 * Pins descriptions
 typedef struct _PinDescription
{
  EPortType       ulPort ;
  uint32_t        ulPin ;
  EPioType        ulPinType ;
  uint32_t        ulPinAttribute ;
  EAnalogChannel  ulADCChannelNumber ;
  EPWMChannel     ulPWMChannel ;
  ETCChannel      ulTCChannel ;
  EExt_Interrupts ulExtInt ;
} PinDescription ;
 */

/*
  PA00 - WS2812B LEDs
  PA02 - DAC (Speaker Out)
  PA06 - Petal PWM 4p
  PA07 - Petal PWM 6p
  PA08 - LM4890MX ~SD (Amp shutdown)
  PA09 - Mic Input
  PA10 - Red LED
  PA14 - Petal PWM 8p
  PA15 - Petal PWM 9p
  PA16 - Petal PWM 10p
  PA17 - Petal PWM 12p
  PA18 - Petal PWM 2p
  PA19 - Petal PWM 3p
  PA24 - USB/DM
  PA25 - USB/DP
  PA30 - SWCLK
  PA31 - SWDIO

// Potential arduino pin order
  PA08 - LM4890MX ~SD (Amp shutdown)
  PA00 - WS2812B LEDs
  PA17 - Petal PWM 12p
  PA18 - Petal PWM 2p
  PA19 - Petal PWM 3p
  PA06 - Petal PWM 4p
  PA07 - Petal PWM 6p
  PA14 - Petal PWM 8p
  PA15 - Petal PWM 9p
  PA16 - Petal PWM 10p
  PA02 - DAC (Speaker Out)
  PA09 - Mic Input
  PA10 - Red LED
  PA24 - USB/DM
  PA25 - USB/DP
  PA30 - SWCLK
  PA31 - SWDIO
*/
const PinDescription g_APinDescription[]=
{
  // GPIO 0: PA08 - LM4890MX ~SD (Amp shutdown)
  { PORTA,  8, PIO_DIGITAL, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },

  // GPIO 1: PA00 WS2812B LEDs
  { PORTA,  0, PIO_DIGITAL, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },

  // GPIO 2 - 9: Petal PWM Channels
  { PORTA, 17, PIO_PWM_ALT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM0_CH7, TCC0_CH7, EXTERNAL_INT_1 },
  { PORTA, 18, PIO_PWM_ALT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM0_CH2, TCC0_CH2, EXTERNAL_INT_2 },
  { PORTA, 19, PIO_PWM_ALT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM0_CH3, TCC0_CH3, EXTERNAL_INT_3 },
  { PORTA,  6, PIO_PWM,     (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM1_CH0, TCC1_CH0, EXTERNAL_INT_6 },
  { PORTA,  7, PIO_PWM,     (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM1_CH1, TCC1_CH1, EXTERNAL_INT_7 },
  { PORTA, 14, PIO_PWM_ALT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM0_CH4, TCC0_CH4, EXTERNAL_INT_14 },
  { PORTA, 15, PIO_PWM_ALT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM0_CH5, TCC0_CH5, EXTERNAL_INT_15 },
  { PORTA, 16, PIO_PWM_ALT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM0_CH6, TCC0_CH6, EXTERNAL_INT_0 },

  // GPIO 10: PA02 - DAC (Speaker Out)
  { PORTA,  2, PIO_ANALOG, (PIN_ATTR_ANALOG), DAC_Channel0, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2 },

  // GPIO 11: PA09 - Mic Input
  { PORTA,  9, PIO_ANALOG, (PIN_ATTR_ANALOG), ADC_Channel17, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_9 },

  // GPIO 12: PA10 - Red LED
  { PORTA, 10, PIO_DIGITAL, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM1_CH0, TCC1_CH0, EXTERNAL_INT_10 },

  // GPIO 13, 14, 15 USB
  // ----------------------------
  { PORTA, 23, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB Host enable
  { PORTA, 24, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB/DM
  { PORTA, 25, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB/DP

  // GPIO 16, 17 (SWCLK & SWDIO)
  // --------------------------
  { PORTA, 30, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
  { PORTA, 31, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },

} ;

const void* g_apTCInstances[TCC_INST_NUM+TC_INST_NUM]={ TCC0, TCC1, TCC2, TC3, TC4, TC5 } ;

// Multi-serial objects instantiation
SERCOM sercom0( SERCOM0 ) ;
SERCOM sercom1( SERCOM1 ) ;
SERCOM sercom2( SERCOM2 ) ;
SERCOM sercom3( SERCOM3 ) ;

/* No hardware serial pins - TODO: investigate using spare petal pins for optional hardware serial
Uart Serial1( &sercom0, PIN_SERIAL1_RX, PIN_SERIAL1_TX, PAD_SERIAL1_RX, PAD_SERIAL1_TX ) ;

void SERCOM0_Handler()
{
  Serial1.IrqHandler();
}
*/
