#include "gpio.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_MAX      10
#define DIRECTION_MAX   48

Gpio::Gpio(int pin)
{
    this->pin = pin;
    gpio_export();
}

Gpio::~Gpio()
{
    gpio_unexport();
}

int Gpio::gpio_export()
{
    char buffer[BUFFER_MAX];
    int len;
    int fd;
    char path[100] = {0};

    memset(path, 0, sizeof(path));
    sprintf(path,"/sys/class/gpio/gpio%d",pin);
    if(access(path, F_OK) != 0) {
        fd = open("/sys/class/gpio/export", O_WRONLY);
        if (fd < 0) {
            fprintf(stderr, "Failed to open export for writing!\n");
            return(-1);
        }

        len = snprintf(buffer, BUFFER_MAX, "%d", pin);
        if(len > 0) {
            if (write(fd, buffer, static_cast<unsigned int>(len)) < 0) {
                fprintf(stderr, "Fail to export gpio!\n");
                return -1;
            }
        }
        close(fd);
    }
    return 0;
}

int Gpio::gpio_unexport()
{
    char buffer[BUFFER_MAX];
    int len;
    int fd;
    char path[100] = {0};

    memset(path, 0, sizeof(path));
    sprintf(path,"/sys/class/gpio/gpio%d",pin);
    if(access(path, F_OK) != 0) {
        fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (fd < 0) {
            fprintf(stderr, "Failed to open unexport for writing!\n");
            return -1;
        }

        len = snprintf(buffer, BUFFER_MAX, "%d", pin);
        if(len > 0) {
            if (write(fd, buffer, static_cast<unsigned int>(len)) < 0) {
                fprintf(stderr, "Fail to unexport gpio!\n");
                return -1;
            }
        }
        close(fd);
    }
    return 0;
}

int Gpio::gpio_direction(int dir)
{
    static const char dir_str[]  = "in\0out";
    char path[DIRECTION_MAX];
    int fd;

    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "failed to open gpio direction for writing!\n");
        return -1;
    }

    if (write(fd, &dir_str[dir == gpio_in ? 0 : 3], dir == gpio_in ? 2 : 3) < 0) {
        fprintf(stderr, "failed to set direction!\n");
        return -1;
    }

    close(fd);
    return 0;
}

int Gpio::gpio_read()
{
    char path[DIRECTION_MAX];
    char value_str[3];
    int fd;

    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "failed to open gpio value for reading!\n");
        return -1;
    }

    if (read(fd, value_str, 3) < 0) {
        fprintf(stderr, "failed to read value!\n");
        return -1;
    }

    close(fd);
    return (atoi(value_str));
}

int Gpio::gpio_write(int value)
{
    static const char values_str[] = "01";
    char path[DIRECTION_MAX];
    int fd;

    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "failed to open gpio value for writing!\n");
        return -1;
    }

    if (write(fd, &values_str[value == gpio_low ? 0 : 1], 1) < 0) {
        fprintf(stderr, "failed to write value!\n");
        return -1;
    }

    close(fd);
    return 0;
}
