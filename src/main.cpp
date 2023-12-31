#include "esp_log.h"
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "driver/rmt_tx.h"
#include "soc/clk_tree_defs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "led/Strip.h"

#include <cmath>
#include <thread>

const char *TAG = "Main";

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    led::Strip<GPIO_LED, LED_COUNT> led{};
    led.enable();

    ESP_LOGI(TAG, "Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    while (true)
    {
        TickType_t time = xTaskGetTickCount();
        double hue = (std::sin(time / 200.0) * 180) + 180;

        led.color.setFromHSV(hue, 1.0, 0.08); // On the Atom Matrix we need to reduce the value, otherwise we damage components

#ifdef PLOTTING
        printf(">hue:%f\n", hue);
        printf(">red:%u\n", led.color.red);
        printf(">green:%u\n", led.color.green);
        printf(">blue:%u\n", led.color.blue);
#endif

        // send out color
        led.transmit();
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    led.disable();
}