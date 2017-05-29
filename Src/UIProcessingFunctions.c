/*
 * UIProcessingFunctions.c
 *
 *  Created on: 29 May 2017
 *      Author: c3276757
 */

#include "Ass-03.h"

/*
 * Function ProcessFileLine
 * Handles the processing of an action started when FileLine object is selected
 * Requires the joystick position
 */
void ProcessFileLine(JoystickDirection joystickAction){
	GenericChangeSelection(joystickAction);
	if(joystickAction == CENTER){
		if(((FSElement*)currentlySelectedElement->specificParameter)->Type == DIRECTORY){
				//Find cd command
				const command_s* Cd_command_p = GetCommandByName((int8_t*)"cd");

				//Execute it
				//No need for mutex since the cd function already contains a mutex
				//Pass the path leading to the directory, stored in the currentlySelectedElement object
				Cd_command_p->Function_p(1, (uint8_t*[]){(uint8_t*)((FSElement*)currentlySelectedElement->specificParameter)->FullPathString}, NULL);

				//Clean the file list array
				CleanFileListArray();

				//Retrieving files from SD Card

				//Find ls command
				const command_s* Ls_command_p = GetCommandByName((int8_t*)"ls");
				int i = 0;

				//Execute it with silent argument and FileList to populate
				//No need for mutex since the ls function already contains a mutex
				Ls_command_p->Function_p(1, (uint8_t*[]){(uint8_t*)"silent"}, (void*)&FileList);

				//Reset selection to first item in the folder
				currentlySelectedElement = &(ScreenElementList[0]);

				//The new file list being populated we can draw the new list on screen
				while(ScreenElementList[i].ElementName != NULL){
					//Update specific parameter which is a POINTER on void, with the structures retrieved by ls command
					if(ScreenElementList[i].ElementType == LIST_ITEM){
						ScreenElementList[i].specificParameter = (void*)&(FileList[i]);
					}

					//If we hit the currently selected element, draw it differently
					if(currentlySelectedElement == &(ScreenElementList[i])){
						ScreenElementList[i].ElementDrawFunction_p(ScreenElementList[i].Xorigin, ScreenElementList[i].Yorigin, SELECTED, ScreenElementList[i].specificParameter);
					}else{
						ScreenElementList[i].ElementDrawFunction_p(ScreenElementList[i].Xorigin, ScreenElementList[i].Yorigin, REGULAR, ScreenElementList[i].specificParameter);
					}
					i++;
				}
		}
	}
}


/*
 * Function CleanFileList Array
 * Frees the memory reserved for previous file paths and names
 * Makes the structure ready to fill again
 */
void CleanFileListArray(void){
		int i = 0;

		while(FileList[i].PathString != NULL){
			free(FileList[i].FullPathString);
			free(FileList[i].PathString);
		}
}

/*
 * Function GenericChangeSelection
 * Handles moving the selection on the screen in a generic way
 * Erases the previous selection on screen, draws a new one
 * Changes the object in the currentlySelectedElement pointer
 * Requires the joystick actions
 */
void GenericChangeSelection(JoystickDirection joystickAction){
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
