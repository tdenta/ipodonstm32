/*
 * ToneGenerator.c
 *
 *  Created on: 21 May 2017
 *      Author: c3276757
 */

//==================================== FILE NOT USED =========================================================================

#include "Ass-03.h"

#define BUFF_SIZE   512
#define SAMPLE_FREQ 8000
#define F0          500
#define A0          10000
int16_t buffer[BUFF_SIZE];
int32_t time = 0;
int32_t ToneDurationGlobal = 0;

void ToneGeneratorTask(int8_t ToneFrequency, int8_t ToneDuration){

	WriteConsole((uint8_t *)"Starting tone generator");
	ToneDurationGlobal = ToneDuration;

	// Initialize the audio data (note that this is very rough as frequency changes!)
	for (int i=0;i<BUFF_SIZE;i++)
	{
		buffer[i] = (int16_t)(A0*sin(ToneFrequency*(float)i/(float)SAMPLE_FREQ*2*3.14));
	}
	HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t *)buffer, BUFF_SIZE);
	while(UDA1380_Init() != HAL_OK)
	{
		osThreadYield();
	}

	while(time<ToneDuration*1000){
		time++;
	}
}

// Call back function for DMA controller
//void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
//{
//	if(time<ToneDurationGlobal*1000){
//		HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t *)buffer, BUFF_SIZE);
//	}else{
//		osThreadTerminate(ToneGeneratorHandle);
//	}
//
//}
