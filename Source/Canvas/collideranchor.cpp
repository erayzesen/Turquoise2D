#include "collideranchor.h"

ColliderAnchor::ColliderAnchor(QVariantMap* _collider,QString _frameSide,GameItem* _gameItem,ColliderShapeItem* _inspectorItem)
{
    collider=_collider;
    frameSide=_frameSide;
    gameItem=_gameItem;
    inspectorItem=_inspectorItem;
    scene=inspectorItem->scene;
    scene->addItem(this);


    this->setAcceptHoverEvents(true);
    //this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    updatePositions();
}

void ColliderAnchor::setNewPosition(QPointF startPosition, QPointF lastPosition)
{
    //setting new position
    float deltaPosX=lastPosition.x()-startPosition.x();
    float deltaPosY=lastPosition.y()-startPosition.y();
    QPointF horizontalVel(deltaPosX,0);
    QPointF verticalVel(0,deltaPosY);
    if(scene->isSnapMode==true){
        horizontalVel=scene->snapFilter(horizontalVel,scene->gridSize);
        verticalVel=scene->snapFilter(verticalVel,scene->gridSize);
    }
    QPolygonF rotatedVelocities=MathOperations::rotatePointGroup(QPolygonF()<<horizontalVel<<verticalVel,QPointF(0,0),gameItem->rotate);
    horizontalVel=rotatedVelocities.at(0);
    verticalVel=rotatedVelocities.at(1);
    QPointF newPosition;
    if(frameSide=="l" || frameSide=="r"){
        newPosition=tempPosition+horizontalVel;
    }else if(frameSide=="t" || frameSide=="b"){
        newPosition=tempPosition+verticalVel;
    }
    this->setPos(newPosition.toPoint());
    //Calculating to real collider rect
    //Checking other anchor positions and getting horizontal and vertical delta
    QList<ColliderAnchor*> *allAnchors=scene->colliderAnchors;
    QPolygonF anchorsPosWithoutAngle;
    for(int i=0;i<allAnchors->count();i++){
        anchorsPosWithoutAngle.append(allAnchors->at(i)->scenePos());
    }
    anchorsPosWithoutAngle=MathOperations::rotatePointGroup(anchorsPosWithoutAngle,gameItem->scenePos(),-gameItem->rotate);

    float hDelta=anchorsPosWithoutAngle.at(2).x()-anchorsPosWithoutAngle.at(0).x();
    float vDelta=anchorsPosWithoutAngle.at(1).y()-anchorsPosWithoutAngle.at(3).y();
    hDelta=qAbs(hDelta);
    vDelta=qAbs(vDelta);
    //Getting horizontal and vertical center points
    float centerX=(anchorsPosWithoutAngle.at(2).x()+anchorsPosWithoutAngle.at(0).x())/2;
    float centerY=(anchorsPosWithoutAngle.at(1).y()+anchorsPosWithoutAngle.at(3).y())/2;
    //
    float deltaFromOriginX=centerX-gameItem->scenePos().x();
    float deltaFromOriginY=centerY-gameItem->scenePos().y();
    deltaFromOriginX/=gameItem->scale.width();
    deltaFromOriginY/=gameItem->scale.height();
    if(collider->value("type")=="circle"){
        if(frameSide=="l" || frameSide=="r"){
            vDelta=hDelta;
        }else if(frameSide=="t" || frameSide=="b"){
            hDelta=vDelta;
        }
    }

    hDelta/=gameItem->scale.width();
    vDelta/=gameItem->scale.height();


    collider->insert("rect",QVariant(QRectF(deltaFromOriginX,deltaFromOriginY,hDelta,vDelta) ) );

    if(frameSide=="l" || frameSide=="r"){
        allAnchors->at(1)->updatePositions();
        allAnchors->at(3)->updatePositions();
    }else if(frameSide=="t" || frameSide=="b"){
        allAnchors->at(0)->updatePositions();
        allAnchors->at(2)->updatePositions();
    }
    inspectorItem->updateValues();
    scene->views().at(0)->update();

    scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings

}

void ColliderAnchor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF originalRect(0,0,anchorSize,anchorSize);
    QPolygonF points=MathOperations::rotateRectPoints(originalRect,QPointF(0,0),gameItem->rotate);
    QColor green(Qt::green);
    //for shadow
    painter->setPen(Qt::black);
    painter->setBrush(Qt::NoBrush);
    painter->drawPolygon(points);
    //for shape
    painter->setPen(green);
    if(isHover==true){
        painter->setBrush(green);
    }else{
        painter->setBrush(Qt::NoBrush);
    }
    painter->drawPolygon(points);
}

QRectF ColliderAnchor::boundingRect() const
{
    return QRectF(-anchorSize/2,-anchorSize/2,anchorSize,anchorSize);
}

bool ColliderAnchor::updatePositions()
{
    QRectF rect=collider->value("rect").toRectF();
    QSizeF s=gameItem->scale; //GameItem scale
    rect.setRect(rect.x()*s.width(),rect.y()*s.height(),rect.width()*s.width(),rect.height()*s.height());
    QPolygonF rotatedPoints=MathOperations::rotateRectPoints(rect,QPointF(0,0),gameItem->rotate);
    QPointF centerPoint;
    if(frameSide=="l"){
        centerPoint=gameItem->scenePos()+(rotatedPoints.at(0)+rotatedPoints.at(3))/2;
    }else if(frameSide=="t"){
        centerPoint=gameItem->scenePos()+(rotatedPoints.at(0)+rotatedPoints.at(1))/2;
    }else if(frameSide=="r"){
        centerPoint=gameItem->scenePos()+(rotatedPoints.at(1)+rotatedPoints.at(2))/2;
    }else if(frameSide=="b"){
        centerPoint=gameItem->scenePos()+(rotatedPoints.at(2)+rotatedPoints.at(3))/2;
    }
    this->setPos(centerPoint);
    prepareAnchor();

}

void ColliderAnchor::prepareAnchor()
{
    tempPosition=this->scenePos();
}


void ColliderAnchor::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    isHover=true;
    update(boundingRect());
}

void ColliderAnchor::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    isHover=false;
    update(boundingRect());
}



