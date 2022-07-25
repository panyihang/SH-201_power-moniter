#ifndef AHT_20_H
#define AHT_20_H

#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_types.h"

struct aht20_data{
    float temperature;
    float rel_humidity;
}aht20_data;

void check_calibration();
bool getbit(int NUM, int n);
void trigger_measure();
void aht20_read_measures(void *ignore);




#endif