#include "include/aht20.h"

#define aht20_addr 0x38

bool getbit(int NUM, int n){
    if(NUM &(1<<n)){return true;}
    else {return false;}
}


void check_calibration(){
    uint8_t status_byte[1] = {0};
    i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
    i2c_master_start(cmd_handle); //0x71
    i2c_master_write_byte(cmd_handle, (aht20_addr << 1 | I2C_MASTER_READ), true ); //send 0x71
    i2c_master_read_byte(cmd_handle, (uint8_t *) &status_byte, I2C_MASTER_ACK);
    i2c_master_stop(cmd_handle);
    i2c_master_cmd_begin(I2C_NUM_0, cmd_handle, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd_handle);
    if(!(getbit(status_byte[0],3))){
        i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
        i2c_master_start(cmd_handle);
        i2c_master_write_byte(cmd_handle, (aht20_addr << 1 | I2C_MASTER_WRITE), true );
        i2c_master_write_byte(cmd_handle, (0xbe), true);
        i2c_master_write_byte(cmd_handle, (0x08), true);
        i2c_master_write_byte(cmd_handle, (0x00), true);
        i2c_master_stop(cmd_handle);
        i2c_master_cmd_begin(I2C_NUM_0, cmd_handle, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd_handle);
    }
    printf("status %u\n", (uint8_t)status_byte[0]);


        vTaskDelay(100 / portTICK_PERIOD_MS);
}


void trigger_measure(){
     i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
     i2c_master_start(cmd_handle); //0x71
     i2c_master_write_byte(cmd_handle, (aht20_addr << 1 | I2C_MASTER_WRITE), true );
     i2c_master_write_byte(cmd_handle, (0xac), true );
     i2c_master_write_byte(cmd_handle, (0x33), true );
     i2c_master_write_byte(cmd_handle, (0x00), true );
     i2c_master_stop(cmd_handle);
     i2c_master_cmd_begin(I2C_NUM_0, cmd_handle, 800 / portTICK_RATE_MS);
     i2c_cmd_link_delete(cmd_handle);
}

void aht20_read_measures(void *ignore){
    
    vTaskDelay(200 / portTICK_PERIOD_MS);
    uint8_t data[7]= {0,0,0,0,0,0,0};
    while(1){
        trigger_measure();
        i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
        i2c_master_start(cmd_handle);
        i2c_master_write_byte(cmd_handle, (aht20_addr << 1 | I2C_MASTER_READ), true );
        i2c_master_read(cmd_handle, (uint8_t *) &data, 7, I2C_MASTER_ACK);
        i2c_master_stop(cmd_handle);
        i2c_master_cmd_begin(I2C_NUM_0, cmd_handle, 800 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd_handle);
        
        uint32_t rh = ( ((uint32_t)data[1] << 16) | ((uint32_t)data[2] << 8) | (data[3]) ) >> 4 ;
        uint32_t temp = ((uint32_t)(data[3]&0x0F) << 16) | ((uint32_t)data[4] << 8) | (uint32_t)data[5] ;


        aht20_data.temperature = (temp *(0.00019073F)-50);
        aht20_data.rel_humidity = (rh * (0.0000953674316F));
        printf("Temperature is %3.2f\n", (float)temp);
        printf("Humidity is %3.2f\n", (float)rh);
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }

}
