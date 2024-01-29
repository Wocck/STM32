/**
 * @file 		AHT20.c
 * @brief 		AHT20 sensor driver implementation.
 * @date 		Nov 21, 2023
 * @author 		Wojtek Sekula
 */

/** @defgroup Sensors
 *  This group contains files related to sensor interfacing.
 *  @{
 */
#include <AHT20.h>

#define AHT20_ADDR		0x70

/**
 * @brief Initializes the AHT20 sensor.
 *
 * This function initializes the AHT20 temperature and humidity sensor.
 * It checks the sensor status and, if necessary, sends a calibration command.
 */
void AHT20_init(){
	uint8_t readBufer;
	HAL_Delay(40);
	HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDR, &readBufer, 1, 100);
	if((readBufer & 0x08) == 0x00){
		uint8_t sendBuffer[3]={0xBE, 0x08, 0x00};
		HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDR, sendBuffer, 3, 100);
	}
}

/**
 * @brief Reads temperature and humidity data from the AHT20 sensor.
 *
 * This function issues a read command to the AHT20 sensor and then reads
 * the raw temperature and humidity data. It converts these readings into
 * human-readable format and stores them in the provided float pointers.
 *
 * @param temp Pointer to a float where the temperature value will be stored.
 * @param humid Pointer to a float where the humidity value will be stored.
 */
void AHT20_read(float *temp, float *humid){
	uint8_t sendBuffer[3]={0xAC, 0x33, 0x00};
	uint8_t readBuffer[6];
	HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDR, sendBuffer, 3, 100);
	HAL_Delay(80);
	HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDR, readBuffer, 6, 100);
	if((readBuffer[0] & 0x80)==0x00){
		uint32_t data=0;
		data = ((uint32_t)readBuffer[3]>>4) + ((uint32_t)readBuffer[2]<<4) + ((uint32_t)readBuffer[1]<<12);
		*humid = data*100.0f/(1<<20);

		data = (((uint32_t)readBuffer[3] & 0x0F) <<16) + ((uint32_t)readBuffer[4]<<8) + ((uint32_t)readBuffer[5]);
		*temp = data*200.0f/(1<<20)-50;
	}
}

/** @} */
