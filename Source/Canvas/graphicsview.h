#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H
#include "QWidget"
#include "QGraphicsView"
#include "QGraphicsScene"
#include "graphicsscene.h"

class GraphicsView : public QGraphicsView
{
public:
    explicit GraphicsView(QWidget* parent=0);
    void setScale(float w,float h);
    QCursor activeCursor;
    void setMouseCursor(QCursor cursor);
protected:
    void enterEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // GRAPHICSVIEW_H
