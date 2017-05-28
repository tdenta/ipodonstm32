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
extern osThreadId audioManagerTaskHandle;

extern osSemaphoreId myBinarySem01Handle;
extern osMessageQId myQueue01Handle;
extern osMutexId LCDMutexHandle;

//User OS Handles
extern osMutexId audioBufferMutexHandle;
extern osMutexId serialOutputMutexHandle;
extern osMutexId DMAControllerMutexHandle;
extern osMutexId FSMutexHandle;
extern osMessageQId toneAmplitudeQueueHandle;
extern osMessageQId toneFrequencyQueueHandle;
extern osMessageQId ButtonQueueHandle;
extern osSemaphoreId serialOutputSemHandle;

extern osTimerId audioPlaybackTimerHandle;
extern osSemaphoreId audioOutputSemHandle;
extern osMessageQId audioOutputQueueHandle;


// Assignment tasks
extern void Ass_03_Task_01(void const *argument);
extern void Ass_03_Task_02(void const *argument);
extern void Ass_03_Task_03(void const *argument);

extern void CommandLineListener(void const * argument);
extern void AudioManager(void const *argument);
extern void UserInterface(void const *argument);

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
#define AUDIO_BUFFER_SIZE 512
#define SAMPLE_FREQ 8000
#define PATH_BUFFER_SIZE 300
#define FILE_BUFFER_SIZE 4
#define LCD_MODE LEFT_MODE

typedef enum{
	LEFT = 0,
	UP = 1,
	DOWN = 2,
	RIGHT = 3,
	CENTER = 4,
}JoystickDirection;

typedef enum{
	SINGLE_FILE,
	DIRECTORY
}FSElementType;

typedef enum{
	SELECTED =  LCD_COLOR_BLUE,	 //Light blue
	REGULAR = LCD_COLOR_BLACK		//Black
}SelectionMode;

typedef enum
{
	LIST_ITEM,
	BUTTON,
	NONE
}UIElementType;

//Structure used to store a path to a file
typedef struct{
	uint8_t *PathString;
	FSElementType Type;
} FSElement;

//User structures & global variables
typedef struct{
	int8_t *NameString;					//The name of the function
	int8_t (*Function_p) (
			uint8_t ArgNum,
			uint8_t *ArgStrings[],
			void *out);						//The pointer to the function implementing the command
	int8_t *HelpString;						//The help message for the function
} command_s;

typedef struct screen_element_s screen_element_s;
struct screen_element_s{
	uint8_t *ElementName;
	void* specificParameter;
	UIElementType ElementType;
	float Xorigin;
	float Yorigin;
	screen_element_s* neighbors[4];
	void (*ElementDrawFunction_p)(float X, float Y, SelectionMode Mode, void* parameter);
	//void (*ElementDrawSelectedFunction_p)(float X, float Y);
	//void (*ElementResetSelectionFunction_p)(float X, float Y);
	void (*ElementFunction_p)(JoystickDirection JoystickAction);
};

extern const command_s CommandList[];
extern screen_element_s ScreenElementList[];
extern screen_element_s* currentlySelectedElement;
extern FSElement FileList [30];
extern screen_element_s* upperNeighbor;
extern screen_element_s* lowerNeighbor;
extern screen_element_s* rightNeighbor;
extern screen_element_s* leftNeighbor;

//User functions prototypes

//Command line processing functions
extern void CommandLineParserInit(void);
extern void CommandLineParserProcess(void);

extern int string_parser(char *inp, char **array_of_words[]);
extern const command_s* GetCommandByName(int8_t CommandName[]);
extern int8_t IsNumber(uint8_t* string);
extern int8_t IsFloatNumber(uint8_t* string);

extern int8_t ProcessArgString(double *out, uint8_t ArgCount, uint8_t *ArgsArray[]);

extern int8_t SDCardInit(void);

extern int8_t continueReadingFlag;
extern int32_t audioSecondsRemaining;
extern uint8_t stringDump[300];
extern int16_t audioBuffer01[AUDIO_BUFFER_SIZE];
extern int16_t audioBuffer02[AUDIO_BUFFER_SIZE];
extern uint8_t pathOfCurrentWorkingDirectory[300];
extern FATFS SDFatFs;
extern const uint8_t* fsErrors[];

//Variables used in the copy function
extern FIL fsrc, fdst;      /* File objects */

//UI drawing variables
uint32_t LCDXSize;
uint32_t LCDYSize;

//Command functions
extern int8_t SubFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);
extern int8_t DebugFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);
extern int8_t HelpFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);
extern int8_t AddFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);
extern int8_t DivFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);
extern int8_t MulFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);
extern int8_t ListFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);
extern int8_t ToneFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);
extern int8_t CdFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);
extern int8_t LsFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);
extern int8_t MkdirFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);
extern int8_t RmFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);
extern int8_t CpFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out);

//Button functions
extern int8_t PlayFunction(uint8_t *s, uint8_t *CurrentButton[]);
extern int8_t BackFunction(uint8_t *s, uint8_t *CurrentButton[]);
extern int8_t FwdFunction(uint8_t *s, uint8_t *CurrentButton[]);
extern int8_t PlaySlctFunction(uint8_t *s, uint8_t *CurrentButton[]);
extern int8_t PlayListFunction(uint8_t *s, uint8_t *CurrentButton[]);

//UI drawing functions

void UserInterfaceInit(void);

extern void DrawFileLine(float X, float Y, SelectionMode Mode, void* filename);
extern void DrawPlayPauseButton(float X, float Y, SelectionMode Mode, void* arg);
//void DrawFileSelection(float X, float Y);

//UI Processing Functions

extern void ProcessFileLine(JoystickDirection joystickAction);

#endif /* ASS_03_H_ */
