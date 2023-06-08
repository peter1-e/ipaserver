#ifndef SWIPESELECTOR_H
#define SWIPESELECTOR_H

#include <QtCore>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QButtonGroup>
#include <QAbstractButton>

class SwipeSelector: public QGraphicsView
{
    Q_OBJECT

    enum{
        MOVE_DIRCTION_LEFT,
        MOVE_DIRCTION_RIGHT,
    };

public:
    SwipeSelector(QWidget *parent = nullptr);
    void setupScene(QButtonGroup *btn_group, int item_width, int item_height, int item_per_page, bool isloop);
    void set_default_item(int id);

private:
    void touchMoveInertial(int offset);

signals:
    void item_reclicked(int id);
    //void item_toggled(int id);

public slots:
    void pageUp();
    void pageDown();
    virtual void on_item_clicked(bool value);
    virtual void on_item_toggled(bool value) = 0;
    virtual void reset_default();

private slots:
    void shiftPage(int frame);
    void on_item_reclicked();
    virtual void animator_finished();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    virtual void initButtonList() = 0;
    void set_focus_index(int value);
    int get_focus_index();
    int get_current_center_item();

private:
    QGraphicsScene m_scene;
    QTimeLine m_pageAnimator;
    bool releaseFlag = true;
    int item_width;
    int item_height;
    int item_count;
    int item_per_page;
    int focus_index;
    int move_direction;
    bool is_moving = false;
    QPoint LastPoint;
    int default_item = 0;
    bool swipe_loop = true;
    bool item_move = false;

protected:
    QButtonGroup *btn_group = nullptr;
};

#endif // SWIPESELECTOR_H
