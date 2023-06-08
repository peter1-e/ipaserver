#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QGridLayout>
#include <QButtonGroup>
#include "filebrower.h"

class HomePage : public QWidget
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *);
    void drawBackground(QPainter *painter);
    void keyPressEvent(QKeyEvent *event);

signals:
    void SwitchToPowerSetting();
    void signal_screensaver();
    void signal_keypress(int);
    void lock_clicked();
    void setting_clicked();
    void factory_clicked();

public slots:


private:
    QColor m_background;
    QGridLayout *grid_layout = nullptr;
    QButtonGroup *buttonGroup = nullptr;
    FileBrower *file_brower = nullptr;
};

#endif // HOMEPAGE_H
