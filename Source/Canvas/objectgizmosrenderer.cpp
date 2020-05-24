#include "objectgizmosrenderer.h"

ObjectGizmosRenderer::ObjectGizmosRenderer(GameItem* _gameItem)
{
    gameItem=_gameItem;
    scene=gameItem->scene;
    this->setParentItem(gameItem);
    this->setFlag(QGraphicsItem::ItemIgnoresTransformations);

}

void ObjectGizmosRenderer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPolygonF points=gameItem->actualBounds();
    painter->setBrush(Qt::NoBrush);
    QPen pen(Qt::white,1,Qt::DashLine,Qt::RoundCap, Qt::RoundJoin);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    painter->drawPolygon(points);
    painter->drawLine(points.at(0),points.at(2));
    painter->drawLine(points.at(1),points.at(3));
}

QRectF ObjectGizmosRenderer::boundingRect() const
{
    return QRectF(0,0,0,0);
}

