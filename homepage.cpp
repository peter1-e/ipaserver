#include "homepage.h"
#include <QPainter>
#include <QPushButton>
#include <QKeyEvent>
#include <QDebug>
#include "appdata.h"
#include "global.h"

typedef enum {
    BUTTON_TYPE_EXT_INPUT,
    BUTTON_TYPE_MESSAGE,
    BUTTON_TYPE_AUX_INPUT,
    BUTTON_TYPE_MUSIC,
    BUTTON_TYPE_ZONE_CONTROL,
    BUTTON_TYPE_INFORMAINTION,
}button_type_t;

typedef struct {
    button_type_t type;
    QString name;
}button_data_t;

const static QVector<button_data_t> buttons =
{
    {
        BUTTON_TYPE_EXT_INPUT,
        QObject::tr("Mic/Line Input"),
    },
    {
        BUTTON_TYPE_MESSAGE,
        QObject::tr("Message"),
    },
    {
        BUTTON_TYPE_AUX_INPUT,
        QObject::tr("Aux Input Control"),
    },
    {
        BUTTON_TYPE_MUSIC,
        QObject::tr("Music"),
    },
    {
        BUTTON_TYPE_ZONE_CONTROL,
        QObject::tr("Zones control"),
    },

    {
        BUTTON_TYPE_INFORMAINTION,
        QObject::tr("Information"),
    },
};

HomePage::HomePage(QWidget *parent) : QWidget(parent)
{
    setGeometry(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("outline: none");
    setAttribute(Qt::WA_StyledBackground);

    setStyleSheet("QPushButton {\
                      color: #ffffff;\
                      font: 26pt \"思源黑体 CN Regular\";\
                      background-color: #3b4349;\
                      border: none;\
                      text-align : center;\
                    }\
                    QPushButton:checked {\
                        background-color: #1e62a7;\
                    }");

    grid_layout = new QGridLayout(this);
    grid_layout->setMargin(0);
    grid_layout->setHorizontalSpacing(1);
    grid_layout->setVerticalSpacing(1);

    buttonGroup = new QButtonGroup;
    buttonGroup->setExclusive(true);
    for(int i = 0; i < buttons.size(); i++) {
        QPushButton *btn = new QPushButton(this);
        btn->setFixedSize(399, 86);
        btn->setCheckable(true);
        btn->setText(buttons.at(i).name);
       // btn->setProperty("type", buttons.at(i).type);
        buttonGroup->addButton(btn, i);
        grid_layout->addWidget(btn, i/2, i%2 + 1);
    }
    connect(buttonGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(buttonToggled(int, bool)));
}

void HomePage::drawBackground(QPainter *painter)
{
    painter->fillRect(rect(), QBrush(QColor(Qt::white)));
}

void HomePage::paintEvent(QPaintEvent *)
{
    QPainter painter (this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawBackground(&painter);
}

void HomePage::keyPressEvent(QKeyEvent *event)
{
    int checked_id = buttonGroup->checkedId();

    switch (event->key()) {
    case Qt::Key_Left:
        qDebug()<<"Qt::Key_Left";

        if (checked_id > 0) {
            checked_id--;
        } else {
            checked_id = buttonGroup->buttons().size() - 1;
        }
        buttonGroup->buttons().at(checked_id)->setChecked(true);
        break;

    case Qt::Key_Right:
        qDebug()<<"Qt::Key_Right";

        if (checked_id < buttonGroup->buttons().size() - 1) {
            checked_id++;
        } else {
            checked_id = 0;
        }
        buttonGroup->buttons().at(checked_id)->setChecked(true);
        break;

    default:
        break;
    }
}
