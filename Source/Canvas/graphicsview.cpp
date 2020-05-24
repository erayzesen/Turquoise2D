#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    //QBrush brush(QColor(0x70, 0x80, 0x50, 255));
    //this->setBackgroundBrush(brush);
    this->setMouseTracking(true);
    this->setUpdatesEnabled(true);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

}

void GraphicsView::setScale(float w, float h)
{
    this->scale(w,h);
}

void GraphicsView::setMouseCursor(QCursor cursor)
{
    activeCursor=cursor;
}

void GraphicsView::enterEvent(QEvent *event)
{
    viewport()->setCursor(activeCursor);
    QGraphicsView::enterEvent(event);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    viewport()->setCursor(activeCursor);
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    viewport()->setCursor(activeCursor);
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    viewport()->setCursor(activeCursor);
    QGraphicsView::mouseReleaseEvent(event);
}

