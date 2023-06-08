#ifndef APPMAIN_H
#define APPMAIN_H

#include <QWidget>
#include "apphome.h"
#include "screensaver.h"

class AppMain : public QWidget
{
    Q_OBJECT
public:
#if 0
    static AppMain* getInstance();
#endif
    explicit AppMain(QWidget *parent = nullptr);
    ~AppMain();
    bool eventFilter(QObject *ob, QEvent *e);
    void init_screensaver();
    void deinit_screensaver();
    void registerMetaType();

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);

signals:
    void signal_screensaver();
    void signal_keypress(int);
    void update_upgrade_progress(unsigned int);

public slots:
    void enable_screensaver();
    void disable_screensaver();
    void enter_screensaver();
    void exit_screensaver();
    void reset_screensaver();
    void app_sleep();
    void app_wakeup();
    void SwitchToAppHome();
    void app_restart_request_handler(int type);

private:
    int read_screen_rect(QRect &rect);
#if 0
    static AppMain* m_instance;
    static QMutex mutex;
    Q_DISABLE_COPY(AppMain)
#endif

private:
    AppHome *apphome = nullptr;
    QTimer *screensaver_timer = nullptr;
    ScreenSaver *screensaver = nullptr;
    QRect screen_rect;
    int ui_offset_x = 0;
    int ui_offset_y = 0;
    int restart_type = 0;
};

#endif // APPMAIN_H
