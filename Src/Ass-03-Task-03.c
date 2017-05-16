//     $Date: 2017-05-16 08:00:09 +1000 (Tue, 16 May 2017) $
// $Revision: 971 $
//   $Author: Peter $

#include "Ass-03.h"

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE
//

uint8_t myReadFile();

FATFS SDFatFs;
FIL MyFile;
HAL_SD_ErrorTypedef Status;

void Ass_03_Task_03(void const * argument)
{
  FRESULT res;
  HAL_StatusTypeDef status;
  uint8_t c; // [2];
  uint8_t ReadFlag=1;
  uint32_t loop=0;
  uint8_t s[100];

  WriteConsole((uint8_t *)"Hello from Task 3 (press any key)\n");

  // Check if SD card driver available
  if(retSD != 0)
  {
    WriteConsole((uint8_t *)"ERROR: SD card driver not available.");
    ReadFlag = 0;
  }
  else
  {
    WriteConsole((uint8_t *)"SD card driver available.\n");

    // Mount file system
    if((res = f_mount(&SDFatFs, (TCHAR const *)SD_Path, 0)) != FR_OK)
    {
      WriteConsole((uint8_t *)"ERROR: Could not mount file system.\n");
      ReadFlag = 0;
    }
    WriteConsole((uint8_t *)"Mounted file system: ");
    WriteConsole((uint8_t *)SD_Path);
    WriteConsole((uint8_t *)"\n");
  }

  while (1)
  {
      ReadConsole(&c);
      sprintf(s,"Task 3: %d (got '%c')",loop,c);
      osMutexWait(myMutex01Handle, osWaitForever);
      BSP_LCD_DisplayStringAt(5,220, s, LEFT_MODE);
      osMutexRelease(myMutex01Handle);
      HAL_GPIO_TogglePin(GPIOD, LD3_Pin); // Toggle LED3
      loop++;

      myReadFile();
  }
}

uint8_t myReadFile()
{
  FRESULT res;
  uint32_t bytesread;
#define BUFF_SIZE 256
  uint8_t rtext[BUFF_SIZE];

  // Open file Hello.txt
  if((res = f_open(&MyFile, "Hello.txt", FA_READ)) != FR_OK)
  {
    WriteConsole((uint8_t *)"ERROR: Opening 'Hello.txt'\n");
    return 1;
  }
  WriteConsole((uint8_t *)"Opened file 'Hello.txt'\n");

  // Read data from file
  if ((res = f_read(&MyFile, rtext, BUFF_SIZE-1, &bytesread)) != FR_OK)
  {
    WriteConsole((uint8_t *)"ERROR: Reading 'Hello.txt'\n");
    f_close(&MyFile);
    return 1;
  }
  rtext[bytesread] = '\0';
  WriteConsole((uint8_t *)"Read: ");
  WriteConsole(rtext);
  WriteConsole((uint8_t *)"\n");

  // Close file
  f_close(&MyFile);

  // Unmount file system
  // FATFS_UnLinkDriver(SD_Path);

  return 0;
}
