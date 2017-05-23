/*
 * Command-functions.c
 *
 *  Created on: 25 avr. 2017
 *      Author: Thomas
 */

#include "Ass-03.h"

//DebugLevel variable is used for the debug mode
uint8_t DebugLevel = 0;
char stringDump3[300] ={0};

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
			sprintf(stringDump3, "%d - %d = %d \n", (int)digit1, (int)digit2, (int)result);
			WriteConsole((uint8_t*)stringDump3);
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
			sprintf(stringDump3, "%f - %f = %f \n", digit1, digit2, result);
			WriteConsole((uint8_t*)stringDump3);
			//END PRINT TO CONSOLE

			//Success return code
			return 1;
		}else{

			//PRINT TO CONSOLE
			sprintf(stringDump3, RED "Arguments are not numbers.\nUsage: help sub\n" RESET);
			WriteConsole((uint8_t*)stringDump3);
			//END PRINT TO CONSOLE

			//Error return code
			return 0;
		}
	}else{

		//PRINT TO CONSOLE
		sprintf(stringDump3, RED "The number of arguments is not correct.\nUsage: help sub\n" RESET);
		WriteConsole((uint8_t*)stringDump3);
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
			sprintf(stringDump3, GRN "Debug ON\n" RESET);
			WriteConsole((uint8_t*)stringDump3);
			//END PRINT TO CONSOLE

			//Success return code
			return 1;

			//Function works with only "on" of "off" arguments
		}else if(!strcmp((char*)ArgStrings[0], "off")){
			//Printing output

			DebugLevel = 0;
			//PRINT TO CONSOLE
			sprintf(stringDump3, YEL "Debug OFF\n" RESET);
			WriteConsole((uint8_t*)stringDump3);
			//END PRINT TO CONSOLE

			//Success return code
			return 1;
		}else{
			//PRINT TO CONSOLE
			sprintf(stringDump3, RED "Argument not recognized.\nUsage: help debug\n" RESET);
			WriteConsole((uint8_t*)stringDump3);
			//END PRINT TO CONSOLE
			//Error return code
			return 0;
		}
	}else{
		//PRINT TO CONSOLE
		sprintf(stringDump3, RED "The number of arguments is not correct.\nUsage: help debug\n" RESET);
		WriteConsole((uint8_t*)stringDump3);
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
			sprintf(stringDump3, GRN "%d arguments correctly detected.\n" RESET, ArgNum);
			WriteConsole((uint8_t*)stringDump3);
		}

		//Search for the command being asked for help
		const command_s* Command_p = GetCommandByName((int8_t*)ArgStrings[0]);

		if(Command_p == NULL){
			sprintf(stringDump3, RED "Command \"%s\" not found.\n" RESET, ArgStrings[0]);
			WriteConsole((uint8_t*)stringDump3);
			return 0;
		}else{

			//Print the actual function's help message
			sprintf(stringDump3, CYN "Usage: %s \n" RESET, (char*)Command_p->HelpString);
			WriteConsole((uint8_t*)stringDump3);
			return 1;
		}

	}else{
		sprintf(stringDump3, RED "The number of arguments is not correct. Usage: help help" RESET);
		WriteConsole((uint8_t*)stringDump3);
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
			sprintf(stringDump3, GRN "%d arguments correctly detected.\n" RESET, ArgNum);
			WriteConsole((uint8_t*)stringDump3);
			//osDelay(10);
		}

		if(IsNumber(ArgStrings[0]) && IsNumber(ArgStrings[1])){

			if(DebugLevel){
				sprintf(stringDump3, GRN "Arguments are numbers.\n" RESET);
				WriteConsole((uint8_t*)stringDump3);
			}

			int ToneFrequency = 0;
			int ToneVolume = 50;
			int ToneDuration = 0;
			int percent = 0;

			//Convert arguments into numbers
			sscanf((char*)ArgStrings[0], "%d", (int*)&ToneFrequency);
			sscanf((char*)ArgStrings[1], "%d", (int*)&ToneVolume);
			sscanf((char*)ArgStrings[2], "%d", (int*)&ToneDuration);

			//Convert ToneVolume from percents to 16 bit signed values, maximum is 32,767
			ToneVolume = (int)(float)(((float)ToneVolume/100)*32767);

			if(DebugLevel){
				sprintf(stringDump3, "Acquired tone frequency: %dHz\n", ToneFrequency);
				WriteConsole((uint8_t*)stringDump3);
				sprintf(stringDump3, "Acquired numerical tone volume: %d over 32767\n", ToneVolume);
				WriteConsole((uint8_t*)stringDump3);
				sprintf(stringDump3, "Acquired tone duration: %d seconds\n", ToneDuration);
				WriteConsole((uint8_t*)stringDump3);
			}

			//Send message to change tone frequency
			osMessagePut (toneFrequencyQueueHandle, ToneFrequency, 0);

			//Send message to change tone volume
			osMessagePut (toneAmplitudeQueueHandle, ToneVolume, 0);

			//Wait a number of seconds specified by ToneDuration and print every percent
			while(percent++ < 100){
				osDelay(floor(ToneDuration*10));
				WriteConsole((uint8_t*)"#");
			}

			//Send message to end audio output (put zero frequency)
			osMessagePut (myQueue01Handle, 0, 0);

			return 1;
		}else{
			sprintf(stringDump3, RED "Arguments are not numbers.\n" RESET);
			WriteConsole((uint8_t*)stringDump3);
			return 0;
		}

	}else{
		sprintf(stringDump3, RED "The number of arguments is not correct. Usage: help tone.\n" RESET);
		WriteConsole((uint8_t*)stringDump3);
		return 0;
	}
}


