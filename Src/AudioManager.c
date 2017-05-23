/*
 * AudioManager.c
 *
 *  Created on: 23 May 2017
 *      Author: c3276757
 */

#include "Ass-03.h"

#define SAMPLE_FREQ 8000
osEvent event;
int16_t* bufferToPlayPtr = NULL;
int32_t audioSecondsRemaining = 0;

void AudioManager(void const * argument)
{

	while (1)
	{
		//Wait for anyone asking for an audio transmission
		osSignalWait(0, osWaitForever);

		//Just after sending the signal to activate the DMA controller we expect a pointer to a first buffer to process
		event = osMessageGet(audioOutputQueueHandle, osWaitForever);
		if (event.status == osEventMessage)
		{
			bufferToPlayPtr = (int16_t*)event.value.v;
		}

		//Initialize audio transmission with DMA
		HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t *)bufferToPlayPtr, AUDIO_BUFFER_SIZE);
		while(UDA1380_Init() != HAL_OK)
		{
			osThreadYield();
		}
	}
}

// Call back function for DMA controller
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	//We first signal that audio transmission is complete
	osSemaphoreRelease(audioOutputSemHandle);

	//Then we get the pointer to the new audio buffer
	event = osMessageGet(audioOutputQueueHandle, 0);
	if (event.status == osEventMessage)
	{
		bufferToPlayPtr = (int16_t*) event.value.v;
	}

	//Continue only if the instruction was given to do so
	if(bufferToPlayPtr){
		HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t *)bufferToPlayPtr, AUDIO_BUFFER_SIZE);
	}
}
