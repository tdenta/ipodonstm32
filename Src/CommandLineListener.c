//     $Date: 2017-05-02 10:39:11 +1000 (Tue, 02 May 2017) $
// $Revision: 941 $
//   $Author: Peter $

#include "Ass-03.h"

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE
//

void CommandLineListener(void const * argument)
{

//  GPIO_PinState s[5];
//  GPIO_PinState p[5] = {GPIO_PIN_SET,GPIO_PIN_SET,GPIO_PIN_SET,GPIO_PIN_SET,GPIO_PIN_SET};
  uint8_t i;

  // Initialize and turn on LCD and calibrate the touch panel
  BSP_LCD_Init();
  BSP_LCD_DisplayOn();

  // Display welcome message
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(5,5, (uint8_t*)"ELEC3730 Assignment 3",LEFT_MODE);
  BSP_LCD_DisplayStringAt(5,20, (uint8_t*)"Use the joystick for user input",LEFT_MODE);
  BSP_LCD_DisplayStringAt(5,35, (uint8_t*)"Up/down frequency, centre reset",LEFT_MODE);
  BSP_LCD_DisplayStringAt(5,50, (uint8_t*)"Sorry, no touch panel this time",LEFT_MODE);

  WriteConsole((uint8_t *)"Hello from Task 1\n");
  WriteConsole((uint8_t *)"After\n");
  CommandLineParserInit();

  while (1)
  {
	  CommandLineParserProcess();
  }
}
