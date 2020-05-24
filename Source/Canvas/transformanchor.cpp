#include "transformanchor.h"

TransformAnchor::TransformAnchor(SelectionTransformItem *_transformGroup,TransformTypes _type)
{
    type=_type;
    transformGroup=_transformGroup;
    this->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    //this->setAcceptHoverEvents(true);
    QGraphicsDropShadowEffect * effect = new QGraphicsDropShadowEffect();
}

void TransformAnchor::UpdateDeltaFromOrigin()
{
    //(i) We call this function when transformItem created. This function save delta from origin before transformations.
    *deltaFromOrigin=GetDeltaFromOrigin();
}

QPointF TransformAnchor::GetDeltaFromOrigin()
{
    QPointF ori=*transformGroup->originPoint; //Item Group Origin
    QPointF cp=this->scenePos(); // Current position
    QPointF del=cp-ori; //delta cp-ori
    return del;
}


float TransformAnchor::GetDistanceFromOrigin(QPointF delta)
{
    //Calculating distance with the pysagor theorem way.
    float dist=qSqrt(delta.x()*delta.x()+delta.y()*delta.y());
    return dist;
}

QPointF TransformAnchor::CalculateScaleRate(QPointF otherDelta)
{
    /*(i) Getting begin distance and current distance with GetDistanceFromOrigin function.
     * We divide current distance to begin distance for find the new scale value.
    */
    Qt::KeyboardModifiers keyMod = QApplication::keyboardModifiers ();
    bool isShift=keyMod.testFlag(Qt::ShiftModifier);
    if(isShift){
        return QPointF(otherDelta.x()/deltaFromOrigin->x(),otherDelta.y()/deltaFromOrigin->y());
    }
    float defaultDist=GetDistanceFromOrigin(*deltaFromOrigin);
    float currentDist=GetDistanceFromOrigin(otherDelta);
    float rate=currentDist/defaultDist;
    return QPointF(rate,rate);
}

bool TransformAnchor::checkRotationDistance(QPointF targetPoint)
{
    /* (i) Calculating distance between mouse position and position of anchor item.
     * if distance is lower than 12, the function returns true.
    */
    QPointF deltaPoint=targetPoint-this->scenePos();
    QPointF deltaMouseFromOrigin=targetPoint-*transformGroup->originPoint;
    float dist=qSqrt(deltaPoint.x()*deltaPoint.x()+deltaPoint.y()*deltaPoint.y());
    if(dist<12 && GetDistanceFromOrigin(deltaPoint)<GetDistanceFromOrigin(deltaMouseFromOrigin)){
        return true;
    }
    return false;
}

void TransformAnchor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Stopped Paint when scene edit mode Collider Edit Mode
    GraphicsScene* gscene= dynamic_cast<GraphicsScene*>(transformGroup->scene);
    if(gscene->editMode==SceneEditModes::ColliderEdit){
        return;
    }

    /*(i) Drawing anchor according to the transform type
    */

    if(type==TransformTypes::Scale){
        //for drop shadows
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::black);
        QRectF dropShadowRect(-6,-6,12,12);
        painter->drawRect(dropShadowRect);

        painter->setPen(QPen(Qt::white));
        painter->setBrush(QColor("#006f74"));
        painter->drawRect(boundingRect());
        painter->setPen(QPen(Qt::NoPen));
        painter->setBrush(QColor("#00dde7"));
        painter->drawRect(QRectF(-2,-2,4,4));
    }else if(type==TransformTypes::Origin){
        //for drop shadows
        painter->setPen(Qt::black);
        painter->setBrush(Qt::black);
        painter->drawEllipse(QPoint(0,0),3,3);

        QPen pen(Qt::white);
        painter->setPen(pen);
        painter->setBrush(Qt::white);
        painter->drawEllipse(QPoint(0,0),originAnchorSize,originAnchorSize);
    }
}

QRectF TransformAnchor::boundingRect() const
{
    //(i) Setting selectable area
    float size=9;
    return QRectF(-size/2,-size/2,size,size);
}


