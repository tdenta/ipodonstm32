//     $Date: 2017-05-16 08:00:09 +1000 (Tue, 16 May 2017) $
// $Revision: 971 $
//   $Author: Peter $

// Assignment 3 include file

#ifndef ASS_03_H_
#define ASS_03_H_

// Standard includes
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "stm32f4xx_hal.h"
#include "usart.h"
#include "uda1380.h"
#include "openx07v_c_lcd.h"
#include "fatfs.h"

#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

#include <ctype.h>

// OS handles
extern osThreadId defaultTaskHandle;
extern osThreadId myTask02Handle;
extern osThreadId myTask03Handle;
extern osThreadId myTaskCommandLineListenerHandle;
extern osTimerId myTimer01Handle;

extern osSemaphoreId myBinarySem01Handle;
extern osMessageQId myQueue01Handle;
extern osMutexId myMutex01Handle;

//User OS Handles
extern osMutexId audioBufferMutexHandle;
extern osMutexId serialOutputMutexHandle;
extern osMessageQId toneAmplitudeQueueHandle;
extern osMessageQId toneFrequencyQueueHandle;
extern osSemaphoreId serialOutputSemHandle;

// Assignment tasks
extern void Ass_03_Task_01(void const *argument);
extern void Ass_03_Task_02(void const *argument);
extern void Ass_03_Task_03(void const *argument);

extern void CommandLineListener(void const * argument);

// Library functions
extern void WriteConsole(uint8_t *s);
extern void ReadConsole(uint8_t *c);

//------------------ USER CUSTOM CODE ----------------

//Colors in the terminal (at least UNIX-supported)
//Source : http://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define BUFFER_MAX_SIZE 100

//User structures & global variables
typedef struct{
	int8_t *NameString;					//The name of the function
	int8_t (*Function_p) (
			uint8_t ArgNum,
			uint8_t *ArgStrings[],
			double *out);						//The pointer to the function implementing the command
	int8_t *HelpString;						//The help message for the function
} command_s;

extern const command_s CommandList[];

//User functions prototypes

//Command line processing functions
extern void CommandLineParserInit(void);
extern void CommandLineParserProcess(void);

extern int string_parser(char *inp, char **array_of_words[]);
extern const command_s* GetCommandByName(int8_t CommandName[]);
extern int8_t IsNumber(uint8_t* string);
extern int8_t IsFloatNumber(uint8_t* string);

extern int8_t ProcessArgString(double *out, uint8_t ArgCount, uint8_t *ArgsArray[]);


//Command functions
extern int8_t SubFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out);
extern int8_t DebugFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out);
extern int8_t HelpFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out);
extern int8_t AddFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out);
extern int8_t DivFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out);
extern int8_t MulFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out);
extern int8_t ListFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out);
extern int8_t ToneFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out);
extern int8_t LsFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out);

#endif /* ASS_03_H_ */
