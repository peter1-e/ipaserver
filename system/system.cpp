#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <QApplication>
#include <QProcess>
#include <QDir>
#include <QThread>
#include "system.h"

#define BUF_MAX_LEN         50
#define PATH_BRIGHTNESS     "/sys/class/backlight/aml-bl/brightness"
#define PATH_LCD_ENABLE     "/sys/class/lcd/enable"
#define PATH_DOOR_CTRL          "/sys/class/doorctrl/send"
#define PATH_DOOR_GET_STATE     "/sys/class/doorctrl/recv"
#define PATH_DOOR_SET_SEND_PIN_STATE     "/sys/class/doorctrl/send_pin"
#define PATH_DOOR_GET_RECV_PIN_STATE     "/sys/class/doorctrl/recv_pin"

#define MAINVER 0
#define SUBVER1 0
#define SUBVER2 3

#define STR(s)     #s
//#define VERSION(a,b,c)  "V" STR(a) "." STR(b) "." STR(c) " "__DATE__
#define VERSION(a,b,c)  STR(a) "." STR(b) "." STR(c)

System* System::m_instance = nullptr;//类外初始化静态成员
static QMutex mutex;

System *System::getInstance()
{
    if(m_instance == nullptr)
    {
        QMutexLocker locker(&mutex);//确保线程安全
        if(m_instance == nullptr)
        {
            m_instance = new System();
        }
    }
    return m_instance;
}

System::System(QObject *parent) : QObject(parent)
{
    restart_timer = new QTimer(this);
    restart_timer->setSingleShot(true);
    connect(restart_timer, &QTimer::timeout, [this]{
        app_restart();
    });
}

System::~System()
{

}
const char *System::get_software_version()
{
    return VERSION(MAINVER,SUBVER1,SUBVER2);
}
using namespace std;
void System::set_brightness(int value)
{
    const string devFile{R"(/sys/class/backlight/aml-bl/brightness)"};
    ofstream outfile;
    outfile.open(devFile);
    cout<<devFile<<endl;
    cout<<"value is "<< value <<endl;
    outfile<<value;
    outfile.close();
}
int System::get_brightness()
{
    const string devFile{R"(/sys/class/backlight/aml-bl/actual_brightness)"};
    uint8_t value;
    ifstream infile;
    infile.open(devFile);
    infile >> value;
    infile.close();
    return value;
}

void System::open_lcd()
{
    int lcd_status = read_lcd_status();

    if (lcd_status != 1) {
        QThread::msleep(200);
        const string devFile{R"(/sys/class/lcd/enable)"};
        ofstream outfile;
        outfile.open(devFile);
        outfile << "1";
        outfile.close();
    }
}

void System::close_lcd()
{
    int lcd_status = read_lcd_status();

    if (lcd_status != 0) {
        QThread::msleep(200);
        const string devFile{R"(/sys/class/lcd/enable)"};
        ofstream outfile;
        outfile.open(devFile);
        outfile << "0";
        outfile.close();
    }
}

int System::read_lcd_status()
{
    string lcd_status_string;
    int lcd_status = -1;

    const string devFile{R"(/sys/class/lcd/enable)"};
    ifstream infile;
    infile.open(devFile);
    infile >> lcd_status_string;
    if (lcd_status_string.find("ON") != string::npos) {
        lcd_status = 1;
    } else if (lcd_status_string.find("OFF") != string::npos) {
        lcd_status = 0;
    }
    infile.close();
    return lcd_status;
}

//时间
time_t System::get_raw_time()
{

}
int System::set_raw_time(time_t rawtime)
{

}

//系统信息
QString System::get_firmware_version()
{

}
QString System::get_uuid()
{
    char buf[128];

    memset(buf, 0, sizeof(buf));
    write_node_file("/sys/class/unifykeys/attach", "1");
    write_node_file("/sys/class/unifykeys/name", "iot_UUID");
    read_node_file("/sys/class/unifykeys/read", buf, sizeof(buf) - 1);
    write_node_file("/sys/class/unifykeys/attach", "0");
    return QString(buf);
}

QString System::get_authkey()
{
    char buf[128];

    memset(buf, 0, sizeof(buf));
    write_node_file("/sys/class/unifykeys/attach", "1");
    write_node_file("/sys/class/unifykeys/name", "iot_AUTHKEY");
    read_node_file("/sys/class/unifykeys/read", buf, sizeof(buf) - 1);
    write_node_file("/sys/class/unifykeys/attach", "0");
    return QString(buf);
}

void System::get_uuid(char *buf, int len)
{
    write_node_file("/sys/class/unifykeys/attach", "1");
    write_node_file("/sys/class/unifykeys/name", "iot_UUID");
    read_node_file("/sys/class/unifykeys/read", buf, len);
    write_node_file("/sys/class/unifykeys/attach", "0");
}


void System::get_authkey(char *buf, int len)
{
    write_node_file("/sys/class/unifykeys/attach", "1");
    write_node_file("/sys/class/unifykeys/name", "iot_AUTHKEY");
    read_node_file("/sys/class/unifykeys/read", buf, len);
    write_node_file("/sys/class/unifykeys/attach", "0");
}

void System::get_auth_code(char *uuid, size_t uuid_len, char *authkey, size_t keylen)
{
    char buf[256];
    QStringList strlist;

    memset(buf, 0, sizeof(buf));
    write_node_file("/sys/class/unifykeys/attach", "1");
    write_node_file("/sys/class/unifykeys/name", "iot_AUTHKEY");
    read_node_file("/sys/class/unifykeys/read", buf, sizeof(buf) - 1);
    write_node_file("/sys/class/unifykeys/attach", "0");
    if((strlen(buf) > 0) && (strlen(buf) < sizeof(buf) - 1)) {
        strlist = QString(buf).split(" ");
        if(strlist.size() >= 2) {
            strncpy(uuid, strlist.at(0).toStdString().c_str(), uuid_len);
            strncpy(authkey, strlist.at(1).toStdString().c_str(), keylen);
            //qDebug("buf is %s, uuid is %s, authkey is %s\n", buf, uuid, authkey);
        }
    }
}

void System::get_uuid_authkey(QString &uuid, QString &authkey)
{
    char buf[256];
    QStringList strlist;

    memset(buf, 0, sizeof(buf));
    write_node_file("/sys/class/unifykeys/attach", "1");
    write_node_file("/sys/class/unifykeys/name", "iot_AUTHKEY");
    read_node_file("/sys/class/unifykeys/read", buf, sizeof(buf) - 1);
    write_node_file("/sys/class/unifykeys/attach", "0");
    if((strlen(buf) > 0) && (strlen(buf) < sizeof(buf) - 1)) {
        strlist = QString(buf).split(" ");
        if(strlist.size() >= 2) {
            uuid = strlist.at(0);
            authkey = strlist.at(1);
        }
    }
}

//led控制
int System::led_ctrl(int id, int status)
{
    char buf[64];

    //qDebug("led %d, status %d\n", id, status);
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d %d", id, status);

    write_node_file("/sys/class/touch_ic/touch_ic_i2c/led", buf);
    return 0;
}

int System::tank_door_control(int status)
{
    const string devFile = PATH_DOOR_CTRL;
    ofstream outfile;
    outfile.open(devFile);
    cout<<devFile<<endl;
    cout<<"value is "<< status <<endl;
    outfile<<status;
    outfile.close();
    return 0;
}

//水箱门
void System::tank_door_open()
{
    tank_door_control(1);
}

void System::tank_door_close()
{
    tank_door_control(0);
}

int System::get_tank_door_state(unsigned int *status)
{
    char buf[BUF_MAX_LEN];
    char value_str[5];
    int fd;
#ifdef __ARM
    fd = open(PATH_DOOR_GET_STATE, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Failed to open node for reading! - %s\n", PATH_DOOR_GET_STATE);
        return -1;
    }

    if (read(fd, value_str, 5) < 0) {
        fprintf(stderr, "Fail to read node! - %s\n", PATH_DOOR_GET_STATE);
        return -1;
    }

    close(fd);
    *status = atoi(value_str);
#endif
    return 0;
}

//操控m1/m2两个GPIO(产测需求，m1/m2为控制水箱门的GPIO)
int System::read_m1_status(unsigned int *status)
{
    char value_str[5];
    int fd;
#ifdef __ARM
    fd = open(PATH_DOOR_GET_RECV_PIN_STATE, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Failed to open node for reading! - %s\n", PATH_DOOR_GET_RECV_PIN_STATE);
        return -1;
    }

    if (read(fd, value_str, 5) < 0) {
        fprintf(stderr, "Fail to read node! - %s\n", PATH_DOOR_GET_RECV_PIN_STATE);
        return -1;
    }

    close(fd);
    *status = atoi(value_str);
#endif
    return 0;
}

int System::write_m2_status(unsigned int status)
{
    char buf[BUF_MAX_LEN];
    int fd;
    int len;

#ifdef __ARM
    fd = open(PATH_DOOR_SET_SEND_PIN_STATE, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "Failed to open node for writing! - %s\n", PATH_DOOR_SET_SEND_PIN_STATE);
        return -1;
    }

    len = snprintf(buf, BUF_MAX_LEN, "%d", status);

    if (write(fd, buf, len) < 0) {
        fprintf(stderr, "Fail to write node! - %s\n", PATH_DOOR_SET_SEND_PIN_STATE);
        return -1;
    }

    close(fd);
#endif
    return 0;
}

//mcu加密验证
int System::send_validation_data(char *data, int len)
{

}
int System::read_validation_result(char **data, int *len)
{

}

void System::app_restart()
{
    QString program = QApplication::applicationFilePath();
    QStringList arguments = QApplication::arguments();
    QString workingDirectory = QDir::currentPath();
    QProcess::startDetached(program, arguments, workingDirectory);
    QApplication::exit();
}

void System::app_restart_delay(int ms)
{
    restart_timer->start(ms);
}

int System::write_node_file(const char *path, const char *data)
{
    int fd;

#ifdef __ARM
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "failed to open %s for writing!\n", path);
        return -1;
    }
    if (write(fd, data, strlen(data)) < 0) {
        fprintf(stderr, "failed to write %s!\n", path);
        return -1;
    }
    close(fd);
#endif
    return 0;
}

int System::read_node_file(const char *path, char *buf, int buf_len)
{
    int fd;

#ifdef __ARM
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "failed to open %s for reading!\n", path);
        return -1;
    }

    if (read(fd, buf, buf_len) < 0) {
        fprintf(stderr, "failed to read %s!\n", path);
        return -1;
    }

    close(fd);
#endif
    return 0;
}

void System::set_network_state(int state)
{
    network_state = state;
}

int System::get_network_state()
{
    return network_state;
}

void System::set_system_state(sys_state_t state)
{
    sys_state = state;
    if (sys_state == SYS_STATE_SLEEP) {
        close_lcd();
        //led_ctrl(LED_ID_TANK, LED_STATUS_OFF);
    } else {
        open_lcd();
        led_ctrl(LED_ID_TANK, LED_STATUS_ON);
    }
}

sys_state_t System::get_system_state()
{
    return sys_state;
}

void System::set_delay_restart(bool value)
{
    delay_restart = value;
}

bool System::get_delay_restart()
{
    return delay_restart;
}

int System::do_cmd(const char *cmd)
{
    FILE *fp;

    if ((fp = popen(cmd, "r")) == nullptr) {
        printf("error\n");
        return -1;
    }
    pclose(fp);

    return 0;
}
