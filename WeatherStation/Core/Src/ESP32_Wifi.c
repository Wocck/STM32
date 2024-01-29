/**
 * @file 			ESP8266_HAL.c
 * @brief 			This file contains functions to initialize and communicate with the ESP8266 module
 * @date			Nov 29, 2023
 * @author			Wojciech Sekuła
 *
 * It's main use is for  Wi-Fi connectivity and to fetch weather data from an online API. It includes
 * functions for setting up the Wi-Fi connection, establishing and closing TCP
 * connections, sending HTTP requests, and parsing the received data.
 */

/** @defgroup Communication
 *  This group contains files for communication protocols and interfaces.
 *  @{
 */

#include <stdbool.h>
#include <ctype.h>
#include <ESP32_Wifi.h>
#include <GFX_Functions.h>
#include <ST7735_Tft.h>
#include <UART_Ringbuffer.h>
#include "stdio.h"
#include "string.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

#define wifi_uart &huart1
#define pc_uart &huart2

#define WIFI_SSID "2DA80"
#define WIFI_PASSWORD "7QEQ2WZUADFN"

//#define WIFI_SSID "SamWi"
//#define WIFI_PASSWORD "jesiennewino"

#define API_URL "api.weatherapi.com"
#define API_PORT "80"
#define GET_URL "/v1/forecast.json?key=272b61815a0f4e2ba7c185237242101&q=Warsaw&aqi=no&alerts=no"

/**
 * @brief Initializes the ESP8266 module in single connection mode.
 */
void ESP_init ()
{
	Ringbuf_init();
	Uart_sendstring("AT+CWMODE=1\r\n", wifi_uart);
	if (!Wait_for("OK\r\n", wifi_uart)) {}
	Uart_sendstring("Set CWMODE to 1\r\n\n", pc_uart);
}

/**
 * @brief Connects to the specified Wi-Fi network using the ESP8266 module.
 */
void ESP_wifiConnect()
{
	char cmd[100];
	sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD);
	Uart_sendstring(cmd, wifi_uart);
	if (!Wait_for("OK\r\n", wifi_uart)) {}
	Uart_sendstring("WiFi Connected!\r\n\n", pc_uart);

	Uart_sendstring("AT+CIPMUX=0\r\n", wifi_uart);
	if (!Wait_for("OK\r\n", wifi_uart)) {}
	Uart_sendstring("Disabled multiple connections\r\n\n", pc_uart);
}

/**
 * @brief Establishes a TCP connection with the weather API server.
 */
void ESP_apiConnect()
{
	char cmd[100];
	sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", API_URL, API_PORT);
	Uart_sendstring(cmd, wifi_uart);
	if (!Wait_for("OK\r\n", wifi_uart)) {}
	Uart_sendstring("Established TCP connection with 'api.weatherapi.com'\r\n", pc_uart);
}

/**
 * @brief Closes the TCP connection with the weather API server.
 */
void ESP_apiClose()
{
    Uart_sendstring("AT+CIPCLOSE\r\n", wifi_uart);
    if (!Wait_for("OK\r\n", wifi_uart)) {}
    Uart_sendstring("TCP connection closed\r\n\n", pc_uart);
}

/**
 * @brief Fetches weather data from the API and updates the display.
 * @param date String buffer to store the current date.
 * @param max_temp String buffer to store the maximum temperature.
 * @param min_temp String buffer to store the minimum temperature.
 * @param avg_temp String buffer to store the average temperature.
 * @param weather_condition String buffer to store the weather condition.
 */
void ESP_fetchWeatherData(char *date, char *max_temp, char *min_temp, char *avg_temp, char *weather_condition)
{
	char cmd[100];
	char getRequest[150];

	ESP_apiConnect();
	snprintf(getRequest, sizeof(getRequest), "GET %s HTTP/1.1\r\nHost: api.weatherapi.com\r\n\r\n", GET_URL);
	snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%d\r\n", strlen(getRequest));
	Uart_sendstring(cmd, wifi_uart);
	if (!Wait_for(">", wifi_uart)) {}
	Uart_sendstring(getRequest, wifi_uart);

	while (!(Get_after ("\"localtime\":", 20, date, wifi_uart)));
	while (!(Get_after ("maxtemp_c\":", 5, max_temp, wifi_uart)));
	while (!(Get_after ("mintemp_c\":", 5, min_temp, wifi_uart)));
	while (!(Get_after ("avgtemp_c\":", 5, avg_temp, wifi_uart)));
	while (!(Get_after ("text\":", 50, weather_condition, wifi_uart)));

	Parse_text(date);
	Parse_text(weather_condition);
	Parse_number(max_temp);
	Parse_number(min_temp);
	Parse_number(avg_temp);

	ST7735_WriteString(25, 6, "                ", Font_7x10, WHITE, BLACK);
	ST7735_WriteString(25, 6, date, Font_7x10, ORANGE, BLACK);
	ST7735_WriteString(10, 82, "                         ", Font_7x10, WHITE, BLACK);
	ST7735_WriteString(10, 82, weather_condition, Font_7x10, TURQUOISE, BLACK);
	ESP_apiClose();
}

/**
 * @brief Parses a string to extract a number.
 * @param temp String containing the number to be parsed.
 */
void Parse_number(char *temp){
	int j = 0;
	for (int i = 0; temp[i] != '\0'; i++) {
		if (isdigit((unsigned char)temp[i]) || temp[i] == '.' || temp[i] == '-') {
			temp[j++] = temp[i];
		}
	}
	temp[j] = '\0';
}

/**
 * @brief Parses a string to extract text within quotes.
 * @param text String containing the text to be parsed.
 */
void Parse_text(char *text){
	int j = 0, in_quotes = 0;
	for(int i = 0; text[i] != '\0'; i++) {
		if (text[i] == '\"') {
			if (in_quotes) break;
			in_quotes = 1;
			continue;
		}
		if (in_quotes) {
			text[j++] = text[i];
		}
	}
	text[j] = '\0';
}

/** @} */
