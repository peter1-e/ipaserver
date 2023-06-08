#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>
#include <QMutex>
#include <QString>
#include <QTimer>

#define LED_ID_TANK		1
#define LED_ID_POWER	2

#define LED_STATUS_OFF	0
#define LED_STATUS_ON	1
#define LED_STATUS_FLASH    2
#define LED_STATUS_HALF	3

typedef enum {
    SYS_STATE_SLEEP = 0,
    SYS_STATE_POWERON,
}sys_state_t;

class System : public QObject
{
    Q_OBJECT
public:
    ~System();
    static System *getInstance();

    const char *get_software_version();

    /************现在就要的**********************/
    //显示屏
    void set_brightness(int value);//设置显示屏亮度
    int get_brightness();//获取显示屏亮度
    void open_lcd();//打开显示屏
    void close_lcd();//关闭显示屏
    int read_lcd_status();

    //时间
    time_t get_raw_time();//设置系统时间
    int set_raw_time(time_t rawtime);//获取系统时间

    //系统信息
    QString get_firmware_version();//获取固件版本
    QString get_uuid();//获取涂鸦UUID
    void get_uuid(char *buf, int len);
    QString get_authkey();//获取涂鸦AUTHKEY
    void get_authkey(char *buf, int len);
    void get_auth_code(char *uuid, size_t uuid_len, char *authkey, size_t keylen);
    void get_uuid_authkey(QString &, QString &);

    //led控制
    int led_ctrl(int id, int status);//status 0:off 1:on 2:flash

    //水箱门
    int tank_door_control(int status);
    void tank_door_open();//打开水箱门
    void tank_door_close();//关闭水箱门
    int get_tank_door_state(unsigned int *status);//获取水箱门状态

    //操控m1/m2两个GPIO(产测需求，m1/m2为控制水箱门的GPIO)
    int read_m1_status(unsigned int *status);//读取M1脚的电平
    int write_m2_status(unsigned int status);//设置M2脚的电平

    //mcu加密验证
    int send_validation_data(char *data, int len);//发送加密数据
    int read_validation_result(char **data, int *len);//读取加密结果

    /************可以晚点再做的**********************/
    //存储
    long get_storage_total(); //获取flash总容量
    long get_storage_available();//获取flash可用容量
    long get_storage_used();//获取flash已用容量

    //网络
    QString get_macaddr();//获取MAC地址
    void set_ipaddr(QString value);//设置IP
    QString get_ipaddr();//获取IP
    void set_netmask(QString value);//设置子网掩码
    QString get_netmask();//获取子网掩码
    void set_gateway(QString value);//设置默认网关
    QString get_gateway();//获取默认网关
    void set_dhcp(bool value);//设置DHCP模式
    bool get_dhcp();//获取DHCP模式
    void set_network_state(int state);
    int get_network_state();

    //其他
    void app_restart();//应用重启
    void app_restart_delay(int ms);
    int do_cmd(const char *cmd);//执行shell命令
    int do_cmd(const char *cmd, char **result);//执行shell命令，并返回结果

    int write_node_file(const char *path, const char *data);
    int read_node_file(const char *path, char *buf, int buf_len);

    void set_system_state(sys_state_t state);
    sys_state_t get_system_state();

    void set_delay_restart(bool value);
    bool get_delay_restart();

signals:
    void sleep(bool);

private:
    explicit System(QObject *parent = nullptr);
    static System* m_instance;
    Q_DISABLE_COPY(System)
    int network_state = 0;
    sys_state_t sys_state = SYS_STATE_POWERON;
    QTimer *restart_timer = nullptr;
    bool delay_restart = false;
};

#endif // SYSTEM_H
