#ifndef INC_ESP32_WIFI_H_
#define INC_ESP32_WIFI_H_


void ESP_init (void);

void ESP_wifiConnect(void);

void ESP_apiConnect(void);

void ESP_apiClose(void);

void ESP_fetchWeatherData(char *date, char *max_temp, char *min_temp, char *avg_temp, char *weather_condition);

void Parse_number(char *temp);

void Parse_text(char *condition);



#endif /* INC_ESP32_WIFI_H_ */
