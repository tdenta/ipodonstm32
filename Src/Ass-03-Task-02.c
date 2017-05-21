//     $Date: 2017-05-16 08:00:09 +1000 (Tue, 16 May 2017) $
// $Revision: 971 $
//   $Author: Peter $

#include "Ass-03.h"

#define BUFF_SIZE   512
#define SAMPLE_FREQ 8000
//#define F0          500
#define A0          10000
int16_t buffer[BUFF_SIZE];
//int16_t f, fp = 0;
//int32_t b;
int32_t frequency;

void Ass_03_Task_02(void const * argument)
{
  uint16_t i=0;
  //uint8_t s[100];
  osEvent event;

  // Initialise the audio data (note that this is very rough as frequency changes!)
  // At beginning no output as no command has been run yet
  for (i=0;i<BUFF_SIZE;i++)
  {
    //buffer[i] = (int16_t)(A0*sin(F0*(float)i/(float)SAMPLE_FREQ*2*3.14));
	  buffer[i] = 0;
  }
  HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t *)buffer, BUFF_SIZE);
  while(UDA1380_Init() != HAL_OK)
  {
      osThreadYield();
  }

  while (1)
  {
	  //Wait for the tone command to send a new frequency value
	  event = osMessageGet(myQueue01Handle, osWaitForever);
      if (event.status == osEventMessage)
      {
	  frequency = event.value.v;
	  //sprintf(s,"Task 2: %d (got %d)",i,b);
      }
//      osMutexWait(myMutex01Handle, osWaitForever);
//      BSP_LCD_DisplayStringAt(5,205, s, LEFT_MODE);
//      osMutexRelease(myMutex01Handle);
//      HAL_GPIO_TogglePin(GPIOD, LD4_Pin); // Toggle LED4
//      i++;

//      if (b == 1) fp=fp+10;
//      if (b == 2) fp=fp-10;
//      if (b == 4) fp=0;

      //Added mutex on the DMA buffer to avoid future conflicts
      osMutexWait(audioBufferMutexHandle, osWaitForever);

      //Load buffer with new frequency sine wave
      for (i=0;i<BUFF_SIZE;i++)
      {
    	  buffer[i] = (int16_t)(A0*sin((frequency)*(float)i/(float)SAMPLE_FREQ*2*3.14));
      }
      //Done: release mutex
      osMutexRelease(audioBufferMutexHandle);
  }
}

// Call back function for DMA controller
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t *)buffer, BUFF_SIZE);
}
