#ifndef GPIO_H
#define GPIO_H

#include <QObject>

class Gpio : public QObject
{
    Q_OBJECT

public:
    enum GpioDirection {
        gpio_in = 0,
        gpio_out = 1,
    };

    enum GpioLevel {
        gpio_low = 0,
        gpio_high = 1,
    };

public:
    explicit Gpio(int pin = 0);
    ~Gpio();
    int gpio_export();
    int gpio_unexport();
    int gpio_direction(int dir);
    int gpio_read();
    int gpio_write(int value);

private:
    int pin;
};

#endif // GPIO_H
