#include "main.h"
#include "app_error.h"
#include "app_timer.h"
#include "app_util_platform.h"
#include "boards.h"
#include "led_softblink.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "sdk_errors.h"
#include <stdbool.h>
#include <stdint.h>

#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "sine_model_data.h"
#include "tensorflow/lite/experimental/micro/testing/micro_test.h"

/**
 * @brief Function for starting lfclk needed by APP_TIMER.
 */
static void lfclk_init(void) {
    uint32_t err_code;
    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
}

/**
 * @brief Function for application main entry.
 */
int main(void) {
    ret_code_t err_code;

    lfclk_init();

    // Start APP_TIMER to generate timeouts.
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    const led_sb_init_params_t led_sb_init_param = LED_SB_INIT_DEFAULT_PARAMS(LEDS_MASK);

    err_code = led_softblink_init(&led_sb_init_param);
    APP_ERROR_CHECK(err_code);

    err_code = led_softblink_start(LEDS_MASK);
    APP_ERROR_CHECK(err_code);

    tflite::ErrorReporter *error_reporter = nullptr;
    const tflite::Model *model = nullptr;
    tflite::MicroInterpreter *interpreter = nullptr;
    TfLiteTensor *model_input = nullptr;
    int input_length;

    // Create an area of memory to use for input, output, and intermediate arrays.
    // The size of this will depend on the model you're using, and may need to be
    // determined by experimentation.
    constexpr int kTensorArenaSize = 60 * 1024;
    uint8_t tensor_arena[kTensorArenaSize];

    // Whether we should clear the buffer next time we fetch data
    bool should_clear_buffer = false;

    model = ::tflite::GetModel(g_sine_model_data);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        error_reporter->Report(
            "Model provided is schema version %d not equal "
            "to supported version %d.\n",
            model->version(), TFLITE_SCHEMA_VERSION);
    }

    while (true) {
        __WFE();
    }
}

/** @} */