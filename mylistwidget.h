#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QListWidget>

class MyListWidget : public QListWidget
{
public:
    MyListWidget(QWidget *parent);

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MYLISTWIDGET_H
