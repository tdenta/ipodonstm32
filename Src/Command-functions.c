/*
 * Command-functions.c
 *
 *  Created on: 25 avr. 2017
 *      Author: Thomas
 */

#include "Ass-03.h"
FRESULT res;

//Variables used in the copy function
FIL fsrc, fdst;      /* File objects */

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
int8_t SubFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){

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
			//*out = result;

			//PRINT TO CONSOLE
			sprintf((char*)stringDump, "%d - %d = %d \n", (int)digit1, (int)digit2, (int)result);
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
			//*out = result;

			//PRINT TO CONSOLE
			sprintf((char*)stringDump, "%f - %f = %f \n", digit1, digit2, result);
			WriteConsole((uint8_t*)stringDump);
			//END PRINT TO CONSOLE

			//Success return code
			return 1;
		}else{

			//PRINT TO CONSOLE
			sprintf((char*)stringDump, RED "Arguments are not numbers.\nUsage: help sub\n" RESET);
			WriteConsole((uint8_t*)stringDump);
			//END PRINT TO CONSOLE

			//Error return code
			return 0;
		}
	}else{

		//PRINT TO CONSOLE
		sprintf((char*)stringDump, RED "The number of arguments is not correct.\nUsage: help sub\n" RESET);
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
int8_t DebugFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){

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
			sprintf((char*)stringDump, GRN "Debug ON\n" RESET);
			WriteConsole((uint8_t*)stringDump);
			//END PRINT TO CONSOLE

			//Success return code
			return 1;

			//Function works with only "on" of "off" arguments
		}else if(!strcmp((char*)ArgStrings[0], "off")){
			//Printing output

			DebugLevel = 0;
			//PRINT TO CONSOLE
			sprintf((char*)stringDump, YEL "Debug OFF\n" RESET);
			WriteConsole((uint8_t*)stringDump);
			//END PRINT TO CONSOLE

			//Success return code
			return 1;
		}else{
			//PRINT TO CONSOLE
			sprintf((char*)stringDump, RED "Argument not recognized.\nUsage: help debug\n" RESET);
			WriteConsole((uint8_t*)stringDump);
			//END PRINT TO CONSOLE
			//Error return code
			return 0;
		}
	}else{
		//PRINT TO CONSOLE
		sprintf((char*)stringDump, RED "The number of arguments is not correct.\nUsage: help debug\n" RESET);
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
int8_t HelpFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){

	//Function works only with 1 argument
	if(ArgNum == 1){

		//Debug messages display depends on DebugLevel variable
		if(DebugLevel){
			sprintf((char*)stringDump, GRN "%d arguments correctly detected.\n" RESET, ArgNum);
			WriteConsole((uint8_t*)stringDump);
		}

		//Search for the command being asked for help
		const command_s* Command_p = GetCommandByName((int8_t*)ArgStrings[0]);

		if(Command_p == NULL){
			sprintf((char*)stringDump, RED "Command \"%s\" not found.\n" RESET, ArgStrings[0]);
			WriteConsole((uint8_t*)stringDump);
			return 0;
		}else{

			//Print the actual function's help message
			sprintf((char*)stringDump, CYN "Usage: %s \n" RESET, (char*)Command_p->HelpString);
			WriteConsole((uint8_t*)stringDump);
			return 1;
		}

	}else{
		sprintf((char*)stringDump, RED "The number of arguments is not correct. Usage: help help" RESET);
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
int8_t AddFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){
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
		//*out = result;
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
int8_t DivFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){

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
			//*out = result;

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
int8_t MulFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){
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
		//*out = result;
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
int8_t ListFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){

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

int8_t ToneFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){
	if(ArgNum == 3){

		if(DebugLevel){
			sprintf((char*)stringDump, GRN "%d arguments correctly detected.\n" RESET, ArgNum);
			WriteConsole((uint8_t*)stringDump);
			//osDelay(10);
		}

		if(IsNumber(ArgStrings[0]) && IsNumber(ArgStrings[1])){

			if(DebugLevel){
				sprintf((char*)stringDump, GRN "Arguments are numbers.\n" RESET);
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
				sprintf((char*)stringDump, "Typed tone frequency: %f Hz\n", ToneFrequency);
				WriteConsole((uint8_t*)stringDump);
				sprintf((char*)stringDump, "Typed tone volume: %d%%\n", ToneVolume);
				WriteConsole((uint8_t*)stringDump);
				sprintf((char*)stringDump, "Typed tone duration: %d seconds\n", ToneDuration);
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
					sprintf((char*)stringDump, "Acquired tone frequency: %f rad/s\n", (float)ToneFrequency);
					WriteConsole((uint8_t*)stringDump);
					sprintf((char*)stringDump, "Acquired numerical tone volume: %d over 32767\n", ToneVolume);
					WriteConsole((uint8_t*)stringDump);
					sprintf((char*)stringDump, "Acquired tone duration: %d seconds\n", ToneDuration);
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

				sprintf((char*)stringDump, "%d s ... \n", lastPlaybackSecond);
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
						sprintf((char*)stringDump, "%d s ... \n", (int)audioSecondsRemaining);
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
				sprintf((char*)stringDump, RED "One of the arguments is out of bounds.\n0 <= Frequency <= 4000\n0 <= Volume <= 100\n0 <= Duration\n" RESET);
				WriteConsole((uint8_t*)stringDump);
				return 0;
			}
		}else{
			sprintf((char*)stringDump, RED "Arguments are not numbers.\n" RESET);
			WriteConsole((uint8_t*)stringDump);
			return 0;
		}

	}else{
		sprintf((char*)stringDump, RED "The number of arguments is not correct. Usage: help tone.\n" RESET);
		WriteConsole((uint8_t*)stringDump);
		return 0;
	}
}

int8_t CdFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){
	if(ArgNum == 1){

		if(DebugLevel){
			sprintf((char*)stringDump, GRN "%d arguments correctly detected.\n" RESET, ArgNum);
			WriteConsole((uint8_t*)stringDump);
		}

		osMutexWait(FSMutexHandle, osWaitForever);

		sprintf((char*)stringDump, "Attempting to change directory with argument %s.\n" RESET, ArgStrings[0]);
		WriteConsole((uint8_t*)stringDump);

		//Save the previous path of cwd to be able to go back in hierarchy on screen
		//Creating a buffer for getcwd, forced to use cwd to avoid circular reference
		TCHAR* OldCwdBuffer[PATH_BUFFER_SIZE];
		f_getcwd((TCHAR*)OldCwdBuffer, PATH_BUFFER_SIZE);
		strcpy((char*)pathOfPreviousWorkingDirectory, (char*)OldCwdBuffer);

		//Change working directory
		res = f_chdir((TCHAR*)ArgStrings[0]);

		//Check if it worked
		if(res != FR_OK){
			sprintf((char*)stringDump, RED "FS Error: %s" RESET, fsErrors[res]);
			WriteConsole((uint8_t*)stringDump);
			return 0;
		}else{
			//Verify new current working directory
			//Creating a buffer for getcwd
			TCHAR* CwdBuffer[PATH_BUFFER_SIZE];

			res = f_getcwd((TCHAR*)CwdBuffer, PATH_BUFFER_SIZE);

			osMutexRelease(FSMutexHandle);

			if(res != FR_OK){
				sprintf((char*)stringDump, RED "FS Error while reading new cwd: %s" RESET, fsErrors[res]);
				WriteConsole((uint8_t*)stringDump);
				return 0;
			}

			//Modify the current working directory global variable, used by ls function to populate the file object array
			strcpy((char*)pathOfCurrentWorkingDirectory, (char*)CwdBuffer);
			sprintf((char*)stringDump, GRN "New working directory: %s\n" RESET, (char*)CwdBuffer);
			if(DebugLevel) WriteConsole((uint8_t*)stringDump);
			return 1;
		}

	}else{
		sprintf((char*)stringDump, RED "The number of arguments is not correct. Usage: help cd.\n" RESET);
		WriteConsole((uint8_t*)stringDump);
		return 0;
	}
}

int8_t LsFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){
	if(ArgNum == 0 || ArgNum == 1){

		if(DebugLevel){
			sprintf((char*)stringDump, GRN "0 or 1 argument(s) detected, correct.\n" RESET);
			WriteConsole((uint8_t*)stringDump);
		}

		//Defining the current directory in terms of relative path
		const TCHAR* CwdPath = ".";
		DIR dir;
		//static is required !!!
		static FILINFO fno;
		uint8_t* tempFname[200];
		//Buffer to store long file names
		static TCHAR LongFileNameBuffer[_MAX_LFN];
		//Cursor used to populate the output structure
		int8_t fileStructureCursor = 0;
		int8_t silentMode = 0;

		//Detecting silent mode
		if(ArgNum == 1 && !strcmp("silent", (char*)ArgStrings[0])){
			silentMode = 1;
		}else if(ArgNum == 1){
			sprintf((char*)stringDump, RED "First argument must be \"silent\".\n" RESET);
			WriteConsole((uint8_t*)stringDump);
			return 0;
		}

		osMutexWait(FSMutexHandle, osWaitForever);

		//Opening current directory and checking for success
		res = f_opendir(&dir, CwdPath);
		if(res != FR_OK){
			if(!silentMode) sprintf((char*)stringDump, RED "FS Error: %s" RESET, fsErrors[res]);
			if(!silentMode) WriteConsole((uint8_t*)stringDump);
			return 0;
		}

		if(DebugLevel) WriteConsole((uint8_t*)"Current directory opened.\n");

		for (;;) {

			//Declaring memory for the long file names as the driver does not do it for us
			fno.lfname = LongFileNameBuffer;
			fno.lfsize = _MAX_LFN-1;

			if(DebugLevel) WriteConsole((uint8_t*)"Attempting reading directory record.\n");
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
			if(DebugLevel) WriteConsole((uint8_t*)"Read directory record.\n");

			if (res != FR_OK || fno.fname[0] == 0){
				if(DebugLevel) WriteConsole((uint8_t*)"Breaking on error or end of directory\n");
				break;  /* Break on error or end of dir */
			}

			//Selecting correct string
			//fname always stores the plain old short file name of FAT32, while lfname is populated only if a long file name exists
			if(DebugLevel) WriteConsole((uint8_t*)"Selecting correct string.\n");

			if(fno.lfname[0] != 0){
				if(DebugLevel) WriteConsole((uint8_t*)"Long name detected.\n");
				strcpy((char*)tempFname, (char*)fno.lfname);
				if(DebugLevel) WriteConsole((uint8_t*)"Long name loaded.\n");
			}else{
				if(DebugLevel) WriteConsole((uint8_t*)"Short name detected.\n");
				strcpy((char*)tempFname, (char*)fno.fname);
				if(DebugLevel) WriteConsole((uint8_t*)"Short name loaded.\n");
			}

			if (fno.fattrib & AM_DIR) {                     /*It is a directory*/
				sprintf((char*)stringDump, "| "CYN"Dir"RESET"  |" CYN " %s\n" RESET, (char*)tempFname);
				if(!silentMode) WriteConsole((uint8_t*)stringDump);

				//Populating the output structure. out is a pointer to an array of structures of type FSElement
				//Only in silent mode in order not to break everything if launched from CLI!!
				//TODO: check if allocated space is enough for termination characters!
				if(silentMode){

					if(!strcmp("..", (char*)tempFname)){//If the entry is the parent directory
						//Populate the displayable PathString
						((FSElement*)out)[fileStructureCursor].PathString = malloc(strlen((char*)tempFname)+1);
						strcpy((char*)((FSElement*)out)[fileStructureCursor].PathString, (char*)tempFname);

						//Populate the element type
						((FSElement*)out)[fileStructureCursor].Type = DIRECTORY;

						//Populate the FullPathString
						//Allocate memory for previous base path only as it is the syntax allowed by fatfs to go to a directory with absolute path
						((FSElement*)out)[fileStructureCursor].FullPathString = malloc(strlen((char*)pathOfPreviousWorkingDirectory)+1);
						//Copy base path in allocated space
						strcpy((char*)((FSElement*)out)[fileStructureCursor].FullPathString, (char*)pathOfPreviousWorkingDirectory);

					}else if(strcmp(".", (char*)tempFname)){//If the entry is just a directory, not the current one
						//Populate the displayable PathString
						((FSElement*)out)[fileStructureCursor].PathString = malloc(strlen((char*)tempFname)+1);
						strcpy((char*)((FSElement*)out)[fileStructureCursor].PathString, (char*)tempFname);

						//Populate the element type
						((FSElement*)out)[fileStructureCursor].Type = DIRECTORY;

						//Populate the FullPathString
						//WARNING: different cases apply if the current directory is the root or not

						//Case 1: current directory is root directory
						if(!strcmp(pathOfCurrentWorkingDirectory, "/")){
							//Allocate memory for base path plus file or folder name
							((FSElement*)out)[fileStructureCursor].FullPathString = malloc(strlen((char*)tempFname)+strlen((char*)pathOfCurrentWorkingDirectory)+1);
							//Copy base path in allocated space
							strcpy((char*)((FSElement*)out)[fileStructureCursor].FullPathString, (char*)pathOfCurrentWorkingDirectory);
							//Concatenate file or folder name into base path in allocated space
							strcat((char*)((FSElement*)out)[fileStructureCursor].FullPathString, (char*)tempFname);

						//Case 2: current directory is not root directory
						}else{
							//Allocate memory for base path PLUS SLASH plus file or folder name
							((FSElement*)out)[fileStructureCursor].FullPathString = malloc(strlen((char*)tempFname)+strlen((char*)pathOfCurrentWorkingDirectory)+2);
							//Copy base path in allocated space
							strcpy((char*)((FSElement*)out)[fileStructureCursor].FullPathString, (char*)pathOfCurrentWorkingDirectory);
							//Concatenate slash into base path in allocated space
							strcat((char*)((FSElement*)out)[fileStructureCursor].FullPathString, (char*)"/");
							//Concatenate file or folder name into base path plus slash in allocated space
							strcat((char*)((FSElement*)out)[fileStructureCursor].FullPathString, (char*)tempFname);
						}



					}else if(!strcmp(".", (char*)tempFname)){//If the entry is the current directory
						//Decrement the cursor to compensate increment and fill the spot anyway in the array
						fileStructureCursor--;
					}
				}

			} else {                                        /*It is a file.*/
				sprintf((char*)stringDump, "| "YEL"File"RESET" | %s", (char*)tempFname);
				if(!silentMode) WriteConsole((uint8_t*)stringDump);

				//Populating the output structure. out is a pointer to an array of structures of type FSElement
				//TODO: check if allocated space is enough for termination characters!
				if(silentMode){
					//Populate the displayable PathString
					((FSElement*)out)[fileStructureCursor].PathString = malloc(strlen((char*)tempFname)+1);
					strcpy((char*)((FSElement*)out)[fileStructureCursor].PathString, (char*)tempFname);

					//Populate the element type
					((FSElement*)out)[fileStructureCursor].Type = SINGLE_FILE;

					//Populate the FullPathString
					//Allocate memory for base path plus file or folder name
					((FSElement*)out)[fileStructureCursor].FullPathString = malloc(strlen((char*)tempFname)+strlen((char*)pathOfCurrentWorkingDirectory)+1);
					//Copy base path in allocated space
					strcpy((char*)((FSElement*)out)[fileStructureCursor].FullPathString, (char*)pathOfCurrentWorkingDirectory);
					//Concatenate file or folder name into base path in allocated space
					strcat((char*)((FSElement*)out)[fileStructureCursor].FullPathString, (char*)tempFname);
				}
			}

			if(fno.fsize){
				sprintf((char*)stringDump, " -- " YEL "%d Bytes\n" RESET, (int)fno.fsize);
				if(!silentMode) WriteConsole((uint8_t*)stringDump);
			}
			fileStructureCursor++;

		}
		f_closedir(&dir);
		//NULL-terminating the structure in any case !
		((FSElement*)out)[fileStructureCursor].PathString = NULL;
		((FSElement*)out)[fileStructureCursor].FullPathString = NULL;
		osMutexRelease(FSMutexHandle);
		return 1;

	}else{
		sprintf((char*)stringDump, RED "The number of arguments is not correct. Usage: help mkdir.\n" RESET);
		WriteConsole((uint8_t*)stringDump);
		return 0;
	}
}

int8_t MkdirFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){
	if(ArgNum == 1){

		if(DebugLevel){
			sprintf((char*)stringDump, GRN "%d arguments correctly detected.\n" RESET, ArgNum);
			WriteConsole((uint8_t*)stringDump);
		}

/*		//Creating a copy of the current working directory path, because we are going to build the full path from it and the argument and we don't want to modify the clean current path
		uint8_t* pwdCopy[PATH_BUFFER_SIZE + 100] = {0};
		strcpy((char*)pwdCopy, (char*)pathOfCurrentWorkingDirectory);

		//Concatenate the pwd and the name of the new directory
		strcat((char*)pwdCopy, (char*)ArgStrings[0]);

		//Creating a constant to meet fatfs functions requirements
		const TCHAR* pathOfNewDirectory = (TCHAR*)pwdCopy;

		res = f_mkdir(pathOfNewDirectory);*/

		res = f_mkdir((TCHAR*)ArgStrings[0]);

		if(res != FR_OK){
			sprintf((char*)stringDump, RED "FS Error: %s" RESET, fsErrors[res]);
			WriteConsole((uint8_t*)stringDump);
			return 0;
		}else{
			sprintf((char*)stringDump, GRN "New folder: %s\n" RESET, ArgStrings[0]);
			WriteConsole((uint8_t*)stringDump);
			return 1;
		}

	}else{
		sprintf((char*)stringDump, RED "The number of arguments is not correct. Usage: help mkdir.\n" RESET);
		WriteConsole((uint8_t*)stringDump);
		return 0;
	}
}

int8_t RmFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){
	if(ArgNum == 1){

		if(DebugLevel){
			sprintf((char*)stringDump, GRN "%d arguments correctly detected.\n" RESET, ArgNum);
			WriteConsole((uint8_t*)stringDump);
		}

/*		//Creating a copy of the current working directory path, because we are going to build the full path from it and the argument and we don't want to modify the clean current path
		uint8_t* pwdCopy[PATH_BUFFER_SIZE + 100] = {0};
		strcpy((char*)pwdCopy, (char*)pathOfCurrentWorkingDirectory);

		//Concatenate the pwd and the name of the new directory
		strcat((char*)pwdCopy, (char*)ArgStrings[0]);

		//Creating a constant to meet fatfs functions requirements
		const TCHAR* pathOfNewDirectory = (TCHAR*)pwdCopy;

		res = f_mkdir(pathOfNewDirectory);*/

		//Warning!!!!!!!!!-This function relies on relative paths

		res = f_unlink((TCHAR*)ArgStrings[0]);

		if(res != FR_OK){
			sprintf((char*)stringDump, RED "FS Error: %s" RESET, fsErrors[res]);
			WriteConsole((uint8_t*)stringDump);
			return 0;
		}else{
			sprintf((char*)stringDump, GRN "File/folder deleted: %s\n" RESET, ArgStrings[0]);
			WriteConsole((uint8_t*)stringDump);
			return 1;
		}

	}else{
		sprintf((char*)stringDump, RED "The number of arguments is not correct. Usage: help rm.\n" RESET);
		WriteConsole((uint8_t*)stringDump);
		return 0;
	}
	return 1;

}

/*
 * Function CpFunction
 * Copies a file to a destination
 * Widely inspired by http://elm-chan.org/fsw/ff/doc/open.html
 */
int8_t CpFunction(uint8_t ArgNum, uint8_t *ArgStrings[], void* out){
	if(ArgNum == 2){

		if(DebugLevel){
			sprintf((char*)stringDump, GRN "%d arguments correctly detected.\n" RESET, ArgNum);
			WriteConsole((uint8_t*)stringDump);
		}

/*		//Creating a copy of the current working directory path, because we are going to build the full path from it and the argument and we don't want to modify the clean current path
		uint8_t* pwdCopy[PATH_BUFFER_SIZE + 100] = {0};
		strcpy((char*)pwdCopy, (char*)pathOfCurrentWorkingDirectory);

		//Concatenate the pwd and the name of the new directory
		strcat((char*)pwdCopy, (char*)ArgStrings[0]);

		//Creating a constant to meet fatfs functions requirements
		const TCHAR* pathOfNewDirectory = (TCHAR*)pwdCopy;

		res = f_mkdir(pathOfNewDirectory);*/

		//Warning!!!!!!!!!-This function relies on relative paths

		UINT br, bw;         /* File read/write count */
		BYTE cpbuffer[FILE_BUFFER_SIZE];   /* File copy buffer */

		//Open the file to be copied (source file)
		res = f_open(&fsrc, (TCHAR*)ArgStrings[0], FA_READ);

		//Check if it was successful
		if(res != FR_OK){
					sprintf((char*)stringDump, RED "FS Error on source file: %s" RESET, fsErrors[res]);
					WriteConsole((uint8_t*)stringDump);
					return 0;
		}

		//Create the destination file and open it in write mode
		res = f_open(&fdst, (TCHAR*)ArgStrings[1], FA_CREATE_NEW | FA_WRITE);

		//Check if it was successful
		if(res != FR_OK){
			sprintf((char*)stringDump, RED "FS Error on destination file: %s" RESET, fsErrors[res]);
			WriteConsole((uint8_t*)stringDump);
			return 0;
		}

		/* Copy source to destination */
		for (;;) {
			if(DebugLevel) WriteConsole((uint8_t*)"Reading chunk ...\n");
			res = f_read(&fsrc, cpbuffer, sizeof cpbuffer, &br);  /* Read a chunk of source file */

			if (res != FR_OK || br == 0){
				if(DebugLevel) WriteConsole((uint8_t*)"Breaking on reading error or eof\n");
				break; /* error or eof : exit*/
			}

			if(DebugLevel) WriteConsole((uint8_t*)"Writing chunk ...\n");
			res = f_write(&fdst, cpbuffer, br, &bw);            /* Write it to the destination file */

			if (res != FR_OK || bw < br){
				if(DebugLevel) WriteConsole((uint8_t*)"Breaking on writing error or disk full\n");
				break; /* error or disk full : exit */
			}
		}

		/* Close open files */
		f_close(&fsrc);
		f_close(&fdst);
		if(DebugLevel) WriteConsole((uint8_t*)"Files closed\n");

		if(res == FR_OK){
			sprintf((char*)stringDump, CYN "%s" GRN " successfully copied into" CYN " %s.\n" RESET, ArgStrings[0], ArgStrings[1]);
			WriteConsole((uint8_t*)stringDump);
			return 1;
		}else{
			sprintf((char*)stringDump, RED "FS Error during copy: %s" RESET, fsErrors[res]);
			WriteConsole((uint8_t*)stringDump);
			return 0;
		}

	}else{
		sprintf((char*)stringDump, RED "The number of arguments is not correct. Usage: help cp.\n" RESET);
		WriteConsole((uint8_t*)stringDump);
		return 0;
	}
}

