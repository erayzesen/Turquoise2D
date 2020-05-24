#include "guidesandgrids.h"

GuidesAndGrids::GuidesAndGrids(GraphicsScene *_scene)
{
    scene=_scene;
    this->setFlag(QGraphicsItem::ItemIsMovable,false);
    this->setFlag(QGraphicsItem::ItemIsSelectable,false);
}

void GuidesAndGrids::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //draw origin grid

}

QRectF GuidesAndGrids::boundingRect() const
{

}

