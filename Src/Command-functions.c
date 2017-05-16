/*
 * Command-functions.c
 *
 *  Created on: 25 avr. 2017
 *      Author: Thomas
 */

#include "Ass-03.h"

//DebugLevel variable is used for the debug mode
uint8_t DebugLevel = 0;


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

			printf("%d - %d = %d \n", (int)digit1, (int)digit2, (int)result);

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

			printf("%f - %f = %f \n", digit1, digit2, result);

			//Success return code
			return 1;
		}else{
			printf(RED "Arguments are not numbers.\nUsage: help sub\n" RESET);
			//Error return code
			return 0;
		}
	}else{
		printf(RED "The number of arguments is not correct.\nUsage: help sub\n" RESET);
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
			printf(GRN "Debug ON\n" RESET);

			//Success return code
			return 1;

			//Function works with only "on" of "off" arguments
		}else if(!strcmp((char*)ArgStrings[0], "off")){
			//Printing output

			DebugLevel = 0;
			printf(YEL "Debug OFF\n" RESET);

			//Success return code
			return 1;
		}else{
			printf(RED "Argument not recognized.\nUsage: help debug\n" RESET);
			//Error return code
			return 0;
		}
	}else{
		printf(RED "The number of arguments is not correct.\nUsage: help debug\n" RESET);
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
			printf(GRN "Correct argument number detected : %d \n" RESET, ArgNum);
		}

		//Search for the command being asked for help
		const command_s* Command_p = GetCommandByName((int8_t*)ArgStrings[0]);

		if(Command_p == NULL){
			printf(RED "Command \"%s\" not found.\n" RESET, ArgStrings[0]);
			return 0;
		}else{

			//Print the actual function's help message
			printf(CYN "Usage: %s \n" RESET, (char*)Command_p->HelpString);
			return 1;
		}

	}else{
		printf(RED "The number of arguments is not correct.\nUsage: help help\n" RESET);
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
		printf(RED "The number of arguments is not correct.\nUsage: help div\n" RESET);
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
		printf(RED "The number of arguments is not correct.\nUsage: help list\n" RESET);
		//Error return code
		return 0;
	}
}
