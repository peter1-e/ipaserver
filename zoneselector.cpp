#include "zoneselector.h"
#include <QKeyEvent>
#include <QDebug>
#include <QApplication>

#define ZONE_COUNT 16
#define ITEM_PER_PAGE 8

#define ITEM_WIDTH 100
#define ITEM_HEIGHT 100

#define DEFAULT_FOCUS_ITEM  4

ZoneSelector::ZoneSelector(QWidget *parent) : SwipeSelector(parent)
{
    setGeometry(0, 0, ITEM_PER_PAGE*ITEM_WIDTH, ITEM_HEIGHT);

    initButtonList();
    setupScene(btn_group, ITEM_WIDTH, ITEM_HEIGHT, ITEM_PER_PAGE, false);
    //set_default_item(DEFAULT_FOCUS_ITEM);
    btn_group->setExclusive(false);
}

ZoneSelector::~ZoneSelector()
{

}

void ZoneSelector::initButtonList()
{
    for (int i = 0; i < ZONE_COUNT; i++) {
        QPushButton *button = new QPushButton();
        button->setCheckable(true);
        button->setFixedSize(80, 80);
        button->setText(QString::number(i + 1));
        button->setStyleSheet("QPushButton {\
                          font: 50px \"思源黑体 CN Regular\";\
                          color:white;\
                          text-align: center;\
                          border-width:2;\
                          border-style:solid;\
                          border-color:rgb(128, 125, 120);\
                          border-radius:40px;\
                          background-color:rgb(34, 34, 34);\
                      }\
                      QPushButton:checked {\
                          border: none;\
                          background-color:rgb(255, 88, 18);\
                      }");
        //button->setProperty("id", mode_items.at(i).ck_mode);
        btn_group->addButton(button);
        btn_group->setId(button, static_cast<int>(i));
    }
}

void ZoneSelector::keyPressEvent(QKeyEvent *event)
{
    QAbstractButton *btn = nullptr;

    if (event->key() >= Qt::Key_F1 && event->key() <= Qt::Key_F8) {
        if (event->isAutoRepeat()) {
            qDebug()<<"long press";
        } else {
            //qDebug()<<"not long press";
        }
        btn = btn_group->buttons().at(event->key() - Qt::Key_F1);
        if (btn != nullptr) {
            btn->setChecked(!btn->isChecked());
        }
    } else if (event->key() == Qt::Key_Left) {
        pageUp();
    } else if (event->key() == Qt::Key_Right) {
        pageDown();
    }
    QWidget::keyPressEvent(event);
}

void ZoneSelector::keyReleaseEvent(QKeyEvent *event)
{
    QAbstractButton *btn = nullptr;

    if (event->key() >= Qt::Key_F1 && event->key() <= Qt::Key_F8) {
        if (event->isAutoRepeat()) {
            qDebug()<<"long press";
        } else {
            //qDebug()<<"not long press";
        }
        btn = btn_group->buttons().at(event->key() - Qt::Key_F1);
        if (btn != nullptr) {
            btn->setChecked(!btn->isChecked());
        }
    } else if (event->key() == Qt::Key_Left) {
        pageUp();
    } else if (event->key() == Qt::Key_Right) {
        pageDown();
    }
    QWidget::keyReleaseEvent(event);
}

void ZoneSelector::on_item_toggled(bool value)
{

}
