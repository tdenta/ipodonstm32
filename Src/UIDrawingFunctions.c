/*
 * UIDrawingFunctions.c
 *
 * This file holds all the functions required for drawing screen elements
 *
 *  Created on: 28 May 2017
 *      Author: c3276757
 */

#include "Ass-03.h"

//Room is provided for 30 files, for more, dynamic allocation has to be implemented
//This program will crash if more than 30 files are in the folder
//TODO: modify ls function to allow dynamic allocation
FSElement FileList [30];

void UserInterfaceInit(void){

	//Mutex this part to prevent other threads from drawing while LCD is not yet initialized
	osMutexWait(LCDMutexHandle, osWaitForever);
	// Initialize and turn on LCD and calibrate the touch panel (needs to be done first)
	BSP_LCD_Init();
	BSP_LCD_DisplayOn();

	BSP_LCD_Clear(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetFont(&Font12);
	osMutexRelease(LCDMutexHandle);

	LCDXSize = BSP_LCD_GetXSize();
	LCDYSize = BSP_LCD_GetYSize();

	//Retrieving files from SD Card

	//Find ls command
	const command_s* Ls_command_p = GetCommandByName((int8_t*)"ls");
	int i = 0;
	currentlySelectedElement = &(ScreenElementList[0]);

	//Execute it with silent argument and FileList to populate
	//No need for mutex since the ls function already contains a mutex
	Ls_command_p->Function_p(1, (uint8_t*[]){(uint8_t*)"silent"}, (void*)&FileList);

	//Debug output for filelist
	int j = 0;
	while(FileList[j].PathString != NULL){
		WriteConsole((uint8_t*)FileList[j].PathString);
		WriteConsole((uint8_t*)"\n");
		j++;
	}

	while(ScreenElementList[i].ElementName != NULL){
		//Update specific parameter which is a POINTER on void, with the structures retrieved by ls command
		if(ScreenElementList[i].ElementType == LIST_ITEM){
			ScreenElementList[i].specificParameter = (void*)&(FileList[i]);
		}

		/*((FSElement*)(ScreenElementList[i].specificParameter))->PathString*/

		//If we hit the currently selected element
		if(currentlySelectedElement == &(ScreenElementList[i])){
			ScreenElementList[i].ElementDrawFunction_p(ScreenElementList[i].Xorigin, ScreenElementList[i].Yorigin, SELECTED, ScreenElementList[i].specificParameter);
		}else{
			ScreenElementList[i].ElementDrawFunction_p(ScreenElementList[i].Xorigin, ScreenElementList[i].Yorigin, REGULAR, ScreenElementList[i].specificParameter);
		}
		i++;
	}
}

void DrawFileLine(float X, float Y, SelectionMode Mode, void* FileElement){

	osMutexWait(LCDMutexHandle, osWaitForever);

	//Saving the previous font used in order to set it back after drawing is finished
	sFONT* previousFont = BSP_LCD_GetFont();
	uint16_t previousColor = BSP_LCD_GetTextColor();

	//Relative width and height of the screen element
	float width = 0.7;
	float height = 0.1;

	//Setting a new font
	BSP_LCD_SetFont(&Font12);
	//Use the color defined by the mode
	BSP_LCD_SetTextColor(Mode);

	//Calculating the relative height of the text in order to center it in the rectangle
	float textHeight = (float)Font12.Height/LCDYSize;
	//Relative text margin
	float textMargin = (height - textHeight)/2;

	//Calculating all the important sizes in absolute pixel numbers (need the LCD sizes)
	uint16_t absoluteXorigin = (uint16_t)((float)LCDXSize*X);
	uint16_t absoluteYorigin = (uint16_t)((float)LCDYSize*Y);
	uint16_t absoluteTextMargin = (uint16_t)((float)LCDYSize*textMargin);
	uint16_t absoluteWidth = (uint16_t)(width*(float)LCDXSize);
	uint16_t absoluteHeight = (uint16_t)(height*(float)LCDYSize);

	if(Mode == REGULAR){
		//Draw the box that represents the file line
		BSP_LCD_DrawRect(absoluteXorigin, absoluteYorigin, absoluteWidth, absoluteHeight);

		//Display the file name in the center
		BSP_LCD_DisplayStringAt(absoluteXorigin + absoluteTextMargin, absoluteYorigin + absoluteTextMargin,((FSElement*)FileElement)->PathString, LCD_MODE);

		//Erase square at end of line to erase possible selection, warning:switch to white
		BSP_LCD_SetTextColor(CLEAR);
		BSP_LCD_FillRect(absoluteXorigin+absoluteWidth-absoluteTextMargin-Font12.Height, absoluteYorigin+absoluteHeight-absoluteTextMargin-Font12.Height, Font12.Height,Font12.Height);
	}else if(Mode == SELECTED){
		//Draw the box that represents the file line
		BSP_LCD_DrawRect(absoluteXorigin, absoluteYorigin, absoluteWidth, absoluteHeight);

		//Display the file name in the center
		BSP_LCD_DisplayStringAt(absoluteXorigin + absoluteTextMargin, absoluteYorigin + absoluteTextMargin,((FSElement*)FileElement)->PathString, LCD_MODE);

		//Draw a square at end of line to make selection clearer
		BSP_LCD_FillRect(absoluteXorigin+absoluteWidth-absoluteTextMargin-Font12.Height, absoluteYorigin+absoluteHeight-absoluteTextMargin-Font12.Height, Font12.Height,Font12.Height);
	}else if(Mode == CLEAR){
		//Clear the box that represents the file line with a white filled rectangle
		BSP_LCD_FillRect(absoluteXorigin, absoluteYorigin, absoluteWidth, absoluteHeight);
	}

	//Reset the font to its previous value
	BSP_LCD_SetFont(previousFont);
	BSP_LCD_SetTextColor(previousColor);

	osMutexRelease(LCDMutexHandle);
}

void DrawPlayPauseButton(float X, float Y, SelectionMode Mode, void* arg){

	//Relative width and height of the screen element
	float width = 0.2;
	float height = 0.2;

	//Calculating all the important sizes in absolute pixel numbers (need the LCD sizes)
	uint16_t absoluteXorigin = (uint16_t)((float)LCDXSize*X);
	uint16_t absoluteYorigin = (uint16_t)((float)LCDYSize*Y);


	osMutexWait(LCDMutexHandle, osWaitForever);

	BSP_LCD_SetTextColor(Mode);

	//Drawing the rectangle that represents the button
	BSP_LCD_DrawRect(absoluteXorigin, absoluteYorigin, (uint16_t)(width*(float)LCDXSize),(uint16_t)(height*(float)LCDYSize));

	//Defining the dimensions of the play triangle relative to the BUTTON size
	float triangleYDimension = 0.5;
	float triangleXDimension = (0.666666)*triangleYDimension;

	//Calculating the absolute position of the triangle center on the screen given the button position and the dimensions of the triangle relative to the button
	uint16_t absoluteButtonXcenter =(uint16_t)((width/2)*(float)LCDXSize) + absoluteXorigin;
	uint16_t absoluteButtonYcenter =(uint16_t)((height/2)*(float)LCDYSize) + absoluteYorigin;

	//Defining the three edges of the triangle in absolute position, following the previous instructions
	Point P1 = {
			(uint16_t)((float)absoluteButtonXcenter - (triangleXDimension/2)*width*(float)LCDXSize),
			(uint16_t)((float)absoluteButtonYcenter - (triangleYDimension/2)*height*(float)LCDYSize)
	};

	Point P2 = {
			(uint16_t)((float)absoluteButtonXcenter + (triangleXDimension/2)*width*(float)LCDXSize),
			(uint16_t)((float)absoluteButtonYcenter)
	};

	Point P3 = {
			(uint16_t)((float)absoluteButtonXcenter - (triangleXDimension/2)*width*(float)LCDXSize),
			(uint16_t)((float)absoluteButtonYcenter + (triangleYDimension/2)*height*(float)LCDYSize)
	};

	Point TrianglePoints[] = {P1, P2, P3};

	//Drawing the triangle
	BSP_LCD_DrawPolygon(TrianglePoints, (uint16_t) 3);

	//Draw the pause button -Will need to be conditional
	uint16_t pauseWidth = 0.1*width*(float)LCDXSize;
	uint16_t pauseHeight = 0.5*height*(float)LCDYSize;

	//Offsets
	uint16_t pauseYOffset = pauseHeight/2;
	uint16_t pauseXOffsetL = 1.5*pauseWidth;
	uint16_t pauseXOffsetR = 0.5*pauseWidth;


	//Print to LCD
	BSP_LCD_DrawRect((absoluteButtonXcenter - pauseXOffsetL), (absoluteButtonYcenter - pauseYOffset) ,pauseWidth, pauseHeight);
	BSP_LCD_DrawRect((absoluteButtonXcenter + pauseXOffsetR), (absoluteButtonYcenter - pauseYOffset) ,pauseWidth, pauseHeight);

	osMutexRelease(LCDMutexHandle);
}

void DrawStopButton(float X, float Y, SelectionMode Mode, void* arg){

	//Relative width and height of the screen element
	float width = 0.2;
	float height = 0.2;

	uint16_t absoluteXorigin = (uint16_t)((float)LCDXSize*X);
	uint16_t absoluteYorigin = (uint16_t)((float)LCDYSize*Y);

	//Calculating the absolute position of the triangle center on the screen given the button position and the dimensions of the triangle relative to the button
	uint16_t absoluteButtonXcenter =(uint16_t)((width/2)*(float)LCDXSize) + absoluteXorigin;
	uint16_t absoluteButtonYcenter =(uint16_t)((height/2)*(float)LCDYSize) + absoluteYorigin;

	//Stop button width based on button size
	uint16_t StopSideLength = 0.5*width*(float)LCDYSize;

	//Offsets
	uint16_t stopOffset = StopSideLength/2;

	osMutexWait(LCDMutexHandle, osWaitForever);

	//BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY); //Colour for box fill
	//BSP_LCD_FillRect(absoluteXorigin, absoluteYorigin, (uint16_t)(width*(float)LCDXSize),(uint16_t)(height*(float)LCDYSize));
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillRect((absoluteButtonXcenter - stopOffset), (absoluteButtonYcenter - stopOffset) ,StopSideLength, StopSideLength);

	BSP_LCD_SetTextColor(Mode); //Colour for borders
	BSP_LCD_DrawRect(absoluteXorigin, absoluteYorigin, (uint16_t)(width*(float)LCDXSize),(uint16_t)(height*(float)LCDYSize)); //Drawing the rectangle that represents the button
	BSP_LCD_DrawRect((absoluteButtonXcenter - stopOffset), (absoluteButtonYcenter - stopOffset) ,StopSideLength, StopSideLength); //Draw stop symbol

	osMutexRelease(LCDMutexHandle);
}

void DrawCurrentTimeMin(float X, float Y, SelectionMode Mode, void* CurrentTimeMin){

	osMutexWait(LCDMutexHandle, osWaitForever);

	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(X*(float)LCDXSize, Y*(float)LCDYSize, (uint8_t*)CurrentTimeMin, LEFT_MODE);
	BSP_LCD_SetFont(&Font12);

	osMutexRelease(LCDMutexHandle);
}
void DrawCurrentTimeTensOfSeconds(float X, float Y, SelectionMode Mode, void* CurrentTimeTensOfSeconds){

	osMutexWait(LCDMutexHandle, osWaitForever);

	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(X*(float)LCDXSize, Y*(float)LCDYSize, (uint8_t*)CurrentTimeTensOfSeconds, LEFT_MODE);
	BSP_LCD_SetFont(&Font12);

	osMutexRelease(LCDMutexHandle);

}
void DrawCurrentTimeSeconds(float X, float Y, SelectionMode Mode, void* CurrentTimeSeconds){

	osMutexWait(LCDMutexHandle, osWaitForever);

	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(X*(float)LCDXSize, Y*(float)LCDYSize, (uint8_t*)CurrentTimeSeconds, LEFT_MODE);
	BSP_LCD_SetFont(&Font12);

	osMutexRelease(LCDMutexHandle);

}
//void DrawFileSelection(float X, float Y){
//
//	// %'s to be replaced with input
//	uint16_t width = 0.7*((float)LCDXSize);
//	uint16_t height = 0.1*((float)LCDYSize);
//
//	osMutexWait(LCDMutexHandle, osWaitForever);
//
//	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
//	BSP_LCD_DrawRect((uint16_t)(X*(float)LCDXSize), (uint16_t)(Y*(float)LCDYSize) ,width , height);
//	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
//
//	osMutexRelease(LCDMutexHandle);
//}
