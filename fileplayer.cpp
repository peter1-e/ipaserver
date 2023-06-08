#include "fileplayer.h"
#include "global.h"

FilePlayer::FilePlayer(QWidget *parent) : QWidget(parent)
{
    setGeometry(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    setStyleSheet("QLabel {\
                      color: #ffffff;\
                      font: 26pt \"思源黑体 CN Regular\";\
                      background-color: #1e62a7;\
                      border: none;\
                      text-align : center;\
                    }\
                    QListWidget {\
                       color: #ffffff;\
                       font: 26pt \"思源黑体 CN Regular\";\
                       background-color: rgb(255, 255, 255,0);\
                       border: none;\
                    }\
                    QListWidget::item:selected {\
                        background-color: rgb(0, 153, 64);\
                    }");
    layout = new QVBoxLayout(this);
    layout->setMargin(0);

    lbl_title = new QLabel(this);
    lbl_title->setText(tr("Message"));
    lbl_title->setAlignment(Qt::AlignCenter);

    zone_selector = new ZoneSelector(this);

    layout->addWidget(lbl_title);
    layout->addWidget(zone_selector);

}
