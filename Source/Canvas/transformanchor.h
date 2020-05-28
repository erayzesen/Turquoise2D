#pragma once

#include "QGraphicsItem"
#include "Canvas/selectiontransformitem.h"
#include "Canvas/transformtypes.h"
#include "QPainter"
#include "QtMath"
#include "QApplication"
#include "QGraphicsSceneHoverEvent"
#include "QGraphicsDropShadowEffect"

class SelectionTransformItem;
class TransformAnchor : public QGraphicsItem
{
public:
    TransformAnchor(SelectionTransformItem *_transformGroup,TransformTypes _type);
    SelectionTransformItem *transformGroup;
    int scaleAnchorSize=8;
    int originAnchorSize=2;
    QPointF *deltaFromOrigin=new QPointF();
    int distanceFromOrigin=0;
    void UpdateDeltaFromOrigin();
    QPointF GetDeltaFromOrigin();
    float GetDistanceFromOrigin(QPointF delta);
    QPointF CalculateScaleRate(QPointF otherDelta);
    bool checkRotationDistance(QPointF targetPoint);
    TransformTypes type;
    //Overriding paint()
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    QRectF boundingRect() const;
    //virtual reload
    bool pressedShift=false;

};

