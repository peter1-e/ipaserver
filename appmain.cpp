#include "appmain.h"
#include <QPainter>
#include <QApplication>
#include <QFile>
#include <QTimer>
#include <QKeyEvent>
#include <QDebug>
#include "appdata.h"
#include "global.h"
#include "system.h"

#ifdef TUYAOS_ENABLE
#include "tuyaclient.h"
#endif
#if 0
AppMain* AppMain::m_instance = nullptr;//类外初始化静态成员
QMutex AppMain::mutex;

AppMain *AppMain::getInstance()
{
    if(m_instance == nullptr)
    {
        QMutexLocker locker(&mutex);//确保线程安全
        if(m_instance == nullptr)
        {
            m_instance = new AppMain();
        }
    }
    return m_instance;
}
#endif

AppMain::AppMain(QWidget *parent) : QWidget(parent)
{
    int ret;

    ret = read_screen_rect(screen_rect);
    qDebug("ret is %d\n", ret);
    if(ret == 0) {
        qDebug("x %d, y %d, width %d, height %d\n", screen_rect.x(), screen_rect.y(), screen_rect.width(), screen_rect.height());
        ui_offset_x = screen_rect.x() + (screen_rect.width() - SCREEN_WIDTH)/2;
        ui_offset_y = screen_rect.y() + (screen_rect.height() - SCREEN_HEIGHT)/2;
    }
    setGeometry(QRect(0, 0, SCREEN_WIDTH + 2*ui_offset_x, SCREEN_HEIGHT + 2*ui_offset_y));
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("outline: none");
    setAttribute(Qt::WA_StyledBackground);
    qApp->installEventFilter(this);
    registerMetaType();
    init_screensaver();
    SwitchToAppHome();
}

AppMain::~AppMain()
{
    deinit_screensaver();
}

void AppMain::registerMetaType()
{
//    qRegisterMetaType<fault_type_t >("fault_type_t");
//    qRegisterMetaType<mode_data_t >("mode_data_t");
//    qRegisterMetaType<work_state_t >("work_state_t");
//    qRegisterMetaType<humidity_t >("humidity_t");
}

int AppMain::read_screen_rect(QRect &rect)
{
    QFile file("/sys/class/graphics/fb0/window_axis");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        return -1;
    }
    QString str = file.readAll();
    QStringList list = str.split(" ");
    if(list.length() != 4) {
        return -1;
    }
    rect.setX(list.at(1).toInt());
    rect.setY(list.at(0).toInt());
    rect.setWidth(list.at(3).toInt() - list.at(1).toInt() + 1);
    rect.setHeight(list.at(2).toInt() - list.at(0).toInt() + 1);
    file.close();
    return 0;
}

void AppMain::paintEvent(QPaintEvent *)
{
    QPainter painter (this);
    painter.fillRect(this->rect(), QColor(41, 33, 30));
}

void AppMain::init_screensaver()
{
    if(screensaver_timer == nullptr) {
        screensaver_timer = new QTimer(this);
        connect(screensaver_timer,SIGNAL(timeout()), this, SLOT(enter_screensaver()));
    }
}

void AppMain::deinit_screensaver()
{
    if(screensaver_timer != nullptr) {
        disconnect(screensaver_timer,SIGNAL(timeout()), this, SLOT(enter_screensaver()));
        screensaver_timer->stop();
        delete screensaver_timer;
        screensaver_timer = nullptr;
    }
}

void AppMain::enable_screensaver()
{
    qDebug("AppMain::enable_screensaver()");
    if(screensaver_timer) {
        screensaver_timer->start(SCREENSAVER_TIME_SECONDS * 1000);
    }
}

void AppMain::exit_screensaver()
{
    screensaver->hide();
}

void AppMain::disable_screensaver()
{
    qDebug("AppMain::disable_screensaver()");
    if(screensaver_timer) {
        screensaver_timer->stop();
    }
}

void AppMain::enter_screensaver()
{
    qDebug("AppMain::enter_screensaver()");
    if(screensaver == nullptr) {
        screensaver = new ScreenSaver(this);
        screensaver->setGeometry(QRect(0, 0, 1280, 480));
        screensaver->move(ui_offset_x, ui_offset_y);
        //connect(screensaver, &ScreenSaver::exit, this, &AppMain::exit_screensaver);
    }
    apphome->hide();
    screensaver->show();
}

void AppMain::reset_screensaver()
{
    apphome->show();
    if (screensaver && !screensaver->isHidden()) {
       screensaver->hide();
    }
    if(screensaver_timer != nullptr && screensaver_timer->isActive()) {
        screensaver_timer->stop();
        screensaver_timer->start(SCREENSAVER_TIME_SECONDS * 1000);
    }
}

bool AppMain::eventFilter(QObject *ob, QEvent *e)
{
    if(e->type()==QEvent::KeyPress) {
        if(System::getInstance()->get_system_state() != SYS_STATE_SLEEP) {
            reset_screensaver();
        }
    }
    return QObject::eventFilter(ob,e);
}

void AppMain::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_F1:
        qDebug()<<"Qt::Key_F1";
        break;

    case Qt::Key_F2:
        qDebug()<<"Qt::Key_F2";
        break;

    case Qt::Key_F3:
        qDebug()<<"Qt::Key_F3";
        break;

    case Qt::Key_F6:
        qDebug()<<"Qt::Key_F6";
        if (System::getInstance()->get_system_state() == SYS_STATE_SLEEP) {
            app_wakeup();
        }
        break;

    case Qt::Key_F7:
        qDebug()<<"Qt::Key_F7";
        break;

    case Qt::Key_F8:
        qDebug()<<"Qt::Key_F8";
        break;

    default:
        break;
    }
}

void AppMain::app_sleep()
{
    //System::getInstance()->close_lcd();
    if(apphome != nullptr) {
        apphome->hide();
        disable_screensaver();
    }
}

void AppMain::app_wakeup()
{
    if(apphome != nullptr) {
        apphome->show();
        enable_screensaver();
    }
}

void AppMain::SwitchToAppHome()
{
    if(apphome == nullptr) {
        apphome = new AppHome(this);
        apphome->move(ui_offset_x, ui_offset_y);
        connect(apphome, &AppHome::destroyed, [this]{
            apphome = nullptr;
        });
#if 0
        connect(SignalStation::getInstance(), &SignalStation::coordinate_update, [this](int x, int y){
            apphome->show_tips(QString("x: ") + QString::number(x) + QString(" y: ") + QString::number(y));
        });
        connect(SignalStation::getInstance(), &SignalStation::touch_release, [this]{
            apphome->clear_tips();
        });
#endif
    }
    apphome->show();
    enable_screensaver();
}

void AppMain::app_restart_request_handler(int type)
{
    app_wakeup();
    restart_type = type;
}
