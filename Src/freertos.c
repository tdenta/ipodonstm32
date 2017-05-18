/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
// STEPIEN
#include "Ass-03.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osThreadId myTaskCommandLineListenerHandle;
osMessageQId myQueue01Handle;
osTimerId myTimer01Handle;
osMutexId myMutex01Handle;
osMutexId serialMutexHandle;
osSemaphoreId myBinarySem01Handle;
osSemaphoreId myCountingSem01Handle;

/* USER CODE BEGIN Variables */
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);
void StartCommandLineListener(void const *argument);
void Callback01(void const * argument);

extern void MX_FATFS_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of myMutex01 */
  osMutexDef(myMutex01);
  myMutex01Handle = osMutexCreate(osMutex(myMutex01));

  /* USER CODE BEGIN RTOS_MUTEX */

  osMutexDef(serialMutex);
  serialMutexHandle = osMutexCreate(osMutex(serialMutex));

  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of myBinarySem01 */
  osSemaphoreDef(myBinarySem01);
  myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);

  /* definition and creation of myCountingSem01 */
  osSemaphoreDef(myCountingSem01);
  myCountingSem01Handle = osSemaphoreCreate(osSemaphore(myCountingSem01), 2);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  osSemaphoreWait(myBinarySem01Handle, osWaitForever); // STEPIEN: Start with nothing
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of myTimer01 */
  osTimerDef(myTimer01, Callback01);
  myTimer01Handle = osTimerCreate(osTimer(myTimer01), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  // STEPIEN
  osTimerStart(myTimer01Handle, 50);
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
/*  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 512);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);*/

  /* definition and creation of myTask02 */
/*  osThreadDef(myTask02, StartTask02, osPriorityNormal, 0, 512);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);*/

  /* definition and creation of myTask03 */
/*  osThreadDef(myTask03, StartTask03, osPriorityNormal, 0, 512);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);*/

  /* definition and creation of CommandLineListener task */
  osThreadDef(CommandLineListenerTask, StartCommandLineListener, osPriorityNormal, 0, 512);
  myTaskCommandLineListenerHandle = osThreadCreate(osThread(CommandLineListenerTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of myQueue01 */
  osMessageQDef(myQueue01, 16, uint16_t);
  myQueue01Handle = osMessageCreate(osMessageQ(myQueue01), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
  /* init code for FATFS */
  MX_FATFS_Init();

  /* USER CODE BEGIN StartDefaultTask */
  // STEPIEN
  Ass_03_Task_01(argument);
  /* USER CODE END StartDefaultTask */
}

/* StartTask02 function */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  // STEPIEN
  osDelay(100);
  Ass_03_Task_02(argument);
  /* USER CODE END StartTask02 */
}

/* StartTask03 function */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
  // STEPIEN
  osDelay(200);
  Ass_03_Task_03(argument);
  /* USER CODE END StartTask03 */
}

/* StartCommandLineListener function */
void StartCommandLineListener(void const *argument){
	CommandLineListener(argument);
}

/* Callback01 function */
void Callback01(void const * argument)
{
  /* USER CODE BEGIN Callback01 */
  
  // STEPIEN: Period timer to signal reading joystick
  osSignalSet(defaultTaskHandle, 1);

  /* USER CODE END Callback01 */
}

/* USER CODE BEGIN Application */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // STEPIEN: Signal that data is available
  osSemaphoreRelease(myBinarySem01Handle);
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
