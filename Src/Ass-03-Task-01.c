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


  /*// Initialize and turn on LCD and calibrate the touch panel (needs to be done first)
  BSP_LCD_Init();
  BSP_LCD_DisplayOn();

  // Set buttons
  osMutexWait(LCDMutexHandle, osWaitForever);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawVLine(  160, 2,  235);
  BSP_LCD_DrawRect( 170, 30, 40, 60); //Skip back button
  BSP_LCD_DrawRect( 220, 30, 40, 60);
  BSP_LCD_DrawRect( 270, 30, 40, 60);
  BSP_LCD_DrawRect( 170, 150, 140, 60);
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_DisplayStringAt(195,160, (uint8_t*)"SELECT",LEFT_MODE);
  BSP_LCD_DisplayStringAt(184,185, (uint8_t*)"PLAYLIST",LEFT_MODE);
  BSP_LCD_SetFont(&Font12);
  osMutexRelease(LCDMutexHandle);*/

  //WriteConsole((uint8_t *)"Hello from Task 1\n");

  while (1)
  {
    s[0]=HAL_GPIO_ReadPin(GPIOA, JOY_A_Pin);
    s[1]=HAL_GPIO_ReadPin(GPIOA, JOY_B_Pin);
    s[2]=HAL_GPIO_ReadPin(JOY_C_GPIO_Port, JOY_C_Pin);
    s[3]=HAL_GPIO_ReadPin(GPIOC, JOY_D_Pin);
    s[4]=HAL_GPIO_ReadPin(GPIOC, JOY_CTR_Pin);
    for (i=0;i<5;i++)
    {
    	//If there is a set
      if (s[i] != p[i])
      {
        if (s[i] == GPIO_PIN_SET)
        {
           osMutexWait(LCDMutexHandle, osWaitForever);
	   //BSP_LCD_DisplayStringAt(5,85+i*15, (uint8_t*)"SET  ",LEFT_MODE);
	   osMutexRelease(LCDMutexHandle);
	   //Send message to UI task
	   osMessagePut(ButtonQueueHandle, i, 0);
        }
        else
        {
           osMutexWait(LCDMutexHandle, osWaitForever);
  	   //BSP_LCD_DisplayStringAt(5,85+i*15, (uint8_t*)"RESET",LEFT_MODE);
    	   osMutexRelease(LCDMutexHandle);
  	  // STEPIEN: Send message to Task 2
        }
        p[i]=s[i];
      }
      sprintf((char*)ts,"Task 1: %d",(int)loop);
      osMutexWait(LCDMutexHandle, osWaitForever);
      //BSP_LCD_DisplayStringAt(5,190, ts, LEFT_MODE);
      osMutexRelease(LCDMutexHandle);
      loop++;
      // STEPIEN: Signal received from timer
      osSignalWait(1,osWaitForever);
    }
  }
}
