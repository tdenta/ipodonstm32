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
const screen_element_s ScreenElementList[] = {
		{(uint8_t*)"FileLine1",
				0.0,
				0.1,
				{NULL, NULL, NULL,NULL},
				&DrawFileLine,
				&DrawFileSelection,
				NULL,
				NULL
		},
		{(uint8_t*)"FileLine2",
				0.0,
				0.2,
				{NULL, NULL, NULL,NULL},
				&DrawFileLine,
				NULL,
				NULL,
				NULL
		},
		{(uint8_t*)"FileLine3",
				0.0,
				0.3,
				{NULL, NULL, NULL,NULL},
				&DrawFileLine,
				NULL,
				NULL,
				NULL
		},
		{(uint8_t*)"PlayPauseButton",
				0.75,
				0.1,
				{NULL, NULL, NULL,NULL},
				&DrawPlayPauseButton,
				NULL,
				NULL,
				NULL
		},
		{NULL,
				0,
				0,
				{NULL,NULL,NULL,NULL},
				NULL,
				NULL,
				NULL,
				NULL
		}
};


int16_t joy;
osEvent event;
//joyId[5] = {"Left","Up","Down","Right","Center"};
int i = 0;

uint32_t LCDXSize;
uint32_t LCDYSize;

void UserInterface(void const * argument){

	UserInterfaceInit();

	  while(1){
		  osDelay(20);
		  event = osMessageGet(ButtonQueueHandle, osWaitForever);
	      if (event.status == osEventMessage)
	      {
	    	  joy = event.value.v;
	      }

	  }
}

