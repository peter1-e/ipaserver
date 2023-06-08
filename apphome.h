#ifndef APPHOME_H
#define APPHOME_H

#include <QWidget>
#include "homepage.h"
#include "filebrower.h"
#include "fileplayer.h"

class AppHome : public QWidget
{
    Q_OBJECT
public:
    explicit AppHome(QWidget *parent = nullptr);

signals:

private:
    HomePage *homepage = nullptr;
    FileBrower *file_brower = nullptr;
    FilePlayer *file_player = nullptr;
};

#endif // APPHOME_H
