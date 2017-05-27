/*
 * UserInterface.c
 *
 *  Created on: 27 May 2017
 *      Author: c3183034
 */


#include "Ass-03.h"

const ButtonFuncs ButtonList[] = {
		{(int8_t*) "play", &PlayFunction},
		{(int8_t*) "back", &BackFunction},
		{(int8_t*) "fwd", &FwdFunction},
		{(int8_t*) "playslct", &PlaySlctFunction},
		{(int8_t*) "playlist", &PlayListFunction},
		{NULL,NULL}
};


int16_t joy;
osEvent event;
//joyId[5] = {"Left","Up","Down","Right","Center"};
int i = 0;

void UserInterface(void const * argument){



	  while(1){
		  osDelay(20);
		  event = osMessageGet(ButtonQueueHandle, osWaitForever);
	      if (event.status == osEventMessage)
	      {
	    	  joy = event.value.v;
	      }

	  }
}
