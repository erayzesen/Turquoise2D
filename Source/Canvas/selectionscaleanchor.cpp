#include "selectionscaleanchor.h"

selectionScaleAnchor::selectionScaleAnchor(int rectSize,SelectionTransformItem *transformGroup)
{
    QPoint *originPoint=transformGroup->originPoint;
    QGraphicsRectItem *rectItem=transformGroup->rectItem;
    if(!rectItem){
        qDebug()<<"recItem isn't valid object!--";
    }
    QPoint rightBottom(originPoint->x()+rectItem->boundingRect().width()/2,originPoint->y()+rectItem->boundingRect().height()/2);
    QGraphicsRectItem * rightbottomScaleItem=new QGraphicsRectItem(rightBottom.x()-rectSize/2,rightBottom.y()-rectSize/2,rectSize,rectSize);
    rightbottomScaleItem->setPen(QPen(Qt::white));
    transformGroup->addToGroup(rightbottomScaleItem);
}

