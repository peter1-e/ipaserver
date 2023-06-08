#include "mylistwidget.h"
#include <QKeyEvent>
#include <QDebug>

MyListWidget::MyListWidget(QWidget *parent) : QListWidget(parent)
{

}

void MyListWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        if (currentRow() > 0) {
            QListWidget::setCurrentRow(currentRow() - 1);
        }
        break;

    case Qt::Key_Right:
        if (currentRow() < count() - 1) {
            QListWidget::setCurrentRow(currentRow() + 1);
        }
        break;

    case Qt::Key_Return:
        emit itemDoubleClicked(currentItem());
        break;


    default:
        break;
    }
    QListWidget::keyPressEvent(event);
}
