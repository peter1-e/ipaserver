#ifndef FILEPLAYER_H
#define FILEPLAYER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QPushButton>
#include "mylistwidget.h"
#include "zoneselector.h"

class FilePlayer : public QWidget
{
    Q_OBJECT
public:
    explicit FilePlayer(QWidget *parent = nullptr);

signals:

public slots:

private:
    QVBoxLayout *layout = nullptr;
    QLabel *lbl_title = nullptr;
    QWidget *ctrl_widget = nullptr;
    QStackedWidget *stacked_widget = nullptr;
    QLabel *lbl_playmode = nullptr;
    QLabel *lbl_volume = nullptr;
    QLabel *lbl_volume_num = nullptr;
    QLabel *lblplaypause = nullptr;
    ZoneSelector *zone_selector = nullptr;
};

#endif // FILEPLAYER_H
