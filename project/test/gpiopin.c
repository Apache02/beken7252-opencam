#include <rtthread.h>
#include <rtdevice.h>

#define PIN_MIN 0
#define PIN_MAX 39


static void usage(void) {
    rt_kprintf("usage: gpiopin [pin] [0|1]\n");
}

int gpiopin(int argc, char **argv) {
    if (argc != 3) {
        usage();
        return RT_EOK;
    }

    int pin = atoi(argv[1]);
    int value = atoi(argv[2]);

    if (pin > PIN_MAX || pin < PIN_MIN || (value != 0 && value != 1)) {
        rt_kprintf("unsupported pin #%d\n", pin);
        usage();
        return RT_EOK;
    }

    rt_pin_mode(pin, PIN_MODE_OUTPUT);
    rt_kprintf("gpiopin #%d %d\n", pin, value);
    rt_pin_write(pin, value ? PIN_HIGH : PIN_LOW);
}

MSH_CMD_EXPORT(gpiopin, GPIO led pin tester);
