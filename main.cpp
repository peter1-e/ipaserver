#include "appmain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppMain w;
    w.show();
    return a.exec();
}
