#include "selectiontransformitem.h"

SelectionTransformItem::SelectionTransformItem(QList<QGraphicsItem*> _items,GraphicsScene * _scene,PropertiesItem *_propItem)
{
    scene=_scene;
    items=_items;
    //(i) Defining anchor items for scale
    rightBottomAnchor=new TransformAnchor(this,TransformTypes::Scale);
    leftBottomAnchor=new TransformAnchor(this,TransformTypes::Scale);
    rightTopAnchor=new TransformAnchor(this,TransformTypes::Scale);
    leftTopAnchor=new TransformAnchor(this,TransformTypes::Scale);
    //Defining items container, all items will add to the container.
    itemsContainer=new QGraphicsItemGroup;
    scene->addItem(itemsContainer);

    scene->addItem(this);
    rectItem=setRectItem();
    this->setZValue(9999999999);
    //rectItem->setFlag(QGraphicsItem::ItemStacksBehindParent);
    /* (i) Order of creating transform Item; 1-add items 2-add transform tools 3-add rectangle item
     */
    this->addItems();
    UpdateContainerZOrder();
    this->addTransformTools();
    this->addToGroup(rectItem);
    this->setFlag(QGraphicsItem::ItemIsMovable,true);
    this->setFlag(QGraphicsItem::ItemIsFocusable,false);
    this->setFlag(QGraphicsItem::ItemIsSelectable,false);
    propItem=_propItem;
    //Creating new properties item and add properties panel
    GraphicsScene* gs=dynamic_cast<GraphicsScene*>(scene);
    if(gs!=NULL){
        if(propItem==NULL){
            propItem=new PropertiesItem(this,gs->propPanel);
            gs->propPanel->AddGameProperties(propItem);
        }else{
            propItem->transformItem=this;
            propItem->UpdateTransformProp();
        }

    }
}

SelectionTransformItem::~SelectionTransformItem()
{

}

void SelectionTransformItem::UpdateContainer()
{
 itemsContainer->setTransform(this->transform());
 itemsContainer->setPos(this->scenePos());
 itemsContainer->setRotation(this->rotation());
}

void SelectionTransformItem::UpdateContainerZOrder()
{
    qreal minZOrder=999999999999;
    for(int i=0;i<items.count();i++){
        GameItem *selectedGameItem=dynamic_cast<GameItem*>(items.at(i));
        if(selectedGameItem){
            minZOrder=selectedGameItem->zValue()<minZOrder ? selectedGameItem->zValue():minZOrder;
        }
    }
    itemsContainer->setZValue(minZOrder);
}


void SelectionTransformItem::updateOrigin()
{
    *originPoint=this->scenePos();
}

SelectionTransformRect* SelectionTransformItem::setRectItem()
{
    //(i) Defining min max variables  with big numbers as default.
    float minX=items.count()==0 ? 0:10000000000;
    float minY=items.count()==0 ? 0:10000000000;
    float maxX=items.count()==0 ? 0:-10000000000;
    float maxY=items.count()==0 ? 0:-10000000000;
    float aloneItemRot;
    if(items.count()==1){
        //(i) if the user selected one item,we're getting actual size and defining new rect.
        GameItem *selectedGameItem=dynamic_cast<GameItem*>(items.at(0));
        QSizeF cgsize=selectedGameItem->actualSize();
        minX=selectedGameItem->pos().x()-cgsize.width()/2;
        minY=selectedGameItem->pos().y()-cgsize.height()/2;
        maxX=selectedGameItem->pos().x()+cgsize.width()/2;
        maxY=selectedGameItem->pos().y()+cgsize.height()/2;
        aloneItemRot=0;
    }else{
        //(i) if the user selected multiple item, we're getting actual bounds from the item and checking min-max values
        for(int i=0;i<items.count();i++){
            GameItem *selectedGameItem=dynamic_cast<GameItem*>(items.at(i));
            if(selectedGameItem){
               QPolygonF itemBounds=selectedGameItem->actualBounds(true);
               for(int n=0;n<itemBounds.count();n++){
                   QPointF p=itemBounds.at(n);
                   minX=p.x()<minX ? p.x():minX;
                   minY=p.y()<minY ? p.y():minY;
                   maxX=p.x()>maxX ? p.x():maxX;
                   maxY=p.y()>maxY ? p.y():maxY;
               }

            }
        }
    }

    selectorSize=new QSizeF(maxX-minX,maxY-minY);
    //(i) We're creating transformRect according to min-max values in this period.
    transformRect=new QRectF(-selectorSize->width()/2,-selectorSize->height()/2,selectorSize->width(),selectorSize->height());
    currentRect=transformRect;
    SelectionTransformRect *nRectItem=new SelectionTransformRect(this);
    // Setting position of transformRect to origin.
    nRectItem->setPos(minX+selectorSize->width()/2,minY+selectorSize->height()/2);
    originPoint=new QPointF(minX+selectorSize->width()/2,minY+selectorSize->height()/2);
    this->setPos(originPoint->x(),originPoint->y());
    UpdateContainer();
    if(items.count()==1) this->setRotation(aloneItemRot);

    return nRectItem;
}

void SelectionTransformItem::addItems()
{
    //(i) Adding all items into group.
    for(int i=0;i<items.count();i++){
        GameItem *selectedGameItem=dynamic_cast<GameItem*>(items.at(i));
        if(selectedGameItem){
            QPointF delta=selectedGameItem->scenePos()-this->scenePos();
            itemsContainer->addToGroup(selectedGameItem);
            selectedGameItem->setPos(delta.x(),delta.y());
            //selectedGameItem->setFlag(QGraphicsItem::ItemStacksBehindParent);
        }
    }
}

void SelectionTransformItem::dropItems()
{
    //(i) Removing all items from group and clearing items array. Calculating the final item size for all items.
    for(int i=0;i<items.count();i++){
        GameItem *selectedGameItem=dynamic_cast<GameItem*>(items.at(i));
        if(selectedGameItem){
           itemsContainer->removeFromGroup(selectedGameItem);
            selectedGameItem->setSelected(false);
            QPointF currentScale(selectedGameItem->transform().m11(),selectedGameItem->transform().m22());
            QSize originalSize=selectedGameItem->pixmap().size();
            QSizeF finalSize=QSizeF(currentScale.x()*originalSize.width(),currentScale.y()*originalSize.height());
            QTransform tempTransform=QTransform();
        }
    }
    items.clear();
}

void SelectionTransformItem::addTransformTools()
{
    /*(i) Settings anchors for scale and rotate operations.
     * If the user selects an item, we're setting the anchor positions to the item's actualBounds
     * If the user selects multiple items, we're setting the anchor positions according to the transformItem's boundingRect values.
    */
    if(!rectItem)
        return;
    int rectSize=8;
    QPolygonF points(4);
    if(items.count()==1){
        GameItem *gitem=dynamic_cast<GameItem*>(items.at(0));
        if(gitem){
            points=gitem->actualBounds(true);
        }
    }else{
        points[0]=QPointF(originPoint->x()-rectItem->boundingRect().width()/2,originPoint->y()-rectItem->boundingRect().height()/2);
        points[1]=QPointF(originPoint->x()+rectItem->boundingRect().width()/2,originPoint->y()-rectItem->boundingRect().height()/2);
        points[2]=QPointF(originPoint->x()+rectItem->boundingRect().width()/2,originPoint->y()+rectItem->boundingRect().height()/2);
        points[3]=QPointF(originPoint->x()-rectItem->boundingRect().width()/2,originPoint->y()+rectItem->boundingRect().height()/2);
    }

    leftTopAnchor->setPos(points.at(0));
    leftTopAnchor->setZValue(100000000);
    leftTopAnchor->UpdateDeltaFromOrigin();
    this->addToGroup(leftTopAnchor);

    rightTopAnchor->setPos(points.at(1));
    rightTopAnchor->setZValue(100000000);
    rightTopAnchor->UpdateDeltaFromOrigin();
    this->addToGroup(rightTopAnchor);

    rightBottomAnchor->setPos(points.at(2));
    rightBottomAnchor->setZValue(100000000);
    rightBottomAnchor->UpdateDeltaFromOrigin();
    this->addToGroup(rightBottomAnchor);

    leftBottomAnchor->setPos(points.at(3));
    leftBottomAnchor->setZValue(100000000);
    leftBottomAnchor->UpdateDeltaFromOrigin();
    this->addToGroup(leftBottomAnchor);



    TransformAnchor *originAnchor=new TransformAnchor(this,TransformTypes::Origin);
    originAnchor->setPos(this->originPoint->x(),this->originPoint->y());
    this->addToGroup(originAnchor);
}

bool SelectionTransformItem::checkScaleCursor(QGraphicsSceneMouseEvent *mouseEvent)
{
    /* (i) Checking if mouse position is in the scale anchor and setting application cursor.*/
    bool tmp=false;
    QGraphicsItem *mouseOverItem=scene->itemAt(mouseEvent->scenePos(),QTransform());
    if(mouseOverItem){
        TransformAnchor *overedAnchor=dynamic_cast<TransformAnchor*>(mouseOverItem);
        if(overedAnchor){
            QPointF go=overedAnchor->transformGroup->scenePos(); // Group Origin
            QPointF tp=overedAnchor->scenePos();
            if((tp.x()>go.x() && tp.y()>go.y()) || (tp.x()<go.x() && tp.y()<go.y())){
                scene->view->setMouseCursor(QCursor(Qt::SizeFDiagCursor));
                tmp=true;
            }else if((tp.x()<go.x() && tp.y()>go.y()) || (tp.x()>go.x() && tp.y()<go.y())){
                scene->view->setMouseCursor(QCursor(Qt::SizeBDiagCursor));
                tmp=true;
            }
        }
    }
    return tmp;
}

bool SelectionTransformItem::checkRotateCursor(QGraphicsSceneMouseEvent *mouseEvent)
{
    /*(i) Checking if mouse position distance between enough for rotate transform.
     * And setting cursor, calculating cursor angle according to the between the origin and mousePosition.
     */
    bool tmp=false;
    bool cond1=this->rightBottomAnchor->checkRotationDistance(mouseEvent->scenePos());
    bool cond2=this->leftBottomAnchor->checkRotationDistance(mouseEvent->scenePos());
    bool cond3=this->leftTopAnchor->checkRotationDistance(mouseEvent->scenePos());
    bool cond4=this->rightTopAnchor->checkRotationDistance(mouseEvent->scenePos());
    if(cond1 || cond2 || cond3 || cond4){
        QPixmap cursor(":/mouseIcons/rotate_mouse_cursor.png");
        float angle=MathOperations::vectorToAngle(mouseEvent->scenePos(),*this->originPoint);
        angle=qRadiansToDegrees(angle);
        QTransform trans;
        trans.rotate(angle-90);
        cursor=cursor.transformed(trans,Qt::SmoothTransformation);
        //qDebug()<<angle;

        QCursor nCursor(cursor);
        scene->view->setMouseCursor(nCursor);
        tmp=true;
    }

    return tmp;
}

void SelectionTransformItem::updateLastRotation()
{
    //(i) We need lastRotation variable because calculating delta rotation except old rotation. Delta rotation=lastRotation+resultAngle
    lastRotation=this->rotation();
}

bool SelectionTransformItem::checkTransformCursor(QGraphicsSceneMouseEvent *mouseEvent)
{
    //(i) Checking if the mouse position is over the transformitem. If it's true, we're setting mouse cursor as arrow.
    bool tmp=false;
    QGraphicsItem *mouseOverItem=scene->itemAt(mouseEvent->scenePos(),QTransform());
    if(mouseOverItem){
        SelectionTransformRect *overedTransformItem=dynamic_cast<SelectionTransformRect*>(mouseOverItem);
        if(overedTransformItem){
            scene->view->setMouseCursor(QCursor(Qt::ArrowCursor));
            tmp=true;
        }
    }
    return tmp;

}

void SelectionTransformItem::rotateTransform(QPointF p1, QPointF p2)
{
    /* Calculating the angle between p1 and p2.
     * p1 for begin angle (clicked mouse position)
     * p2 for current angle (moving mouse position)
     * Result angle is the endAngle-beginAngle. Because we need rotate according to the clicked point angle.
     * Delta rotation is how many degree rotated item except the default rotation angle.
    */

    float beginAngle=MathOperations::vectorToAngle(*this->originPoint,p1);
    float endAngle=MathOperations::vectorToAngle(*this->originPoint,p2);
    //qDebug()<<"begin: "<<beginAngle<<" end: "<<endAngle;
    float resultAngle=endAngle-beginAngle;
    resultAngle=qRadiansToDegrees(resultAngle);
    //qDebug()<<"result: "<<resultAngle;
    rotateTransform(resultAngle);

}

void SelectionTransformItem::rotateTransform(float angle)
{
    deltaRotation=lastRotation+angle;
    //reducing digits
    deltaRotation=qRound(deltaRotation*100);
    deltaRotation/=100;
    this->setRotation(deltaRotation);
    applyRotations();
    this->UpdateContainer();
}

void SelectionTransformItem::UpdateBounds()
{
    //(i) We need update the selectable area according to the current scale.
    float w=transformRect->width()*this->transform().m11();
    float h=transformRect->height()*this->transform().m22();
    currentRect=new QRectF(-w/2,-h/2,w,h);
}

void SelectionTransformItem::applyRotations()
{
    /* (i) Setting new rotation values of game items.
     * We're using tempRotate for that. tempRotate is the angle before transform operation.
     * We're adding delta rotation to the tempRotate value of item for set current item's rotation value.
    */
    for(int i=0;i<items.count();i++){
        GameItem *selectedGameItem=dynamic_cast<GameItem*>(items.at(i));
        if(selectedGameItem){
            selectedGameItem->rotate=selectedGameItem->tempRotate+deltaRotation;
        }
    }
}

void SelectionTransformItem::applyScales()
{
    /* (i) Setting new scale values of game items.
     * We're using tempScale for that. tempScale is the scale value of item before transform operation.
     * We're crossing tempScale values and transformItem current scale values for calculating new item's scale.
    */
    for(int i=0;i<items.count();i++){
        GameItem *selectedGameItem=dynamic_cast<GameItem*>(items.at(i));
        if(selectedGameItem){
            selectedGameItem->scale.setWidth(selectedGameItem->tempScale.width()*(this->transform().m11()));
            selectedGameItem->scale.setHeight(selectedGameItem->tempScale.height()*(this->transform().m22()));
        }
    }
}

void SelectionTransformItem::rePrepareItems()
{
    /*(i) We didn't use that function. We didn't need that for current version.
     * This function re-prepare game items for transformations.
     */
    for(int i=0;i<items.count();i++){
        GameItem *selectedGameItem=dynamic_cast<GameItem*>(items.at(i));
        if(selectedGameItem){
            selectedGameItem->prepareTransform();
        }
    }
}

void SelectionTransformItem::updateAnchorTransforms(QPolygonF points)
{

}

void SelectionTransformItem::setNewScale(QPointF newScale,bool digitFilter)
{
    //Setting scale with new value (we convert two-digit number)
    QPointF filtScale=digitFilter==true ? MathOperations::setDigitFloat(newScale):newScale;
    QTransform trans;
    trans.scale(filtScale.x(),filtScale.y());
    this->setTransform(trans);
    this->applyScales();
    this->UpdateBounds();
    this->propItem->UpdateTransformProp();
    this->UpdateContainer();
}



