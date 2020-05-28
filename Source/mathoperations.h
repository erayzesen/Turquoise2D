#pragma once

#include "QPointF"
#include "QPoint"
#include "QtMath"
#include "QPolygonF"

class MathOperations
{
public:
    MathOperations();
    static float vectorToAngle(QPointF v1,QPointF v2);
    static float vectorToAngle(QPointF vector);
    //Getting rotated points
    static QPolygonF rotatePointGroup(QPolygonF points,QPointF origin,float angle);
    //Getting rotated points rotated rect
    static QPolygonF rotateRectPoints(QRectF rect,QPointF origin,float angle);
    //Getting angle between three point
    static float threePointAngle(QPointF p1, QPointF p2, QPointF origin, bool convertToDegree=true);
    //The function reduce float digit numbers. Example: 12.23234234 to 12.23
    static float setDigitFloat(float targetNum,int digitCount=2);
    static QPointF setDigitFloat(QPointF targetPoint,int digitCount=2);
    static QSizeF setDigitFloat(QSizeF targetSize,int digitCount=2);
    //Getting draw points with DDA(Digital Differential Analyzer) algorithm
    static QList<QPointF> getDDAPoints(QPointF p1,QPointF p2,QSizeF pixelSize);

};
