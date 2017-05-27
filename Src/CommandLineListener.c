//     $Date: 2017-05-02 10:39:11 +1000 (Tue, 02 May 2017) $
// $Revision: 941 $
//   $Author: Peter $

#include "Ass-03.h"

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE
//

uint8_t stringDump[300] = {0};

void CommandLineListener(void const * argument)
{
	if(SDCardInit()){
		WriteConsole((uint8_t*)GRN"SD Card successfully initialized.\n"RESET);
	}else{
		WriteConsole((uint8_t*)RED"SD Card init failed.\n"RESET);
	}

	CommandLineParserInit();

	//Initialize the buffer for typing commands
	//Array of chars
	int8_t buffer[BUFFER_MAX_SIZE] = {0};
	//Cursor intended to keep track of the insertion point for new characters coming
	int8_t BufferCursor = 0;

	while (1)
	{
		uint8_t c;

		//Next section of the code: fill the buffer with received characters from UART

		//ReadConsole is interrupt driven: the thread will stop executing itself here and wait for characters to come (no polling anymore)
		ReadConsole(&c);

		//Print back the character being read
		char prt1[2];

		if ((BufferCursor == 0) && (c == 0x7f))
		{

		}
		else
		{
			sprintf(prt1,"%c",c);
			WriteConsole((uint8_t *)prt1);
		}



		if(BufferCursor == BUFFER_MAX_SIZE-1){//In case of buffer overflow we cannot get the entire command typed

			//PRINT TO CONSOLE
			sprintf(stringDump,RED"\nBuffer overflow! Type a shorter command!\n"RESET);
			WriteConsole((uint8_t*)stringDump);
			//END PRINT TO CONSOLE

			//Cleaning buffer for next use: putting spaces in the buffer to make sure last command won't be re-executed
			//Null terminate at the beginning in case of any string-related operations on the buffer
			//Reset the cursor to zero for next buffer-filling
			memset(buffer,0,BUFFER_MAX_SIZE);
			buffer[0] = '\0';
			BufferCursor = 0;

			sprintf(stringDump, CYN"\n%s" RESET, pathOfCurrentWorkingDirectory);
			WriteConsole((uint8_t*)stringDump);
			WriteConsole((uint8_t *)" > $  ");

			//Verify then that \n is being passed or not in order not to put it in the buffer (avoids extra processing)
			//13 is ascii code for new line
		}else if(c == 13){

			//DEBUG CODE
			//printf(CYN"Got the following string \"%s\"\n"RESET,buffer);
			//printf("Buffer: %s\n", buffer);

			//Pass the received string in the buffer to the string parser for word splitting
			char** InputWordsArray;
			int WordCount = string_parser((char*)&buffer, (char***)&InputWordsArray);

			//DEBUG CODE
			//printf("Word count got: %d \n", WordCount);

			//Cleaning buffer for next use: putting zeroes in the buffer to make sure last command won't be re-executed
			//Null terminate at the beginning in case of any string-related operations on the buffer
			//Reset the cursor to zero for next buffer-filling
			memset(buffer,0,BUFFER_MAX_SIZE);
			buffer[0] = '\0';
			BufferCursor = 0;

			//DEBUG CODE
//			for(int i=0; i<WordCount;i++){
//				sprintf(stringDump,"Argument number %d: %s\n", i, InputWordsArray[i]);
//				WriteConsole((uint8_t*)stringDump);
//				memset(stringDump,0,300);
//			}

			//Process the arguments array and return the result (if any) in the output variable
			double output=0;
			int8_t ErrorCode = ProcessArgString(&output, WordCount, (uint8_t**)InputWordsArray);

			sprintf(stringDump, CYN"\n%s" RESET, pathOfCurrentWorkingDirectory);
			WriteConsole((uint8_t*)stringDump);
			WriteConsole((uint8_t *)" > $  ");

			//DEBUG CODE
			//printf("Got the output: %f and error code %d \n", output, ErrorCode);
		}else if(c == 0x7f){ //Check for backspace and start of buffer

			if (BufferCursor != 0){
				BufferCursor--;
			}
			buffer[BufferCursor] = '\0';

		}else{
			//Buffer filling code
			//The current character is not a new line and can be added to the buffer
			//Pressing ENTER causes the \n character to be passed and the command processing to be fired
			//Do not go beyond buffer capacity

			buffer[BufferCursor] = c;
			BufferCursor++;

			//DEBUG CODE
//			sprintf(stringDump,"Contents of buffer: %s\n", buffer);
//			WriteConsole((uint8_t*)stringDump);
			//memset(stringDump,0,300);
		}
	}
}
