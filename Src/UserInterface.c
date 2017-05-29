/*
 * UserInterface.c
 *
 *  Created on: 27 May 2017
 *      Author: c3183034
 *
 *      Management of the user interface
 */


#include "Ass-03.h"

//Array of screen elements that defines the screen layout and state
screen_element_s ScreenElementList[] = {
		{(uint8_t*)"FileLine1",
				NULL,
				LIST_ITEM,
				0.0,
				0.1,
				{NULL, NULL, &(ScreenElementList[1]),NULL},
				&DrawFileLine,
				&ProcessFileLine
		},
		{(uint8_t*)"FileLine2",
				NULL,
				LIST_ITEM,
				0.0,
				0.2,
				{NULL, &(ScreenElementList[0]), &(ScreenElementList[2]),NULL},
				&DrawFileLine,
				&ProcessFileLine
		},
		{(uint8_t*)"FileLine3",
				NULL,
				LIST_ITEM,
				0.0,
				0.3,
				{NULL, &(ScreenElementList[1]), &(ScreenElementList[3]),NULL},
				&DrawFileLine,
				&ProcessFileLine
		},
		{(uint8_t*)"FileLine4",
				NULL,
				LIST_ITEM,
				0.0,
				0.4,
				{NULL, &(ScreenElementList[2]), &(ScreenElementList[4]),NULL},
				&DrawFileLine,
				&ProcessFileLine
		},
		{(uint8_t*)"FileLine5",
				NULL,
				LIST_ITEM,
				0.0,
				0.5,
				{NULL, &(ScreenElementList[3]), &(ScreenElementList[5]),NULL},
				&DrawFileLine,
				&ProcessFileLine
		},
		{(uint8_t*)"FileLine6",
				NULL,
				LIST_ITEM,
				0.0,
				0.6,
				{NULL, &(ScreenElementList[4]), &(ScreenElementList[6]),NULL},
				&DrawFileLine,
				&ProcessFileLine
		},
		{(uint8_t*)"FileLine7",
				NULL,
				LIST_ITEM,
				0.0,
				0.7,
				{NULL, &(ScreenElementList[5]), NULL,NULL},
				&DrawFileLine,
				&ProcessFileLine
		},
		{(uint8_t*)"PlayPauseButton",
				NULL,
				BUTTON,
				0.75,
				0.1,
				{NULL, NULL, NULL,NULL},
				&DrawPlayPauseButton,
				NULL
		},
		{(uint8_t*)"StopButton",
				NULL,
				BUTTON,
				0.75,
				0.35,
				{NULL, NULL, NULL,NULL},
				&DrawStopButton,
				NULL
		},
		{(uint8_t*)"CurrentTimeMin",
				(void*){102, 180},
				NONE,
				0.02,
				0.9,
				{NULL, NULL, NULL,NULL},
				&DrawCurrentTime,
				NULL
		},
		{NULL,
				NULL,
				NONE,
				0,
				0,
				{NULL,NULL,NULL,NULL},
				NULL,
				NULL
		}
};

//Pointer to the current selection, global variable, widely used
screen_element_s* currentlySelectedElement;
//JoystickPosition used to store the message coming from the queue
JoystickDirection joystickPosition;
osEvent event;

//int i = 0;

//Variables used to draw screen elements, relative values are related to absolute screen dimensions
uint32_t LCDXSize;
uint32_t LCDYSize;

void UserInterface(void const * argument){

	UserInterfaceInit();

	  while(1){
		  event = osMessageGet(ButtonQueueHandle, osWaitForever);
	      if (event.status == osEventMessage)
	      {
	    	  joystickPosition = event.value.v;
	      }

	      sprintf((char*)stringDump, "Joystick event detected: %d\n", (int)joystickPosition);
	      if(DebugLevel) WriteConsole((uint8_t*)stringDump);

	      //Call the element function with the joystick position to trigger an action
	      currentlySelectedElement->ElementFunction_p(joystickPosition);

	  }
}

