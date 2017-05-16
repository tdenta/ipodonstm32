//     $Date: 2017-05-16 08:00:09 +1000 (Tue, 16 May 2017) $
// $Revision: 971 $
//   $Author: Peter $

#include "Ass-03.h"

//
// PLACE YOUR COMMON CODE HERE
//

// STEPIEN: Write to the serial port (update to use callback function, see below)

void WriteConsole(uint8_t *s)
{
  HAL_StatusTypeDef status;

  status = HAL_UART_Transmit_IT(&huart2, s, strlen(s));
  while (status != HAL_OK)
  {
    osThreadYield();
    status = HAL_UART_Transmit_IT(&huart2, s, strlen(s));
  }
}

// STEPIEN: Read the serial port

void ReadConsole(uint8_t *c)
{
  HAL_UART_Receive_IT(&huart2, c, 1);
  osSemaphoreWait(myBinarySem01Handle, osWaitForever);
}
