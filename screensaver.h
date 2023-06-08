#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QWidget>
#include <QLabel>

class ScreenSaver : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenSaver(QWidget *parent = nullptr);
    ~ScreenSaver();

private slots:
    void update_time();

private:
    QLabel *lbl_date = nullptr;
    QLabel *lbl_time = nullptr;
    QTimer *time_clock;
};

#endif // SCREENSAVER_H
