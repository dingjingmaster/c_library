#ifndef WIDGET_H
#define WIDGET_H

#include <QPoint>
#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    bool        mIsDraging = false;
    QPoint      mOffset;
};
#endif // WIDGET_H
