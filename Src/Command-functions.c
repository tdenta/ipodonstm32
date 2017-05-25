/*
 * Command-functions.c
 *
 *  Created on: 25 avr. 2017
 *      Author: Thomas
 */

#include "Ass-03.h"
FILINFO fno;
FRESULT res;
DIR dir;

//DebugLevel variable is used for the debug mode
uint8_t DebugLevel = 0;

//Audio buffers for ToneFunction
int16_t audioBuffer01[AUDIO_BUFFER_SIZE] = {0};
int16_t audioBuffer02[AUDIO_BUFFER_SIZE] = {0};

/*
 * Function SubFunction
 * Implements the sub command
 * Prints the result of a subtraction between two integers or two floating point numbers
 * Returns 1 if successful, 0 otherwise
 * Returns the result as floating point in the out variable
 */
int8_t SubFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out){

	//Function works only with two arguments
	if(ArgNum == 2){
		//Debug messages display depends on DebugLevel variable
		if(DebugLevel){
			printf(GRN "Correct argument number detected : %d \n" RESET, ArgNum);
		}

		//Two cases are considered: floating point and integer. This enables printing better result.

		//Function works with integer numbers
		if(IsNumber(ArgStrings[0]) && IsNumber(ArgStrings[1])){
			//Printing output
			//Need to use specific functions to get numbers out of strings

			int32_t digit1 = 0;
			int32_t digit2 = 0;

			sscanf((char*)ArgStrings[0], "%d", (int*)&digit1);
			sscanf((char*)ArgStrings[1], "%d", (int*)&digit2);

			int16_t result = digit1 - digit2;

			//Returning the result of the function inside an external integer
			*out = result;

			//PRINT TO CONSOLE
			sprintf(stringDump, "%d - %d = %d \n", (int)digit1, (int)digit2, (int)result);
			WriteConsole((uint8_t*)stringDump);
			//END PRINT TO CONSOLE

			//Success return code
			return 1;

			//Function works with floating point numbers
		}else if(IsFloatNumber(ArgStrings[0]) && IsFloatNumber(ArgStrings[1])){
			//Printing output
			//Need to use specific casting functions to get numbers out of strings

			float digit1 = 0;
			float digit2 = 0;

			sscanf((char*)ArgStrings[0], "%f", &digit1);
			sscanf((char*)ArgStrings[1], "%f", &digit2);

			float result = digit1 - digit2;
			*out = result;

			//PRINT TO CONSOLE
			sprintf(stringDump, "%f - %f = %f \n", digit1, digit2, result);
			WriteConsole((uint8_t*)stringDump);
			//END PRINT TO CONSOLE

			//Success return code
			return 1;
		}else{

			//PRINT TO CONSOLE
			sprintf(stringDump, RED "Arguments are not numbers.\nUsage: help sub\n" RESET);
			WriteConsole((uint8_t*)stringDump);
			//END PRINT TO CONSOLE

			//Error return code
			return 0;
		}
	}else{

		//PRINT TO CONSOLE
		sprintf(stringDump, RED "The number of arguments is not correct.\nUsage: help sub\n" RESET);
		WriteConsole((uint8_t*)stringDump);
		//END PRINT TO CONSOLE

		//Error return code
		return 0;
	}
}

/*
 * Function DebugFunction
 * Implements the debug command
 * Turns debug mode on and off, and prints a confirmation message
 * Returns 1 if successful, 0 otherwise
 */
int8_t DebugFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out){

	//Function works only with 1 argument
	if(ArgNum == 1){

		//Debug messages display depends on DebugLevel variable
		if(DebugLevel){
			printf(GRN "Correct argument number detected : %d \n" RESET, ArgNum);
		}

		//Function works with only "on" of "off" arguments
		if(!strcmp((char*)ArgStrings[0], "on")){

			DebugLevel = 1;
			//PRINT TO CONSOLE
			sprintf(stringDump, GRN "Debug ON\n" RESET);
			WriteConsole((uint8_t*)stringDump);
			//END PRINT TO CONSOLE

			//Success return code
			return 1;

			//Function works with only "on" of "off" arguments
		}else if(!strcmp((char*)ArgStrings[0], "off")){
			//Printing output

			DebugLevel = 0;
			//PRINT TO CONSOLE
			sprintf(stringDump, YEL "Debug OFF\n" RESET);
			WriteConsole((uint8_t*)stringDump);
			//END PRINT TO CONSOLE

			//Success return code
			return 1;
		}else{
			//PRINT TO CONSOLE
			sprintf(stringDump, RED "Argument not recognized.\nUsage: help debug\n" RESET);
			WriteConsole((uint8_t*)stringDump);
			//END PRINT TO CONSOLE
			//Error return code
			return 0;
		}
	}else{
		//PRINT TO CONSOLE
		sprintf(stringDump, RED "The number of arguments is not correct.\nUsage: help debug\n" RESET);
		WriteConsole((uint8_t*)stringDump);
		//END PRINT TO CONSOLE
		//Error return code
		return 0;
	}
}

/*
 * Function HelpFunction
 * Implements the help command
 * Displays help message corresponding to a specific command
 * Returns 1 if successful, 0 otherwise
 */
int8_t HelpFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out){

	//Function works only with 1 argument
	if(ArgNum == 1){

		//Debug messages display depends on DebugLevel variable
		if(DebugLevel){
			sprintf(stringDump, GRN "%d arguments correctly detected.\n" RESET, ArgNum);
			WriteConsole((uint8_t*)stringDump);
		}

		//Search for the command being asked for help
		const command_s* Command_p = GetCommandByName((int8_t*)ArgStrings[0]);

		if(Command_p == NULL){
			sprintf(stringDump, RED "Command \"%s\" not found.\n" RESET, ArgStrings[0]);
			WriteConsole((uint8_t*)stringDump);
			return 0;
		}else{

			//Print the actual function's help message
			sprintf(stringDump, CYN "Usage: %s \n" RESET, (char*)Command_p->HelpString);
			WriteConsole((uint8_t*)stringDump);
			return 1;
		}

	}else{
		sprintf(stringDump, RED "The number of arguments is not correct. Usage: help help" RESET);
		WriteConsole((uint8_t*)stringDump);
		//Error return code
		return 0;
	}
}

/*
 * Function AddFunction
 * Implements the add command
 * Prints the result of the addition of several integers or floating point numbers
 * Returns 1 if successful, 0 otherwise
 */
int8_t AddFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out){
	float result = 0;

	//digit stands for the current argument being processed
	float digit = 0;
	int8_t IsFloat = 1;
	int i = 0;

	//While we still have float numbers and not reached end of arguments
	while(IsFloat && i < ArgNum){
		//Verify that current argument is a float number
		if(IsFloatNumber(ArgStrings[i])){
			digit=0;
			//Scan to float and add to result
			sscanf((char*)ArgStrings[i], "%f", &digit);
			result += digit;
			i++;
		}else{
			IsFloat=0;
		}
	}

	if(IsFloat){
		//Return result
		printf("%f\n", result);
		*out = result;
		return 1;
	}else{
		printf(RED "Arguments are not numbers.\nUsage: help add\n" RESET);
		//Error return code
		return 0;
	}

}

/*
 * Function DivFunction
 * Implements the div command
 * Prints the result of a division between two integers or two floating point numbers
 * Returns 1 if successful, 0 otherwise
 */
int8_t DivFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out){

	//Function works only with two arguments
	if(ArgNum == 2){
		//Debug messages display depends on DebugLevel variable
		if(DebugLevel){
			printf(GRN "Correct argument number detected : %d \n" RESET, ArgNum);
		}

		//Function works with floating point numbers
		if(IsFloatNumber(ArgStrings[0]) && IsFloatNumber(ArgStrings[1])){
			//Need to use specific functions to get numbers out of strings

			float digit1 = 0;
			float digit2 = 0;

			sscanf((char*)ArgStrings[0], "%f", &digit1);
			sscanf((char*)ArgStrings[1], "%f", &digit2);

			float result = digit1 / digit2;
			*out = result;

			printf("%f %% %f = %f \n", digit1, digit2, result);

			//Success return code
			return 1;
		}else{
			printf(RED "Arguments are not numbers.\nUsage: help div\n" RESET);
			//Error return code
			return 0;
		}
	}else{
		//PRINT TO CONSOLE
		size_t cn = snprintf(NULL, 0, RED "The number of arguments is not correct.\nUsage: help div\n" RESET);
		char  *prt = malloc(cn+1);
		sprintf(prt, RED "The number of arguments is not correct.\nUsage: help div\n" RESET);
		WriteConsole((uint8_t*)prt);
		free(prt);
		//END PRINT TO CONSOLE
		//Error return code
		return 0;
	}
}

/*
 * Function MulFunction
 * Implements the mul command
 * Prints the result of the multiplication of several integers or floating point numbers
 * Returns 1 if successful, 0 otherwise
 */
int8_t MulFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out){
	float result = 1;

	//digit stands for the current argument being processed
	float digit = 0;
	int8_t IsFloat = 1;
	int i = 0;

	//While we still have float numbers and not reached end of arguments
	while(IsFloat && i < ArgNum){
		//Verify it is float number
		if(IsFloatNumber(ArgStrings[i])){
			digit=0;
			//Scan to float and multiply to result
			sscanf((char*)ArgStrings[i], "%f", &digit);
			result *= digit;
			i++;
		}else{
			IsFloat=0;
		}
	}

	if(IsFloat){
		printf("%f\n", result);
		*out = result;
		return 1;
	}else{
		printf(RED "Arguments are not numbers.\nUsage: help mul\n" RESET);
		//Error return code
		return 0;
	}

}

/*
 * Function ListFunction
 * Implements the list command
 * Displays the list of available commands and their help messages
 * Returns 1 if successful, 0 otherwise
 * Assumes the CommandList is properly initialized (ie NULL terminated at least)
 */
int8_t ListFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out){

	//Function works only with 0 arguments
	if(ArgNum == 0){

		//Debug messages display depends on DebugLevel variable
		if(DebugLevel){
			printf(GRN "Correct argument number detected : %d \n" RESET, ArgNum);
		}

		printf("Available commands:\n");

		int i = 0;

		while(CommandList[i].NameString != NULL){
			printf(CYN "- %s: " RESET "%s\n", (char*)CommandList[i].NameString, (char*)CommandList[i].HelpString);
			i++;
		}

		return 1;

	}else{
		//PRINT TO CONSOLE
		size_t cn = snprintf(NULL, 0, RED "The number of arguments is not correct.\nUsage: help list\n" RESET);
		char  *prt = malloc(cn+1);
		sprintf(prt, RED "The number of arguments is not correct.\nUsage: help list\n" RESET);
		WriteConsole((uint8_t*)prt);
		free(prt);
		//END PRINT TO CONSOLE
		//Error return code
		return 0;
	}
}

int8_t ToneFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out){
	if(ArgNum == 3){

		if(DebugLevel){
			sprintf(stringDump, GRN "%d arguments correctly detected.\n" RESET, ArgNum);
			WriteConsole((uint8_t*)stringDump);
			//osDelay(10);
		}

		if(IsNumber(ArgStrings[0]) && IsNumber(ArgStrings[1])){

			if(DebugLevel){
				sprintf(stringDump, GRN "Arguments are numbers.\n" RESET);
				WriteConsole((uint8_t*)stringDump);
			}

			float ToneFrequency = 0;
			int ToneVolume = 50;
			int ToneDuration = 0;
			//int percent = 0;
			int lastPlaybackSecond;

			//Convert arguments into numbers
			sscanf((char*)ArgStrings[0], "%f", (float*)&ToneFrequency);
			sscanf((char*)ArgStrings[1], "%d", (int*)&ToneVolume);
			sscanf((char*)ArgStrings[2], "%d", (int*)&ToneDuration);

			if(DebugLevel){
				sprintf(stringDump, "Typed tone frequency: %f Hz\n", ToneFrequency);
				WriteConsole((uint8_t*)stringDump);
				sprintf(stringDump, "Typed tone volume: %d%%\n", ToneVolume);
				WriteConsole((uint8_t*)stringDump);
				sprintf(stringDump, "Typed tone duration: %d seconds\n", ToneDuration);
				WriteConsole((uint8_t*)stringDump);
			}

			if(ToneFrequency >= 0 &&
					ToneFrequency <= 4000 &&
					ToneVolume >= 0 &&
					ToneVolume <=100 &&
					ToneDuration >= 0){

				/*//Send message to change tone frequency
			osMessagePut (toneFrequencyQueueHandle, ToneFrequency, 0);

			//Send message to change tone volume
			osMessagePut (toneAmplitudeQueueHandle, ToneVolume, 0);

			//Wait a number of seconds specified by ToneDuration and print every percent
			while(percent++ < 100){
				osDelay(floor(ToneDuration*10));
				WriteConsole((uint8_t*)"#");
			}

			continueReadingFlag = 0;
			//Send message to end audio output (put zero frequency)
			osMessagePut (toneFrequencyQueueHandle, 0, 0);

			//Send message to put zero volume
			osMessagePut (toneAmplitudeQueueHandle, 0, 0);*/

				//Convert ToneVolume from percents to 16 bit signed values, maximum is 32,767
				ToneVolume = (int)(float)(((float)ToneVolume/100)*32767);
				//Convert ToneFrequency from Hertz to Radians per second
				ToneFrequency = (float)ToneFrequency *2*3.14;

				if(DebugLevel){
					sprintf(stringDump, "Acquired tone frequency: %f rad/s\n", (float)ToneFrequency);
					WriteConsole((uint8_t*)stringDump);
					sprintf(stringDump, "Acquired numerical tone volume: %d over 32767\n", ToneVolume);
					WriteConsole((uint8_t*)stringDump);
					sprintf(stringDump, "Acquired tone duration: %d seconds\n", ToneDuration);
					WriteConsole((uint8_t*)stringDump);
				}

				//Audio buffers declared as global
				int16_t* previousBufferPtr = audioBuffer01;
				int16_t* currentBufferPtr = audioBuffer01;

				//Load initial buffer with new frequency sine wave
				for (int blah=0;blah<AUDIO_BUFFER_SIZE;blah++)
				{
					currentBufferPtr[blah] = (int16_t)(ToneVolume*sin(((float)ToneFrequency)*(float)blah/(float)SAMPLE_FREQ*2*3.14));
				}

				//Waiting for DMA to be released by another thread
				osMutexWait(DMAControllerMutexHandle, osWaitForever);

				//Signalling that DMA should be activated
				osSignalSet(audioManagerTaskHandle, 0x01);

				//Immediate signal clear to avoid trying to restart the DMA again
				//Not needed as we wait for any signal to be set
				//osSignalClear(audioManagerTaskHandle, 0x01);

				//Initializing the audio duration for timer
				audioSecondsRemaining = ToneDuration;
				lastPlaybackSecond = audioSecondsRemaining;

				sprintf(stringDump, "%d s ... \n", lastPlaybackSecond);
				WriteConsole((uint8_t*)stringDump);

				//Sending the pointer for the initial buffer to the audio manager
				osMessagePut(audioOutputQueueHandle, (int)currentBufferPtr, 0);

				//Starting playback timer
				osTimerStart(audioPlaybackTimerHandle, 1000);

				//The main idea is that the next buffer to play should always be available BEFORE the DMA fires the callback telling it's done with current data
				while(audioSecondsRemaining>0){
					if(previousBufferPtr == audioBuffer01){
						currentBufferPtr = audioBuffer02;
					}else if(previousBufferPtr == audioBuffer02){
						currentBufferPtr = audioBuffer01;
					}

					//Load current buffer with new frequency sine wave
					for (int i=0;i<AUDIO_BUFFER_SIZE;i++)
					{
						currentBufferPtr[i] = (int16_t)(ToneVolume*sin((ToneFrequency)*(float)i/(float)SAMPLE_FREQ*2*3.14));
					}

					//Sending the pointer for the next buffer to the audio manager
					osMessagePut(audioOutputQueueHandle, (int)currentBufferPtr, 0);

					previousBufferPtr = currentBufferPtr;

					if(lastPlaybackSecond != audioSecondsRemaining){
						sprintf(stringDump, "%d s ... \n", (int)audioSecondsRemaining);
						WriteConsole((uint8_t*)stringDump);
						lastPlaybackSecond = audioSecondsRemaining;
					}

					//Wait for the end of playback for the last buffer
					osSemaphoreWait(audioOutputSemHandle, osWaitForever);

				}

				//Audio playback is done. We can now pass a null pointer to stop outputting audio.
				osMessagePut(audioOutputQueueHandle, (int)NULL, 0);
				osTimerStop(audioPlaybackTimerHandle);

				osMutexRelease(DMAControllerMutexHandle);

				return 1;

			}else{
				sprintf(stringDump, RED "One of the arguments is out of bounds.\n0 <= Frequency <= 4000\n0 <= Volume <= 100\n0 <= Duration\n" RESET);
				WriteConsole((uint8_t*)stringDump);
				return 0;
			}
		}else{
			sprintf(stringDump, RED "Arguments are not numbers.\n" RESET);
			WriteConsole((uint8_t*)stringDump);
			return 0;
		}

	}else{
		sprintf(stringDump, RED "The number of arguments is not correct. Usage: help tone.\n" RESET);
		WriteConsole((uint8_t*)stringDump);
		return 0;
	}
}

int8_t cdFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out){

//		if(ArgNum == 1 && (res = f_stat(ArgStrings[0], &fno)) == FR_OK ){
//
//			strcat(ArgString[0], )
//			f_chdir(SD_Path);
//			WriteConsole((uint8_t*));
//		}



	return 1;
}

int8_t LsFunction(uint8_t ArgNum, uint8_t *ArgStrings[], double* out){

	int error = 0;
	static TCHAR LongFileName[_MAX_LFN];
	static int FolderCount = 0;
	static int FileCount = 0;


	if ((res = f_opendir(&dir, SD_Path )) != FR_OK){                                   //Open the current directory

		WriteConsole((uint8_t*)"ERROR: Opening Directory");
	}
	//res = f_opendir(&dir, //SD_Path ); //Open the current directory
	sprintf(stringDump,"%s\n", SD_Path);
	WriteConsole((uint8_t*)stringDump);

		for(;;){ // Loop as readdir can only read one entry at a time not a whole directory
		//while(res == FR_OK  && fno.lfname[0] != 0){
			fno.lfname = LongFileName;
			fno.lfsize = _MAX_LFN-1;

			res = f_readdir(&dir, &fno);

			if(res !=  FR_OK || fno.lfname[0] == 0) break;

			if(fno.fname[0] == 0 ){
				WriteConsole((uint8_t*)fno.fname);

			}else{
				WriteConsole((uint8_t*)fno.lfname);
			}
			if (fno.fattrib & AM_DIR){ //If it is a directory
				sprintf(stringDump,YEL " (Folder)\n"RESET);
				WriteConsole((uint8_t *)stringDump);
				FolderCount++;
			}else{
				sprintf(stringDump,YEL " (%d bytes)\n"RESET, fno.fsize);
				WriteConsole((uint8_t *)stringDump);
				FileCount++;
			}
		}
		sprintf(stringDump, YEL "Folders: %d\nFiles: %d" RESET, FolderCount, FileCount);
		WriteConsole((uint8_t *)stringDump);
		f_closedir(&dir);
		FolderCount = 0;
		FileCount = 0;

	return 1;
}

