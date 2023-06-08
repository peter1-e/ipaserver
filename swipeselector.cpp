#include "swipeselector.h"
#include "global.h"

SwipeSelector::SwipeSelector(QWidget *parent)
            : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setStyleSheet("background: transparent;border:0px");
    setScene(&m_scene);
    m_pageAnimator.setCurveShape(QTimeLine::EaseInCurve);
    connect(&m_pageAnimator, SIGNAL(frameChanged(int)), SLOT(shiftPage(int)));
    connect(&m_pageAnimator, SIGNAL(finished()), this, SLOT(animator_finished()));
    btn_group = new QButtonGroup();
    btn_group->setExclusive(true);
}

void SwipeSelector::shiftPage(int frame)
{
    centerOn(frame, item_height/2);
}

void SwipeSelector::animator_finished()
{
    //qDebug("SwipeSelector::animator_finished");
}

void SwipeSelector::mousePressEvent(QMouseEvent *event)
{
    //qDebug()<<"mousePressEvent"<<"event pos is "<<event->pos()<<endl;
    LastPoint = event->pos();
    releaseFlag = false;
    is_moving = false;
    QGraphicsView::mousePressEvent(event);
}

void SwipeSelector::mouseReleaseEvent(QMouseEvent *event)
{
    //qDebug()<<"mouseReleaseEvent"<<"event pos is "<<event->pos()<<endl;
    releaseFlag = true;
    //QPoint offset_point = event->pos() - LastPoint;

    if(is_moving) {
        QPointF current_left_point = QGraphicsView::mapToScene(QPoint(0, height()/2));
        int offset;
        if(move_direction == MOVE_DIRCTION_LEFT) {
            offset = (item_width - static_cast<int>(current_left_point.x())%item_width);
        } else {
            offset = - static_cast<int>(current_left_point.x())%item_width;
        }
        touchMoveInertial(offset);
    } else {
        //touchMoveInertial((event->pos().x()/item_width - item_per_page/2)*item_width);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void SwipeSelector::mouseMoveEvent(QMouseEvent *event)
{
    QPointF new_center_point;
    QPointF current_left_point;
    qreal new_left_x;

    if(!releaseFlag) {
        int offset = event->pos().x() - LastPoint.x();
        if(qAbs(offset) > 4) {
            if(offset > 0) {
                move_direction = MOVE_DIRCTION_RIGHT;
            } else {
                move_direction = MOVE_DIRCTION_LEFT;
            }
            current_left_point = QGraphicsView::mapToScene(QPoint(0, height()/2));
            if(swipe_loop) {
                new_left_x = (static_cast<int>(current_left_point.x()) - offset + (item_count - item_per_page)*item_width)%((item_count - item_per_page)*item_width);
            } else {
                new_left_x = current_left_point.x() - offset;
            }
            new_center_point = QPointF(new_left_x + width()/2, height()/2);
            centerOn(new_center_point);
            is_moving = true;
        }
    }
    LastPoint = event->pos();
    QGraphicsView::mouseMoveEvent(event);
}

void SwipeSelector::touchMoveInertial(int offset)
{
    QPointF current_left_point = QGraphicsView::mapToScene(QPoint(0, height()/2));
    int cur_left_item = static_cast<int>(current_left_point.x() + item_width/2)/item_width;
    //qDebug("current_left_point.x() is %f, item_count is %d, item_per_page is %d\n", current_left_point.x(), item_count, item_per_page);
    //qDebug("cur_left_x is %f, offset is %d\n", current_left_point.x(), offset);
    if(swipe_loop) {
        if(cur_left_item == 0 && offset < 0) {
            qDebug()<<"111";
            centerOn(QPointF((item_count - item_per_page/2)*item_width + item_width/2, height()/2));
            current_left_point = QGraphicsView::mapToScene(QPoint(0, height()/2));
        } else if((cur_left_item == (item_count - item_per_page)) && offset > 0) {
            qDebug()<<"222";
            centerOn(QPointF(item_per_page/2*item_width + item_width/2, height()/2));
            current_left_point = QGraphicsView::mapToScene(QPoint(0, height()/2));
        }
    }

    qreal new_left_x = current_left_point.x() + offset;
    if(new_left_x < 0) {
        new_left_x = 0;
    } else if(new_left_x > item_width*(item_count - item_per_page)) {
        new_left_x = item_width*(item_count - item_per_page);
    }
    int current_center_x = static_cast<int>(current_left_point.x() + item_width*item_per_page/2);
    int new_center_x = static_cast<int>(new_left_x + item_width*item_per_page/2);
    if(qAbs(current_center_x - new_center_x) >= item_width) {
        item_move = true;
    } else {
        item_move = false;
    }
    m_pageAnimator.setFrameRange(current_center_x, new_center_x);
    m_pageAnimator.setDuration(qAbs(new_center_x - current_center_x)*2);
    m_pageAnimator.start();
}

void SwipeSelector::pageUp()
{
    touchMoveInertial(-item_per_page*item_width);
}

void SwipeSelector::pageDown()
{
    touchMoveInertial(item_per_page*item_width);
}

void SwipeSelector::setupScene(QButtonGroup *btn_group, int item_width, int item_height, int item_per_page, bool isloop)
{
    this->item_width = item_width;
    this->item_height = item_height;
    this->item_per_page = item_per_page;
    swipe_loop = isloop;

    setFixedSize(item_width*item_per_page, item_height);

    for(int i = 0; i < btn_group->buttons().size(); i++) {
        QGraphicsProxyWidget *widget = m_scene.addWidget(btn_group->buttons().at(i));
        widget->setPos(i*item_width, 0);
        widget->setZValue(1);
        connect(btn_group->buttons().at(i), SIGNAL(clicked(bool)), this, SLOT(on_item_clicked(bool)));
        //connect(btn_group->buttons().at(i), SIGNAL(reclicked()), this, SLOT(on_item_reclicked()));
        connect(btn_group->buttons().at(i), SIGNAL(toggled(bool)), this, SLOT(on_item_toggled(bool)));
    }
    item_count = btn_group->buttons().size();
    m_scene.setSceneRect(0, 0, item_count*this->item_width - 1, this->item_height - 1);
    centerOn(width() / 2, height() / 2);
    qDebug("width is %d, height is %d\n", width(), height());
    qDebug()<<"setupScene item_count "<<item_count;
}

void SwipeSelector::on_item_reclicked()
{
    QAbstractButton* btn = static_cast<QAbstractButton *>(sender());

    emit item_reclicked(btn->property("id").toInt());
    //qDebug()<<"SwipeSelector::on_item_reclicked() "<<btn->text()<<btn->property("id").toInt()<<endl;
}

void SwipeSelector::on_item_clicked(bool value)
{
    Q_UNUSED(value)
}

void SwipeSelector::set_focus_index(int value)
{
    if(value < item_count) {
        focus_index = value;
        qreal new_center_x = focus_index*item_width + item_width/2;
        centerOn(QPointF(new_center_x, height()/2));
        btn_group->buttons().at(focus_index)->setChecked(true);
    }
}

int SwipeSelector::get_focus_index()
{
    return focus_index;
}

void SwipeSelector::reset_default()
{
    set_focus_index(default_item);
}

void SwipeSelector::set_default_item(int id)
{
    default_item = id;
    set_focus_index(default_item);
}

int SwipeSelector::get_current_center_item()
{
    QPointF current_center_point = QGraphicsView::mapToScene(QPoint(width()/2, height()/2));

    return static_cast<int>(current_center_point.x())/item_width;
}
