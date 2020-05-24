#include "colliderrenderer.h"
#include "Canvas/graphicsscene.h"

ColliderRenderer::ColliderRenderer(GameItem* _gameItem)
{
    gameItem=_gameItem;
    this->setParentItem(gameItem);
    scene=gameItem->scene;
    //this->setPos(0,0);
    //this->setTransformOriginPoint(0,0);
    this->setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

void ColliderRenderer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(gameItem->enablePyhsics!=true){
        //exit
    }
    //Checking if item is selected.(if item isn't selected, canceling renderer operation)
    if(gameItem->scene->lastTransformItem!=NULL){
        if(gameItem->scene->lastTransformItem->items.indexOf(gameItem)==-1){
            return;
        }
    }else {
        return;
    }
    //painter opacity options
    float activeOpacity=0.2;
    float pasiveOpacity=0.1;
    float opacity;

    for(int i=0;i<gameItem->colliderShapes.count();i++){
        QString shapeType=gameItem->colliderShapes.at(i)->value("type").toString();
        QRectF rect=gameItem->colliderShapes.at(i)->value("rect").toRectF();
        QSizeF s=gameItem->scale; //GameItem scale
        rect.setRect(rect.x()*s.width(),rect.y()*s.height(),rect.width()*s.width(),rect.height()*s.height());
        QRectF settedZeroRect=QRectF(rect.x(),rect.y(),rect.width()/2,rect.height()/2);

        QColor green(Qt::green);
        opacity=pasiveOpacity;
        if(gameItem->scene!=NULL){
            if(scene->lastEditedCollider!=NULL){
                if(scene->lastEditedCollider==gameItem->colliderShapes.at(i)){
                    qDebug()<<"beni editliyore";
                    opacity=activeOpacity;
                }
            }

        }
        if(shapeType=="box"){
            QPolygonF rotatedPoints=MathOperations::rotateRectPoints(rect,QPointF(0,0),gameItem->rotate);
            rotatedPoints=setSceneZoomFactor(rotatedPoints);
            painter->setOpacity(1);
            //drawing frame shape
            painter->setPen(QPen(green));
            painter->setBrush(Qt::NoBrush);
            painter->drawPolygon(rotatedPoints);
            //drawing rectangle shape
            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(green));
            painter->setOpacity(opacity);
            painter->drawPolygon(rotatedPoints);
        }else if(shapeType=="circle"){
            //drawing frame shape
            QPolygonF rotateForCenter=MathOperations::rotatePointGroup(QPolygonF()<<QPointF(settedZeroRect.x(),settedZeroRect.y()),QPointF(0,0),gameItem->rotate);
            rotateForCenter=setSceneZoomFactor(rotateForCenter);
            QPointF rotatedCenter=rotateForCenter.at(0);
            painter->setOpacity(1);
            painter->setPen(QPen(green));
            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(rotatedCenter,settedZeroRect.width(),settedZeroRect.height());
            //drawing rectangle shape
            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(green));
            painter->setOpacity(opacity);
            painter->drawEllipse(rotatedCenter,settedZeroRect.width(),settedZeroRect.height());
        }
    }
}

QRectF ColliderRenderer::boundingRect() const
{
    return QRectF(0,0,0,0);
}

QPolygonF ColliderRenderer::setSceneZoomFactor(QPolygonF points)
{
    QPolygonF resPoints;
    for(int i=0;i<points.count();i++){
        resPoints<<points.at(i)*scene->view->transform().m11();
    }
    return resPoints;
}
