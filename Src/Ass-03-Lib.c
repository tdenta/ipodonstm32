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

	//Wait on the serial mutex: avoids conflicts in serial port writing by several threads
	osMutexWait(serialOutputMutexHandle, osWaitForever);

	status = HAL_UART_Transmit_IT(&huart2, s, strlen((char*)s));

	//The semaphore wait is intended to prevent the thread from getting out of WriteConsole until writing to the serial port is actually done
	//This semaphore is released in the HAL_UART_TxCpltCallback that is fired when transmission is done
	//The callback is defined in freertos.c
	osSemaphoreWait(serialOutputSemHandle, osWaitForever);

	//We have the guarantee that writing is done. We can release the mutex for all other threads that might want to output something
	osMutexRelease(serialOutputMutexHandle);
}

// STEPIEN: Read the serial port

void ReadConsole(uint8_t *c)
{

	HAL_UART_Receive_IT(&huart2, c, 1);
	osSemaphoreWait(myBinarySem01Handle, osWaitForever);
}
