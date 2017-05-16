//     $Date: 2017-04-23 00:44:32 +1000 (Sun, 23 Apr 2017) $
// $Revision: 919 $
//   $Author: Peter $

#include "Ass-03.h"
#include "usart.h"


//Build command list
const command_s CommandList[] = {
		{(int8_t*) "sub", &SubFunction, (int8_t*)"sub <num1> <num2>"},
		{(int8_t*) "debug", &DebugFunction, (int8_t*)"debug <on|off>"},
		{(int8_t*) "help", &HelpFunction, (int8_t*)"help <command_name>"},
		{(int8_t*) "add", &AddFunction, (int8_t*)"add <num1> <num2> ... <numN>"},
		{(int8_t*) "div", &DivFunction, (int8_t*)"div <num1> <num2>"},
		{(int8_t*) "mul", &MulFunction, (int8_t*)"mul <num1> <num2>"},
		{(int8_t*) "list", &ListFunction, (int8_t*)"list _no-arguments_"},
		{NULL, NULL, NULL}
};

//Initialize the buffer for typing commands
//Array of chars
int8_t buffer[BUFFER_MAX_SIZE];
//Cursor intended to keep track of the insertion point for new characters coming
int8_t BufferCursor = 0;

void CommandLineParserInit(void)
{
  // Print welcome message
  WriteConsole((uint8_t *)"\014");
  WriteConsole((uint8_t *)"\n>>> Enter a command > ");
}

/*
 * Function CommandLineParserProcess
 * This function is regularly fired to check for user input, either from STDIN or from UART
 * It fills a buffer and, when ENTER is pressed, parses the input and then passes it to the
 * ProcessArgString function
 * Returns nothing and takes no argument
 * Expected to produce terminal output only
 */
void CommandLineParserProcess(void)
{
	uint8_t c;

	//Next section of the code: fill the buffer with received characters, either from UART of from STDIN

	if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK)
	{
		HAL_GPIO_TogglePin(GPIOD, LD4_Pin); // Toggle LED4
		printf("%c",c);

		//Verify first that \n is being passed or not in order not to put it in the buffer (avoids extra processing)
		//13 is ascii code for new line
		if(c == 13){

			if(BufferCursor == BUFFER_MAX_SIZE){//In case of buffer overflow we cannot get the entire command typed
				printf(RED "Buffer overflow! Type a shorter command!\n" RESET);
			}
			else
			{
				//DEBUG CODE
				//printf(CYN"Got the following string \"%s\"\n"RESET,buffer);
				//printf("Buffer: %s\n", buffer);

				//Pass the received string in the buffer to the string parser for word splitting
				char** InputWordsArray;
				int WordCount = string_parser((char*)&buffer, (char***)&InputWordsArray);

				//DEBUG CODE
				//printf("Word count got: %d \n", WordCount);

				//Cleaning buffer for next use: putting spaces in the buffer to make sure last command won't be re-executed
				//Null terminate at the beginning in case of any string-related operations on the buffer
				//Reset the cursor to zero for next buffer-filling
				memset(buffer,' ',sizeof(buffer));
				buffer[0] = '\0';
				BufferCursor = 0;

				//DEBUG CODE
				//for(int i=0; i<WordCount;i++){
				//	printf("Argument number %d: %s\n", i, InputWordsArray[i]);
				//}

				//Process the arguments array and return the result (if any) in the output variable
				double output=0;
				int8_t ErrorCode = ProcessArgString(&output, WordCount, (uint8_t**)InputWordsArray);
				printf("\n>>> Enter a command > ");

				//DEBUG CODE
				//printf("Got the output: %f and error code %d \n", output, ErrorCode);
			}
		}
		else{
			//Buffer filling code
			//The current character is not a new line and can be added to the buffer
			//Pressing ENTER causes the \n character to be passed and the command processing to be fired
			//Do not go beyond buffer capacity

			buffer[BufferCursor] = c;
			BufferCursor++;

			//DEBUG CODE
			//printf("Contents of buffer: %s\n", buffer);
		}

	}
}

/*
 * Function ProcessArgString
 * Processes the array of arguments that has been parsed from the buffer
 * Calls the specified command
 * Returns the result in the output variable being pointed to by the parameter out
 * Returns an error code corresponding to what happened:
 * 0 if no error
 * -1 if command not found
 * -2 if command error
 * -3 if nothing to process
 */
int8_t ProcessArgString(double *out, uint8_t ArgCount, uint8_t *ArgsArray[]){

	int8_t ReturnCode = 0;

	//Handle the case of nothing typed by the user
	//Not to be forgotten because all the following utility-functions work only with non-empty word arrays ...
	if(ArgCount != 0){

		//Retrieve command structure of asked command if it exists
		const command_s* Command_p = GetCommandByName((int8_t*)ArgsArray[0]);

		if(Command_p == NULL){
			printf(RED "\nCommand \"%s\" not found.\n" RESET, ArgsArray[0]);
			ReturnCode = -1;
		}else{
			//Call the actual function implementing the command
			//We remove the first argument which is the command name
			//from the word count and from the array of words being passed
			//so as the words passed are only the command arguments
			int8_t CommandResult = Command_p->Function_p(ArgCount-1, (uint8_t**)ArgsArray+1, out);

			//The command returns an int representing an error code.
			if(CommandResult == 0){
				printf(RED "An error has occurred executing the command.\n" RESET);
				ReturnCode = -2;
			}
		}

	}else{
		printf(RED "Please type something.\n" RESET);
		ReturnCode = -3;
	}
	return ReturnCode;
}

/*
 * Function string_parser
 * Parses a string inp into an array of words
 * Returns the number of words that have been parsed
 * NOTE: this function has not been modified since Assignment1
 * (Copied from c3276757 Ass-01)
 */
int string_parser(char *inp, char **array_of_words[])
{
	//printf("\n========== Begin parsing \"%s\" ... ==========\n", inp);

	size_t inputStringLen = strlen(inp);
	int wordCount = 0;
	//printf("This string is %lu bytes long\n", inputStringLen);

	if(inputStringLen){
		//printf(GRN "This string is not empty. There is something to process ...\n" RESET);

		/*
		 * FIRST PART
		 * Determining the number of words in the input string.
		 * This is required to allocate memory for the array of pointers.
		 */

		//Allocating memory for the first copy of the string
		char * copy1 = malloc(strlen(inp)+1);
		strcpy(copy1, inp);

		/*
		 * Following part from :
		 * http://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
		 */

		// Returns first token
		char *token1 = strtok(copy1, " ");

		// Keep printing tokens while one of the
		// delimiters present in copy1[].
		while (token1 != NULL)
		{
			//printf("%s\n", token1);
			token1 = strtok(NULL, " ");
			wordCount++;
		}

		//Allocating memory for the second copy of the string
		char * copy2 = malloc(strlen(inp)+1);
		strcpy(copy2, inp);

		//Allocating memory for the pointer array
		*array_of_words = malloc(wordCount*sizeof(char*));

		// Returns first token
		char *token2 = strtok(copy2, " ");
		int wordIndex = 0;

		//printf("OK");

		// Keep printing tokens while one of the
		// delimiters present in copy2[].
		while (token2 != NULL)
		{

			//printf("Populating cell n° %d", wordIndex);

			//Allocating some memory for the word itself
			(*array_of_words)[wordIndex] = malloc(strlen(token2)+1);

			//Copying a part of the "copy2" string, pointed to by token2, into the memory allocated
			//This can be done as strtok replaces by null-termination all spaces matched
			strcpy((*array_of_words)[wordIndex], token2);

			wordIndex++;
			//printf("%s\n", token2);

			//Warning ! This operation must be done at end to ensure the loop test is relevant
			token2 = strtok(NULL, " ");
		}

		//No need for the copies anymore
		free(copy1);
		free(copy2);

		//printf(GRN "String parsing done.\n" RESET);
	}else{
		//printf(RED "This string is empty. There's nothing to do with it. Exiting.\n" RESET);
		wordCount = 0;
	}

	return wordCount;
}

/*
 * Function GetCommandByName
 * Finds the command specified by the string CommandName
 * Returns a pointer to the command_s structure representing the command if found
 * Returns NULL otherwise
 */
const command_s* GetCommandByName(int8_t CommandName[]){
	int i = 0;

	//Rely on NULL termination to check for array end
	//Rely on strcmp to compare the provided name and the names in the command list
	while(CommandList[i].NameString != NULL && strcmp((char*)CommandList[i].NameString, (char*)CommandName)){
		i++;
	}

	//printf("Searching loop ended with i = %d \n", i);

	if(CommandList[i].NameString == NULL){
		return NULL;
	}else{
		return &CommandList[i];
	}
}

/*
 * Function IsNumber
 * Scans a null-terminated string and returns 1 if it contains only digits, 0 otherwise
 * Returns 0 in case the string is not null-terminated.
 */
int8_t IsNumber(uint8_t* string){

	int8_t TheStringContainsOnlyDigits = 1;

	//If we have a true null-terminated string
	if(strlen((char*)string)){

		//Loop over it
		for(int i =0; i < strlen((char*)string); i++){

			//Test each character for digit
			if(!isdigit(string[i])){
				TheStringContainsOnlyDigits = 0;
			}
		}
	}else{
		//Intended to be distinguished from strings that are not number, but integer type does not support NULL values
		//TODO: find a smarter way to return this information
		TheStringContainsOnlyDigits = 0;
	}

	return TheStringContainsOnlyDigits;
}

/*
 * Function IsFloatNumber
 * Scans a null-terminated string and returns 1 if it contains a properly formatted float, 0 otherwise
 * Float: [0-9]*.[0-9]*
 * Returns 0 in case the string is not null-terminated.
 */
int8_t IsFloatNumber(uint8_t* string){

	int8_t TheStringContainsOnlyDigits = 1;
	int8_t NumberOfPointsFound = 0;
	int8_t IsAString = 1;

	//If we have a true null-terminated string
	if(strlen((char*)string)){

		//Loop over it
		for(int i =0; i < strlen((char*)string); i++){
			//Search for any point added: only 0 or 1 point allowed in float numbers
			//Test it first to avoid saying that we found something different than a number
			if(string[i] == '.'){
				NumberOfPointsFound++;
			}else if(!isdigit(string[i])){//Then, test each character for digit
				TheStringContainsOnlyDigits = 0;
			}
		}
	}else{
		IsAString = 0;
	}

	//DEBUG CODE
	//printf("IsAString = %d\n", IsAString);
	//printf("TheStringContainsOnlyDigits = %d\n", IsAString);

	//Return logic intended to distinguish the cases when the string passed
	//is a number or is not a properly formatted string
	//However the integer type does not support NULL value, so we only use 0 or 1
	//TODO: implement a way of returning more information!
	if(IsAString){
		if(TheStringContainsOnlyDigits && NumberOfPointsFound <= 1){
			return 1;
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}
