#include <QTimer>
#include <QDateTime>
#include <QPainter>
#include <QEvent>
#include "screensaver.h"
#include "global.h"

ScreenSaver::ScreenSaver(QWidget *parent) :
    QWidget(parent)
{
    //setAttribute(Qt::WA_DeleteOnClose);
    setGeometry(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    lbl_date = new QLabel(this);
    lbl_date->setGeometry(100, 30, 600, 40);
    lbl_date->setAlignment(Qt::AlignCenter);
    lbl_date->setStyleSheet("font: 40px \"思源黑体 CN Regular\";\
                             color: rgb(255, 255, 255);");

    lbl_time = new QLabel(this);
    lbl_time->setGeometry(100, 100, 600, 80);
    lbl_time->setAlignment(Qt::AlignCenter);
    lbl_time->setStyleSheet("font: 80px \"思源黑体 CN Regular\";\
                             color: rgb(255, 255, 255);");

    time_clock = new QTimer(this);
    connect(time_clock,SIGNAL(timeout()),this,SLOT(update_time()));
    time_clock->start(500);
}

ScreenSaver::~ScreenSaver()
{

}

void ScreenSaver::update_time()
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString date = datetime.toString("yyyy'/'MM'/'dd  dddd");
    QString time = datetime.toString("hh:mm");
    lbl_date->setText(date);
    lbl_time->setText(time);
}
