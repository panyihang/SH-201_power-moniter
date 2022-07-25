/* LVGL Example project
 *
 * Basic project to test LVGL on ESP32 based projects.
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "driver/gpio.h"

/* Littlevgl specific */
#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "include/aht20.h"
#include "aht20.c"
#include "background.c"

#define DEFAULT_VREF 1000 //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES 100 //Multisampling

#include "lvgl.h"

#define CONFIG_LV_USE_DEMO_BENCHMARK

#include "lvgl_helpers.h"

#define TAG "demo"
#define LV_TICK_PERIOD_MS 1

LV_IMG_DECLARE(background);

static esp_adc_cal_characteristics_t *adc_chars;

static const adc_channel_t channel = ADC_CHANNEL_7;
static const adc_channel_t channel1 = ADC_CHANNEL_6;
static const adc_bits_width_t width = ADC_WIDTH_BIT_13;

static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_2;

static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);
static void create_demo_application(void);

#define aht20_addr 0x38

static i2c_config_t aht20_i2c_conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = 38, //default pin for SDA
    .scl_io_num = 39, //default pin for SCL
    .sda_pullup_en = GPIO_PULLUP_DISABLE,
    .scl_pullup_en = GPIO_PULLUP_DISABLE,
    .master.clk_speed = 100000};

void aht20_init(gpio_num_t clock, gpio_num_t data)
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    aht20_i2c_conf.sda_io_num = data;
    aht20_i2c_conf.scl_io_num = clock;
}

static void i2c_setup()
{

    i2c_param_config(I2C_NUM_0, &aht20_i2c_conf);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}
float getVoltage()
{
    esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP);

    adc2_config_channel_atten((adc2_channel_t)channel, atten);
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);

    int adc_reading = 0;
    float voltages = 0;

    for (int i = 0; i < NO_OF_SAMPLES; i++)
    {
        adc2_get_raw((adc2_channel_t)channel, width, &adc_reading);
        voltages = voltages + (float)adc_reading;
    }
    return (voltages / NO_OF_SAMPLES / 308.82);
}

float getcurrent()
{
    esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP);

    adc2_config_channel_atten((adc2_channel_t)channel1, atten);
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);

    int adc_reading = 0;
    float voltages = 0;

    for (int i = 0; i < NO_OF_SAMPLES; i++)
    {
        adc2_get_raw((adc2_channel_t)channel1, width, &adc_reading);
        voltages = voltages + (float)adc_reading;
    }
    //return ( ((voltages / NO_OF_SAMPLES) - getVoltage()*314) *0.0004 );
    return (voltages / NO_OF_SAMPLES * 0.000408);
}

void app_main()
{

    xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 2, NULL, 0, NULL, 1);

    // aht20_init(39, 38);
    //i2c_setup();
    //check_calibration();
    //xTaskCreate(&aht20_read_measures, "task_read_ath20", 10096, NULL, 0, NULL);
}

SemaphoreHandle_t xGuiSemaphore;

static lv_style_t small_style, large_style;

static void guiTask(void *pvParameter)
{

    (void)pvParameter;
    xGuiSemaphore = xSemaphoreCreateMutex();

    lv_init();

    lvgl_driver_init();

    lv_color_t *buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1 != NULL);
    static lv_color_t *buf2 = NULL;

    static lv_disp_buf_t disp_buf;

    uint32_t size_in_px = DISP_BUF_SIZE;

    lv_disp_buf_init(&disp_buf, buf1, buf2, size_in_px);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;

    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"};
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    float voltage = getVoltage();
    float current = getcurrent();
    lv_style_init(&small_style);
    lv_style_set_text_font(&small_style, LV_STATE_DEFAULT, &lv_font_montserrat_26);
    lv_style_init(&large_style);
    lv_style_set_text_font(&large_style, LV_STATE_DEFAULT, &lv_font_montserrat_48);

    lv_obj_t *img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &background);
    lv_obj_align(img1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    
    lv_obj_t *label1 = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(label1, LV_LABEL_PART_MAIN, &large_style);
    lv_label_set_long_mode(label1, LV_LABEL_LONG_BREAK);
    lv_label_set_recolor(label1, true);  
    lv_label_set_align(label1, LV_ALIGN_CENTER); 
    lv_label_set_text_fmt(label1, "#00ff00 %00.2f V#\n\n#66ccff %02.2f A#\n\n#ff2200 %04.2f W#", voltage, current, voltage * current);
    lv_obj_set_width(label1, 300);
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 100, -40);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));

        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
            voltage = getVoltage();
            current = getcurrent();
            lv_label_set_text_fmt(label1, "#00ff00 %00.2f V#\n\n#66ccff %02.2f A#\n\n#ff2200 %04.2f W#", voltage, current, voltage * current);
        }
    }

    free(buf1);
    vTaskDelete(NULL);
}

static void lv_tick_task(void *arg)
{
    (void)arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}
