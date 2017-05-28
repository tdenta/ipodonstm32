/*
 * UIProcessingFunctions.c
 *
 *  Created on: 29 May 2017
 *      Author: c3276757
 */

#include "Ass-03.h"

void ProcessFileLine(JoystickDirection joystickAction){
	if(joystickAction == UP){
		screen_element_s* upperNeighbor = currentlySelectedElement->neighbors[UP];
		//If we have an upper neighbor
		if(upperNeighbor != NULL){
//			WriteConsole((uint8_t*)"Got to processing of up action\n");
//			sprintf((char*)stringDump, "Filepath of neighbor: %s\n", ((FSElement*)(upperNeighbor->specificParameter))->PathString);
//			WriteConsole((uint8_t*)stringDump);
			currentlySelectedElement->ElementDrawFunction_p(currentlySelectedElement->Xorigin,currentlySelectedElement->Yorigin, REGULAR, currentlySelectedElement->specificParameter);
			upperNeighbor->ElementDrawFunction_p(upperNeighbor->Xorigin,upperNeighbor->Yorigin, SELECTED, upperNeighbor->specificParameter);
			//Update the currently selected element
			currentlySelectedElement = upperNeighbor;
		}
	}else if(joystickAction == DOWN){
		screen_element_s* lowerNeighbor = currentlySelectedElement->neighbors[DOWN];
		//If we have an upper neighbor
		if(lowerNeighbor != NULL){
//			WriteConsole((uint8_t*)"Got to processing of down action\n");
//			sprintf((char*)stringDump, "Filepath of neighbor: %s\n", ((FSElement*)(lowerNeighbor->specificParameter))->PathString);
//			WriteConsole((uint8_t*)stringDump);
			currentlySelectedElement->ElementDrawFunction_p(currentlySelectedElement->Xorigin,currentlySelectedElement->Yorigin, REGULAR, currentlySelectedElement->specificParameter);
			lowerNeighbor->ElementDrawFunction_p(lowerNeighbor->Xorigin,lowerNeighbor->Yorigin, SELECTED, lowerNeighbor->specificParameter);
			//Update the currently selected element
			currentlySelectedElement = lowerNeighbor;
		}
	}else if(joystickAction == RIGHT){
		screen_element_s* rightNeighbor = currentlySelectedElement->neighbors[RIGHT];
		//If we have an upper neighbor
		if(rightNeighbor != NULL){
			currentlySelectedElement->ElementDrawFunction_p(currentlySelectedElement->Xorigin,currentlySelectedElement->Yorigin, REGULAR, currentlySelectedElement->specificParameter);
			rightNeighbor->ElementDrawFunction_p(rightNeighbor->Xorigin,rightNeighbor->Yorigin, SELECTED, rightNeighbor->specificParameter);
			//Update the currently selected element
			currentlySelectedElement = rightNeighbor;
		}
	}else if(joystickAction == LEFT){
		screen_element_s* leftNeighbor = currentlySelectedElement->neighbors[LEFT];
		//If we have an upper neighbor
		if(leftNeighbor != NULL){
			currentlySelectedElement->ElementDrawFunction_p(currentlySelectedElement->Xorigin,currentlySelectedElement->Yorigin, REGULAR, currentlySelectedElement->specificParameter);
			leftNeighbor->ElementDrawFunction_p(leftNeighbor->Xorigin,leftNeighbor->Yorigin, SELECTED, leftNeighbor->specificParameter);
			//Update the currently selected element
			currentlySelectedElement = leftNeighbor;
		}
	}
}
