//     $Date: 2017-05-16 08:00:09 +1000 (Tue, 16 May 2017) $
// $Revision: 971 $
//   $Author: Peter $

#include "Ass-03.h"

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE
//

void Ass_03_Task_01(void const * argument)
{

  GPIO_PinState s[5];
  GPIO_PinState p[5] = {GPIO_PIN_RESET,GPIO_PIN_RESET,GPIO_PIN_RESET,GPIO_PIN_RESET,GPIO_PIN_RESET};
  uint8_t i;
  uint32_t loop=0;
  uint8_t ts[100];

  // Initialize and turn on LCD and calibrate the touch panel (needs to be done first)
  BSP_LCD_Init();
  BSP_LCD_DisplayOn();

  // Display welcome message
  osMutexWait(myMutex01Handle, osWaitForever);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(5,5, (uint8_t*)"ELEC3730 Assignment 3",LEFT_MODE);
  BSP_LCD_DisplayStringAt(5,20, (uint8_t*)"Use the joystick for user input",LEFT_MODE);
  BSP_LCD_DisplayStringAt(5,35, (uint8_t*)"Up/down frequency, centre reset",LEFT_MODE);
  BSP_LCD_DisplayStringAt(5,50, (uint8_t*)"Sorry, no touch panel this time",LEFT_MODE);
  osMutexRelease(myMutex01Handle);

  WriteConsole((uint8_t *)"Hello from Task 1\n");

  while (1)
  {
    s[0]=HAL_GPIO_ReadPin(GPIOA, JOY_A_Pin);
    s[1]=HAL_GPIO_ReadPin(GPIOA, JOY_B_Pin);
    s[2]=HAL_GPIO_ReadPin(JOY_C_GPIO_Port, JOY_C_Pin);
    s[3]=HAL_GPIO_ReadPin(GPIOC, JOY_D_Pin);
    s[4]=HAL_GPIO_ReadPin(GPIOC, JOY_CTR_Pin);
    for (i=0;i<5;i++)
    {
      if (s[i] != p[i])
      {
        if (s[i] == GPIO_PIN_SET)
        {
           osMutexWait(myMutex01Handle, osWaitForever);
	   BSP_LCD_DisplayStringAt(5,85+i*15, (uint8_t*)"SET  ",LEFT_MODE);
	   osMutexRelease(myMutex01Handle);
        }
        else
        {
           osMutexWait(myMutex01Handle, osWaitForever);
  	   BSP_LCD_DisplayStringAt(5,85+i*15, (uint8_t*)"RESET",LEFT_MODE);
    	   osMutexRelease(myMutex01Handle);
  	  // STEPIEN: Send message to Task 2
  	  osMessagePut (myQueue01Handle, (uint32_t)i, 0);
        }
        p[i]=s[i];
      }
      sprintf(ts,"Task 1: %d",loop);
      osMutexWait(myMutex01Handle, osWaitForever);
      BSP_LCD_DisplayStringAt(5,190, ts, LEFT_MODE);
      osMutexRelease(myMutex01Handle);
      loop++;
      // STEPIEN: Signal received from timer
      osSignalWait(1,osWaitForever);
    }
  }
}
