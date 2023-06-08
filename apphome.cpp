#include "apphome.h"
#include "global.h"

AppHome::AppHome(QWidget *parent) : QWidget(parent)
{
    setGeometry(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("outline: none");
    setAttribute(Qt::WA_StyledBackground);

    homepage = new HomePage(this);
    homepage->show();

//    file_brower = new FileBrower(this);
//    file_brower->show();
    file_player = new FilePlayer(this);
    file_player->show();
}
