/*
 * UserInterface.c
 *
 *  Created on: 27 May 2017
 *      Author: c3183034
 */


#include "Ass-03.h"

/*const ButtonFuncs ButtonList[] = {
		{(int8_t*) "play", &PlayFunction},
		{(int8_t*) "back", &BackFunction},
		{(int8_t*) "fwd", &FwdFunction},
		{(int8_t*) "playslct", &PlaySlctFunction},
		{(int8_t*) "playlist", &PlayListFunction},
		{NULL,NULL}
};*/
screen_element_s ScreenElementList[] = {
		{(uint8_t*)"FileLine1",
				NULL,
				LIST_ITEM,
				0.0,
				0.1,
				{NULL, NULL, &(ScreenElementList[1]),NULL},
				&DrawFileLine,
				&ProcessFileLine,
				NULL
		},
		{(uint8_t*)"FileLine2",
				NULL,
				LIST_ITEM,
				0.0,
				0.2,
				{NULL, &(ScreenElementList[0]), &(ScreenElementList[2]),NULL},
				&DrawFileLine,
				&ProcessFileLine,
				NULL,
				NULL
		},
		{(uint8_t*)"FileLine3",
				NULL,
				LIST_ITEM,
				0.0,
				0.3,
				{NULL, &(ScreenElementList[1]), &(ScreenElementList[3]),NULL},
				&DrawFileLine,
				&ProcessFileLine,
				NULL,
				NULL
		},
		{(uint8_t*)"FileLine4",
				NULL,
				LIST_ITEM,
				0.0,
				0.4,
				{NULL, &(ScreenElementList[2]), &(ScreenElementList[4]),NULL},
				&DrawFileLine,
				&ProcessFileLine,
				NULL,
				NULL
		},
		{(uint8_t*)"FileLine5",
				NULL,
				LIST_ITEM,
				0.0,
				0.5,
				{NULL, &(ScreenElementList[3]), &(ScreenElementList[5]),NULL},
				&DrawFileLine,
				&ProcessFileLine,
				NULL,
				NULL
		},
		{(uint8_t*)"FileLine6",
				NULL,
				LIST_ITEM,
				0.0,
				0.6,
				{NULL, &(ScreenElementList[4]), &(ScreenElementList[6]),NULL},
				&DrawFileLine,
				&ProcessFileLine,
				NULL,
				NULL
		},
		{(uint8_t*)"FileLine7",
				NULL,
				LIST_ITEM,
				0.0,
				0.7,
				{NULL, &(ScreenElementList[5]), NULL,NULL},
				&DrawFileLine,
				&ProcessFileLine,
				NULL,
				NULL
		},
		{(uint8_t*)"PlayPauseButton",
				NULL,
				BUTTON,
				0.75,
				0.1,
				{NULL, NULL, NULL,NULL},
				&DrawPlayPauseButton,
				NULL,
				NULL,
				NULL
		},
		{NULL,
				NULL,
				NONE,
				0,
				0,
				{NULL,NULL,NULL,NULL},
				NULL,
				NULL,
				NULL,
				NULL
		}
};

screen_element_s* currentlySelectedElement;

JoystickDirection joystickPosition;

osEvent event;
//joyId[5] = {"Left","Up","Down","Right","Center"};
int i = 0;

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
	      WriteConsole((uint8_t*)stringDump);

	      currentlySelectedElement->ElementFunction_p(joystickPosition);

	  }
}

