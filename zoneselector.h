#ifndef ZONESELECTOR_H
#define ZONESELECTOR_H

#include <QListWidget>
#include <QButtonGroup>
#include <QPushButton>
#include "swipeselector.h"

class ZoneSelector : public SwipeSelector
{
public:
    ZoneSelector(QWidget *parent = nullptr);
    ~ZoneSelector();
    void initButtonList();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

public slots:
    void on_item_toggled(bool value);
};

#endif // ZONESELECTOR_H
