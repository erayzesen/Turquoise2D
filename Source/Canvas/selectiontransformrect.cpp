#include "selectiontransformrect.h"

SelectionTransformRect::SelectionTransformRect(SelectionTransformItem *_transformGroup)
{
    transformGroup=_transformGroup;
    this->setFlag(QGraphicsItem::ItemIsMovable,false);
    this->setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
}



void SelectionTransformRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Stopped Paint when scene edit mode Collider Edit Mode
    GraphicsScene* gscene= dynamic_cast<GraphicsScene*>(transformGroup->scene);
    if(gscene->editMode==SceneEditModes::ColliderEdit){
        return;
    }

    QRectF *rect=transformGroup->currentRect;
    QPolygonF points;
    if(transformGroup->items.count()==1){
        GameItem *ngItem=dynamic_cast<GameItem*>(transformGroup->items.at(0));
        //Getting actual bound points from selected item
        points=ngItem->actualBounds();
    }else{
        float w=rect->width()/2,h=rect->height()/2;
        points=QPolygonF()<<QPointF(-w,-h)<<QPointF(w,-h)<<QPointF(w,h)<<QPointF(-w,h);
        // Rotating rect bound points for multiple item select
        points=MathOperations::rotatePointGroup(points,QPointF(0,0),transformGroup->deltaRotation);
    }
    //qDebug()<<"transformRectPaint :"<<points;
    //Crossing points scene viewport zoom factor
    for(int i=0;i<points.count();i++){
        points.replace(i,points.at(i)*transformGroup->scene->view->transform().m11());
    }
    /*
     points struct:
       p0➡︎p1
       ⬆︎##⬇︎
       p3⬅︎p2
      */
    //Drawing shadow
    painter->setPen(QPen(Qt::black,3));
    painter->drawLine(points.at(0),points.at(1));
    painter->drawLine(points.at(1),points.at(2));
    painter->drawLine(points.at(2),points.at(3));
    painter->drawLine(points.at(3),points.at(0));
    //Drawing Line
    painter->setPen(QPen(Qt::white,1));
    painter->drawLine(points.at(0),points.at(1));
    painter->drawLine(points.at(1),points.at(2));
    painter->drawLine(points.at(2),points.at(3));
    painter->drawLine(points.at(3),points.at(0));
}

QRectF SelectionTransformRect::boundingRect() const
{
    QRectF *rect=transformGroup->currentRect;
    return *rect;
}
