/*
 * UIProcessingFunctions.c
 *
 *  Created on: 29 May 2017
 *      Author: c3276757
 *
 *      This file holds all the functions that are triggered when the joystick is moved
 *      The functions redraw the screen accordingly
 */

#include "Ass-03.h"

/*
 * Function ProcessFileLine
 * Handles the processing of an action started when FileLine object is selected
 * Requires the joystick position
 */
void ProcessFileLine(JoystickDirection joystickAction){
	//Attempt to change the selection if this is the action required; otherwise jump to center action
	GenericChangeSelection(joystickAction);

	if(joystickAction == CENTER){

		//If the click was made on a directory we need to enter it
		if(((FSElement*)currentlySelectedElement->specificParameter)->Type == DIRECTORY){
				//WriteConsole("Click on directory detected\n");

				//Find cd command to change current working directory
				const command_s* Cd_command_p = GetCommandByName((int8_t*)"cd");

				//Execute cd command
				//No need for mutex on filesystem since the cd function already contains a mutex
				//Pass the path leading to the directory, stored in the specificParameter attribute of the currentlySelectedElement screen element structure
				Cd_command_p->Function_p(1, (uint8_t*[]){(uint8_t*)((FSElement*)currentlySelectedElement->specificParameter)->FullPathString}, NULL);

				//WriteConsole("Working directory changed\n");

				//Code to erase only the file lines on the screen; not used
				/*int j = 0;
				//BEFORE cleaning the array of file objects we MUST clean the screen !!
				while(ScreenElementList[j].ElementType == LIST_ITEM){
					//Clear the file line first to erase any text that could remain
					ScreenElementList[j].ElementDrawFunction_p(ScreenElementList[j].Xorigin, ScreenElementList[j].Yorigin, CLEAR, NULL);
					j++;
				}*/

				//Clean the file list array in order to populate it again with the new directory's contents
				CleanFileListArray();

				//WriteConsole("Array of files cleaned\n");

				//Retrieving files from SD Card
				//Find ls command
				const command_s* Ls_command_p = GetCommandByName((int8_t*)"ls");
				int i = 0;

				//Execute it with silent argument (enables the FileList array to be populated) and FileList pointer to populate
				//No need for mutex since the ls function already contains a mutex
				Ls_command_p->Function_p(1, (uint8_t*[]){(uint8_t*)"silent"}, (void*)&FileList);

				//Reset selection to first item on the screen
				currentlySelectedElement = &(ScreenElementList[0]);

				//Clear LCD entirely to rewrite the new folder and all the buttons
				//Mutex this part to prevent other threads from drawing
				osMutexWait(LCDMutexHandle, osWaitForever);
				BSP_LCD_Clear(LCD_COLOR_WHITE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_SetFont(&Font12);
				osMutexRelease(LCDMutexHandle);

				//The new file list being populated we can draw the new list on screen as well as all the other buttons that were previously erased
				while(ScreenElementList[i].ElementName != NULL){

					//Update specific parameter which is a POINTER on void, with the structures retrieved by ls command
					//This is to be done only for the LIST_ITEM screen elements that represent a file or a folder
					//The seven lines will be affected with the seven first FSElement structures in the FileList array
					if(ScreenElementList[i].ElementType == LIST_ITEM){
						ScreenElementList[i].specificParameter = (void*)&(FileList[i]);
					}

					//If we hit the currently selected element, draw it differently, otherwise regular drawing
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
 * Frees the memory reserved in the FileList global variable for previous file paths and names
 * Puts NULL pointers to make sure memory can't be retrieved
 * Makes the structure ready to fill again, when we navigate through directories
 */
void CleanFileListArray(void){
		int i = 0;

		while(FileList[i].PathString != NULL){
			free(FileList[i].FullPathString);
			FileList[i].FullPathString = NULL;
			free(FileList[i].PathString);
			FileList[i].PathString = NULL;

			FileList[i].Type = BLANK_LINE;
			i++;
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
			//Re-draw the current selection in normal mode
			currentlySelectedElement->ElementDrawFunction_p(currentlySelectedElement->Xorigin,currentlySelectedElement->Yorigin, REGULAR, currentlySelectedElement->specificParameter);
			//Draw the new selection in selected mode
			upperNeighbor->ElementDrawFunction_p(upperNeighbor->Xorigin,upperNeighbor->Yorigin, SELECTED, upperNeighbor->specificParameter);
			//Update the currently selected element
			currentlySelectedElement = upperNeighbor;
		}
	}else if(joystickAction == DOWN){
		screen_element_s* lowerNeighbor = currentlySelectedElement->neighbors[DOWN];
		//If we have an upper neighbor
		if(lowerNeighbor != NULL){
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
