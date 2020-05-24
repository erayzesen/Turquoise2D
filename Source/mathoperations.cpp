#include "mathoperations.h"

MathOperations::MathOperations()
{

}

float MathOperations::vectorToAngle(QPointF v1, QPointF v2)
{
    return vectorToAngle(v2-v1);
}

float MathOperations::vectorToAngle(QPointF vector)
{
    float radian=qAtan2(vector.y(),vector.x());
    return radian;
}

QPolygonF MathOperations::rotatePointGroup(QPolygonF points, QPointF origin, float angle)
{
    if(angle==0){ // if object not rotated
        return points; // Returning unrotated points
    }
    qreal rot=qDegreesToRadians(angle);
    QPolygonF rotatedPoints; // Defining rotatepoly named new QPolygonF variable. It will be rotated point container after calculates.
    /*(i) Firstly, we are calculating the angle between the origin point and the border point and we're adding that angle to item's rotation angle.
    After we're crossing Cos(angle) Sin(angle) value with origin-border point distance.
    */
    for(int i=0;i<points.count();i++){ // For all unrotated points
        QPointF deltaPoint=points.at(i)-origin;
        qreal dist=qSqrt(deltaPoint.x()*deltaPoint.x()+deltaPoint.y()*deltaPoint.y()); // Getting distance between origin point and border point with pythagorean teorem.
        qreal defaultRot=qAtan2(deltaPoint.y(),deltaPoint.x()); // Getting angle value between origin point and border point with atan2(y,x) method.
        qreal totalRot=defaultRot+rot; // Getting total angle with add item's angle to point-origin angle.
        QPointF nPoint(qCos(totalRot)*dist,qSin(totalRot)*dist); // Cos(totalRot) for new point's x value, Sin(totalRot) for new point's y value
        nPoint=nPoint+origin;
        rotatedPoints<<nPoint; // Adding point to rotatedPoly
    }
    return rotatedPoints;
}

QPolygonF MathOperations::rotateRectPoints(QRectF rect, QPointF origin, float angle)
{
    QPointF a(rect.x()-rect.width()/2,rect.y()-rect.height()/2);
    QPointF b(rect.x()+rect.width()/2,rect.y()-rect.height()/2);
    QPointF c(rect.x()+rect.width()/2,rect.y()+rect.height()/2);
    QPointF d(rect.x()-rect.width()/2,rect.y()+rect.height()/2);
    QPolygonF toPolygon=QPolygonF()<<a<<b<<c<<d;
    toPolygon=rotatePointGroup(toPolygon,origin,angle);
    return toPolygon;

}



float MathOperations::threePointAngle(QPointF p1, QPointF p2, QPointF origin, bool convertToDegree)
{
    float beginAngle=MathOperations::vectorToAngle(origin,p1);
    float endAngle=MathOperations::vectorToAngle(origin,p2);
    float resultAngle=endAngle-beginAngle;
    resultAngle=convertToDegree==true ? qRadiansToDegrees(resultAngle):resultAngle;
    return resultAngle;
}

float MathOperations::setDigitFloat(float targetNum,int digitCount)
{
    float pow=qPow(10,digitCount);
    float temp=qRound(targetNum*pow)/pow;
    return temp;
}

QPointF MathOperations::setDigitFloat(QPointF targetPoint, int digitCount)
{
    return QPointF(setDigitFloat(targetPoint.x()),setDigitFloat(targetPoint.y()));
}

QSizeF MathOperations::setDigitFloat(QSizeF targetSize, int digitCount)
{
    return QSizeF(setDigitFloat(targetSize.width()),setDigitFloat(targetSize.height()));
}

//Getting draw points with DDA(Digital Differential Analyzer) algorithm
QList<QPointF> MathOperations::getDDAPoints(QPointF p1, QPointF p2, QSizeF pixelSize)
{
    QList<QPointF> result;
    float x1=p1.x();
    float y1=p1.y();
    float x2=p2.x();
    float y2=p2.y();
    float dx=x2-x1;
    float dy=y2-y1;
    float m=qAbs(dy/dx);
    float vx=x1<x2 ? pixelSize.width():-pixelSize.width();
    float vy=y1<y2 ? pixelSize.height():-pixelSize.height();
    if(qAbs(m)<=1){
        int pCount=qRound(qAbs(dx)/pixelSize.width());
        for(int i=0;i<pCount;i++){
            x1=x1+vx;
            y1=y1+m*vy;
            result<<QPointF(x1,y1);
        }
    }else{
        int pCount=qRound(qAbs(dy)/pixelSize.height());
        for(int i=0;i<pCount;i++){
            x1=x1+vx/m;
            y1=y1+vy;
            result<<QPointF(x1,y1);
        }
    }
    return result;
}



