#include "graphicsscene.h"
#include "ui_turquoise2deditor.h"

GraphicsScene::GraphicsScene(QString _sceneFilePath,PropertiesPanel *_propPanel,ScenePanel *_scenePanel)
{
    mainWindow=_scenePanel->mainWindow;
    sceneFilePath=_sceneFilePath;
    scenePanel=_scenePanel;

    drawOriginPoint();
    //Parsing scene json file (this period must be first then hierarchy operations!)
    sceneJsonObject=fromJsonFile(sceneFilePath);

    //Setting Scene Properties
    QJsonObject sceneProperties=sceneJsonObject->value("sceneProperties").toObject();
    QString jsonType=sceneProperties.value("type").toString();
    QJsonArray gridSizeArr=sceneProperties.value("gridSize").toArray();
    qDebug()<<gridSizeArr.at(0);
    this->gridSize=QSizeF(gridSizeArr.at(0).toDouble(),gridSizeArr.at(1).toDouble());
    this->isSnapMode=sceneProperties.value("isSnapMode").toBool();
    QString snapStyleStr=sceneProperties.value("snapStyle").toString();
    if(snapStyleStr=="withBounds"){
        this->snapStyle=SnapStyle::withBounds;
    }else if(snapStyleStr=="withCenter"){
        this->snapStyle=SnapStyle::withCenter;
    }

    //Defining hierarchyList and propPanel pointers
    hierarchyPanel=new HierarchyPanel(this,mainWindow);
    //hierarchyList=new HierarchyListView();
    //hierarchyList->gameScene=this;
    propPanel=_propPanel;
    connect(this,&QGraphicsScene::selectionChanged,this,&GraphicsScene::isSelectedChanged);
    //QApplication::setOverrideCursor(QCursor(Qt::SizeFDiagCursor));
    stack=new QUndoStack(this);
    guidesAndGrids=new GuidesAndGrids(this);
    this->toJsonObject();
    this->setFocus();
}

GraphicsScene::~GraphicsScene()
{
    qDebug()<<"scene deleted!";
    clearTransformSelector();
    delete lastTransformItem;
    delete hierarchyPanel;

}
void GraphicsScene::isSelectedChanged(){
    if(QApplication::focusWidget()->objectName()!="sceneView"){
        return;
    }

}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(editMode==SceneEditModes::GameItemsEdit){
        GameItemsEditMode_MousePress(mouseEvent);
    }else if(editMode==SceneEditModes::CameraEdit){
        //Camera Edit Press Event
    }else if(editMode==SceneEditModes::CanvasEdit){
        //Canvas Edit Press Event
    }else if(editMode==SceneEditModes::ColliderEdit){
        ColliderEditMode_MousePress(mouseEvent);
        //Collider Edit Press Event
    }else if(editMode==SceneEditModes::ZoomEdit){
        ZoomEditMode_MousePress(mouseEvent);
        //Zoom Edit Press Event
    }else if(editMode==SceneEditModes::PanEdit){
        PanEditMode_MousePress(mouseEvent);
        //Pan Edit Press Event
    }else if(editMode==SceneEditModes::DrawEdit){
        DrawEditMode_MousePress(mouseEvent);
        //Draw Edit Press Event
    }else if(editMode==SceneEditModes::EyedropperEdit){
        EyedropperEditMode_MousePress(mouseEvent);
        //Draw Edit Press Event
    }
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(editMode==SceneEditModes::GameItemsEdit){
        GameItemsEditMode_MouseMove(mouseEvent);
    }else if(editMode==SceneEditModes::CameraEdit){
        //Camera Edit Move Event
    }else if(editMode==SceneEditModes::CanvasEdit){
        //Canvas Edit Move Event
    }else if(editMode==SceneEditModes::ColliderEdit){
        ColliderEditMode_MouseMove(mouseEvent);
        //Collider Edit Move Event
    }else if(editMode==SceneEditModes::ZoomEdit){
        ZoomEditMode_MouseMove(mouseEvent);
        //Zoom Edit Move Event
    }else if(editMode==SceneEditModes::PanEdit){
        PanEditMode_MouseMove(mouseEvent);
        //Pan Edit Move Event
    }else if(editMode==SceneEditModes::DrawEdit){
        DrawEditMode_MouseMove(mouseEvent);
        //Draw Edit Press Event
    }else if(editMode==SceneEditModes::EyedropperEdit){
        EyedropperEditMode_MouseMove(mouseEvent);
        //Draw Edit Press Event
    }
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(editMode==SceneEditModes::GameItemsEdit){
        GameItemsEditMode_MouseRelease(mouseEvent);
    }else if(editMode==SceneEditModes::CameraEdit){
        //Camera Edit Release Event
    }else if(editMode==SceneEditModes::CanvasEdit){
        //Canvas Edit Release Event
    }else if(editMode==SceneEditModes::ColliderEdit){
        ColliderEditMode_MouseRelease(mouseEvent);
        //Collider Edit Release Event
    }else if(editMode==SceneEditModes::ZoomEdit){
        ZoomEditMode_MouseRelease(mouseEvent);
        //Zoom Edit Release Event
    }else if(editMode==SceneEditModes::PanEdit){
        PanEditMode_MouseRelease(mouseEvent);
        //Pan Edit Release Event
    }else if(editMode==SceneEditModes::DrawEdit){
        DrawEditMode_MouseRelease(mouseEvent);
        //Draw Edit Press Event
    }else if(editMode==SceneEditModes::EyedropperEdit){
        EyedropperEditMode_MouseRelease(mouseEvent);
        //Draw Edit Press Event
    }
}




void GraphicsScene::drawOriginPoint(){
    // (i) Drawing scene origin point (zero point) as ellipse
    QPen pen(Qt::white);
    this->addRect(-.5,-.5,1,1,pen,QBrush(Qt::white));
}

void GraphicsScene::sceneChanged()
{
    scenePanel->sceneChanged();
}

void GraphicsScene::setEditMode(SceneEditModes targetMode)
{
    this->views().at(0)->setDragMode(QGraphicsView::NoDrag);
    this->view->setMouseCursor(QCursor(Qt::ArrowCursor)); //Setting default mouse cursor

    //Clear variables;

    if(targetMode==SceneEditModes::GameItemsEdit){
        scenePanel->clearToolsLayout();
        this->clearColliderAnchors();
        this->views().at(0)->setDragMode(QGraphicsView::RubberBandDrag); // Setting drag mode with default rubberband feature
    }else if(targetMode==SceneEditModes::PanEdit){
        scenePanel->clearToolsLayout();
        this->view->setMouseCursor(QCursor(Qt::OpenHandCursor)); //Setting hand cursor
    }else if(targetMode==SceneEditModes::ColliderEdit){

        this->views().at(0)->setDragMode(QGraphicsView::NoDrag);
    }else if(targetMode==SceneEditModes::ZoomEdit){
        scenePanel->clearToolsLayout();
        scenePanel->addZoomTools();
        QPixmap cursor(":/mouseIcons/zoom_mouse_cursor.png");
        QCursor nCursor(cursor);
        this->view->setMouseCursor(nCursor);
    }else if(targetMode==SceneEditModes::DrawEdit){
        this->clearTransformSelector();
        if(scenePanel->eyedropperButton!=NULL){
            scenePanel->eyedropperButton->setChecked(false);
        }else{
            this->scenePanel->addDrawTools();
        }
        QPixmap cursor(":/mouseIcons/pencil_mouse_cursor.png");
        QCursor nCursor(cursor);
        this->view->setMouseCursor(nCursor);
    }else if(targetMode==SceneEditModes::EyedropperEdit){
        this->clearTransformSelector();
        if(scenePanel->eyedropperButton)
            scenePanel->eyedropperButton->setChecked(true);
        QPixmap cursor(":/mouseIcons/eyedropper_mouse_cursor.png");
        QCursor nCursor(cursor);
        this->view->setMouseCursor(nCursor);
    }
    this->editMode=targetMode;
}

//Drawing Grids and Gudes
void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QColor backgroundColor("#201f1f"); // Setting scene background color
    QColor gridColor("#2a2a2a"); // Setting grid color
    QColor zeroAxesColor("#4e4e4e"); // Setting grid color
    //this->setBackgroundBrush(backgroundColor);
    painter->setBrush(backgroundColor);
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect);

    //draw zero axes
    painter->setBrush(Qt::NoBrush);
    QPen axesPen(zeroAxesColor);
    axesPen.setWidth(2);
    painter->setPen(axesPen);

    painter->drawLine(QPointF(0,rect.top()),QPointF(0,rect.bottom()));
    painter->drawLine(QPointF(rect.left(),0),QPointF(rect.right(),0));


    //draw grid
    painter->setBrush(Qt::NoBrush);
    QPen gridPen(gridColor);
    gridPen.setWidth(1);
    painter->setPen(gridPen);
    qreal top=int(rect.top())-(int(rect.top() ) % int(gridSize.width()));
    qreal left=int(rect.left())-(int(rect.left() )%int(gridSize.height()));

    for(int x=left;x<rect.right();x+=gridSize.width())
        painter->drawLine(QPointF(x,rect.top()),QPointF(x,rect.bottom()));
    for(int y=top;y<rect.bottom();y+=gridSize.height())
        painter->drawLine(QPointF(rect.left(),y),QPointF(rect.right(),y));


}

//::::::::::::::::::::GAME ITEMS EDIT MODE::::::::::::::::::::

void GraphicsScene::GameItemsEditMode_MousePress(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button()!=Qt::LeftButton){  // if don't left click end event function
        return;
    }
    startRotatePoint=NULL; // It's the clear rotate point when the user clicks mouse-left.
    QMap<QString,bool> iconResults=setMouseIcons(mouseEvent);
    if(iconResults.value("selectedRotate")==true){
        startRotatePoint=new QPointF(mouseEvent->scenePos().x(),mouseEvent->scenePos().y()); // It's the define new rotate point when user start rotate transform.
        lastTransformItem->updateLastRotation(); // We're call that function because of save current rotation to transformItem.
        qDebug()<<"startRotatePoint:"<< *startRotatePoint;
        return;
    }

    QList<QGraphicsItem*> selectedList=this->selectedItems();
    QGraphicsItem *item=this->itemAt(mouseEvent->scenePos(),QTransform()); // Getting when current mouse position over the QGraphicsItem.
    GameItem *selectedGameItem; // Defining GameItem object
    SelectionTransformItem *transformItem; // Defining SelectionTransformItem
    TransformAnchor *transformAnchorItem; // Defining Anchor Item
    this->startSelectionPoint=NULL;// It's clear selection point when the user clicks mouse-left.
    this->selectedAnchor=NULL; // It's clear selectorAnchor variable
    this->lastSelectedTransformItem=NULL; // It's clear last selected transform item variable
    if(item){
        // (i) Trying types to understand item types.
        selectedGameItem=dynamic_cast<GameItem*>(item);
        transformItem=dynamic_cast<SelectionTransformItem*>(item->parentItem());
        transformAnchorItem=dynamic_cast<TransformAnchor*>(item);

        if(transformAnchorItem){ // Checking if item is TransformAnchor
            selectedAnchor=transformAnchorItem;
            //Saving transform last scale values before scale operation
            if(lastTransformItem){
                this->startScaleSize=new QSizeF(lastTransformItem->transform().m11(),lastTransformItem->transform().m22());
            }
            // (i) disabled this function for now
            //selectedAnchor->UpdateDeltaFromOrigin();
        }else if(transformItem){ // Checking if item is SelectionTransformItem
            *startMovePoint=transformItem->scenePos();
            qDebug()<<"baslangic pos: "<<*startMovePoint;
            lastSelectedTransformItem=transformItem;
        }else if(selectedGameItem){ // Checking if item is GameItem
            qDebug()<<"gameItem Secildi:";
            if(selectedGameItem->isLocked==true){
                return;
            }
            this->clearTransformSelector(); //Calling this function for clear transform selector
            this->addTransformSelector(QList<QGraphicsItem*>()<<selectedGameItem); // Calling this function for add new transformSelector item
            //Defining last selected transform item and startMovePoint
            *startMovePoint=lastTransformItem->scenePos();
            qDebug()<<"baslangic pos: "<<*startMovePoint;
            lastSelectedTransformItem=transformItem;
            //
            if(lastTransformItem){
               lastSelectedTransformItem=lastTransformItem;
            }
        }else{
            this->clearTransformSelector(); // if item isn't SelectedGameItem, calling this function for clear transform selector
        }
    }else{
        this->clearTransformSelector(); //if there isn't an item, calling this function for clear transform selector
        this->startSelectionPoint=new QPointF(mouseEvent->scenePos().x(),mouseEvent->scenePos().y()); //
    }
    QGraphicsScene::mousePressEvent(mouseEvent); // Calling default event.
}

void GraphicsScene::GameItemsEditMode_MouseMove(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(startRotatePoint!=NULL){ //Checking if the user has started rotation mode.
        this->views().at(0)->setDragMode(QGraphicsView::NoDrag); // Setting drag mode off (for deactive rubber band feature)
        lastTransformItem->rotateTransform(*startRotatePoint,mouseEvent->scenePos()); // Calling rotateTransform function of TransformItem with start point and current point parameters.
        lastTransformItem->propItem->UpdateTransformProp();
        return;
    }else{
        this->views().at(0)->setDragMode(QGraphicsView::RubberBandDrag); // Setting drag mode with default rubberband feature
    }
    //(i) if the user don't select scale anchor or enter rotation mode, we're calling setMouseIcons function.
    if(selectedAnchor==NULL && startRotatePoint==NULL){
        this->view->setMouseCursor(QCursor(Qt::ArrowCursor)); // Setting default mouse cursor
        setMouseIcons(mouseEvent); // Choosing all currently mouse icon options with setMouseIcons function.
    }


    if(mouseEvent->buttons()!=Qt::LeftButton){ // Checking if the user click left click mouse button
        return;
    }

    if(selectedAnchor!=NULL){
        //(i) if the user is dragging selectedAnchor, calculating new size and applying size
        QPointF mouseDelta=mouseEvent->scenePos()-*lastTransformItem->originPoint;
        QPointF nSize=selectedAnchor->CalculateScaleRate(mouseDelta);
        lastTransformItem->setNewScale(nSize);
    }else if(lastSelectedTransformItem){
        // (i) if the user dragging transform area, calling default event.
        lastSelectedTransformItem->updateOrigin();
        lastSelectedTransformItem->propItem->UpdateTransformProp();
        QGraphicsScene::mouseMoveEvent(mouseEvent);
        rePosForSnap(lastSelectedTransformItem);
        lastSelectedTransformItem->UpdateContainer();
    }else if(startSelectionPoint){
        // If the user clicked the empty area, starting calculate rectangle-selection items operation and creating new transformItem for selected items.
        QSize selectionSize=QSize(mouseEvent->scenePos().x()-startSelectionPoint->x(),mouseEvent->scenePos().y()-startSelectionPoint->y());
        QPoint sPoint=QPoint(startSelectionPoint->x(),startSelectionPoint->y());
        // These conditions for fix the reverse selection rectangle.
        if(selectionSize.width()<0){
            // Fixing x position if the user created reverse width rectangle
            sPoint.setX(startSelectionPoint->x()+selectionSize.width());
            selectionSize.setWidth(selectionSize.width()*-1);
        }
        if(selectionSize.height()<0){
            // Fixing y position if the user created reverse width rectangle
            sPoint.setY(startSelectionPoint->y()+selectionSize.height());
            selectionSize.setHeight(selectionSize.height()*-1);
        }
        // Creating selection rect
        QRect selectionRect=QRect(sPoint.x(),sPoint.y(),selectionSize.width(),selectionSize.height());
        //Getting selected items with QGraphicsScene::items(QRect) method.
        QList<QGraphicsItem*> selectedList=this->items(selectionRect);
        //(i)Filtering List according to if the item is GameItem.
        QList<QGraphicsItem*> tempList;
        for(int i=0;i<selectedList.count();i++){
            GameItem* gitem=dynamic_cast<GameItem*>(selectedList.at(i));
            if(gitem){
                if(gitem->isLocked==false){
                    tempList<<selectedList.at(i);
                }
            }
        }
        selectedList=tempList;
        //Clearing old transform item
        this->clearTransformSelector();
        //Creating new transform item for new selected items.
        if(selectedList.count()>0){
            this->addTransformSelector(selectedList);
        }
        QGraphicsScene::mouseMoveEvent(mouseEvent);

    }else{
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void GraphicsScene::GameItemsEditMode_MouseRelease(QGraphicsSceneMouseEvent *mouseEvent)
{
    QUndoCommand *cmd;
    //(i) Clearing all check mouse event variables
    if(startRotatePoint!=NULL){
        float deltaAngle=MathOperations::threePointAngle(*startRotatePoint,mouseEvent->scenePos(),*lastTransformItem->originPoint);
        qDebug()<<"delta angle :"<<deltaAngle;
        cmd=new RotateItemCommand(this,lastTransformItem->items,deltaAngle);
        stack->push(cmd);
        startRotatePoint=NULL;

        sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    }else if(selectedAnchor!=NULL){
        // (i) disabled re-create function for now.
        QSizeF finalScale(lastTransformItem->transform().m11(),lastTransformItem->transform().m22());
        cmd=new ScaleItemCommand(this,lastTransformItem->items,*startScaleSize,finalScale);
        stack->push(cmd);
        selectedAnchor=NULL;

        sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    }else if(lastSelectedTransformItem!=NULL){
        cmd=new MoveItemCommand(this,lastTransformItem->items,*startMovePoint,lastTransformItem->scenePos());
        stack->push(cmd);

        lastSelectedTransformItem->updateOrigin();

        sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    }else if(lastTransformItem){
        lastTransformItem->updateOrigin();
    }
    startSelectionPoint=NULL;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

//::::::::::::::::::::END::::::::::::::::::::

//::::::::::::::::::::COLLIDERS EDIT MODE::::::::::::::::::::

void GraphicsScene::ColliderEditMode_MousePress(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem *item=this->itemAt(mouseEvent->scenePos(),QTransform()); // Getting when current mouse position over the QGraphicsItem.
    startSelectionPoint=NULL;
    ColliderAnchor *colAnchor=dynamic_cast<ColliderAnchor*>(item);
    if(item){
        if(colAnchor){
            qDebug()<<"collider Anchor secildi";
            startSelectionPoint=new QPointF(mouseEvent->scenePos());
            lastSelectedColliderAnchor=colAnchor;
        }else{
            clearTransformSelector();
            lastSelectedTransformItem=NULL;
            return;
        }
    }else{
        clearTransformSelector();
        lastSelectedTransformItem=NULL;
        return;
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::ColliderEditMode_MouseMove(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(lastSelectedColliderAnchor!=NULL){
        lastSelectedColliderAnchor->setNewPosition(*startSelectionPoint,mouseEvent->scenePos());
    }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphicsScene::ColliderEditMode_MouseRelease(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(lastSelectedColliderAnchor!=NULL){
        lastSelectedColliderAnchor->prepareAnchor();
        lastSelectedColliderAnchor=NULL;
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}



//::::::::::::::::::::END::::::::::::::::::::

//::::::::::::::::::::PAN EDIT MODE::::::::::::::::::::

void GraphicsScene::PanEditMode_MousePress(QGraphicsSceneMouseEvent *mouseEvent)
{
    startSelectionPoint=new QPointF(mouseEvent->scenePos());
    isPanStarted=true;
}

void GraphicsScene::PanEditMode_MouseMove(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(isPanStarted && startSelectionPoint){
        QPointF deltaMousePosition=*startSelectionPoint-mouseEvent->scenePos();
        deltaMousePosition/=4;
        this->setSceneRect(sceneRect().x()+deltaMousePosition.x(),sceneRect().y()+deltaMousePosition.y(),sceneRect().width(),sceneRect().height());
        this->views().at(0)->update();
        this->view->setMouseCursor(QCursor(Qt::ClosedHandCursor));
    }else{
        this->view->setMouseCursor(QCursor(Qt::OpenHandCursor));
    }
}

void GraphicsScene::PanEditMode_MouseRelease(QGraphicsSceneMouseEvent *mouseEvent)
{
    startSelectionPoint=NULL;
    isPanStarted=false;
}

//::::::::::::::::::::END::::::::::::::::::::

//::::::::::::::::::::ZOOM EDIT MODE::::::::::::::::::::

void GraphicsScene::ZoomEditMode_MousePress(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(startSelectionPoint!=NULL){
        delete startSelectionPoint;
        startSelectionPoint=NULL;
    }
    if(startMovePoint!=NULL){
        delete startMovePoint;
        startMovePoint=NULL;
    }
    if(startScaleSize!=NULL){
        delete startScaleSize;
        startScaleSize=NULL;
    }
    startSelectionPoint=new QPointF(mouseEvent->scenePos());
    startMovePoint=new QPointF(mouseEvent->scenePos());
    isPanStarted=true;
    float beginScale=this->views().at(0)->transform().m11();
    startScaleSize=new QSizeF(beginScale,beginScale);
    externalCounter=0;

}

void GraphicsScene::ZoomEditMode_MouseMove(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPixmap cursor(":/mouseIcons/zoom_mouse_cursor.png");
    QCursor nCursor(cursor);
    this->view->setMouseCursor(nCursor);

    float scaleFactor=0.1;

    if(startSelectionPoint!=NULL){
        externalCounter++;
        if(externalCounter%5!=0){
            *startMovePoint=mouseEvent->scenePos();
            return;
        }
        float deltaX=mouseEvent->scenePos().x()-startMovePoint->x();
        float nScale=deltaX<0 ? 1-scaleFactor:1+scaleFactor;
        nScale=deltaX==0 ? 1:nScale;
        if(view->transform().m11()*nScale<0.25){
            view->resetTransform();
            view->scale(0.25,0.25);
        }else if(view->transform().m11()*nScale>32){
            view->resetTransform();
            view->scale(32,32);
        }else{
            view->centerOn(*startSelectionPoint);
            view->scale(nScale,nScale);
        }
        if(scenePanel->zoomResult!=NULL){
            float percent=qFloor(this->view->transform().m11()*100);
            scenePanel->zoomResult->setText(QString::number(percent)+"%");
        }
        *startMovePoint=mouseEvent->scenePos();

    }
}

void GraphicsScene::ZoomEditMode_MouseRelease(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(startSelectionPoint!=NULL){
        delete startSelectionPoint;
        startSelectionPoint=NULL;
    }
    externalCounter=0;

    if(startScaleSize!=NULL){
        delete startScaleSize;
        startScaleSize=NULL;
    }

}

void GraphicsScene::resetZoomLevel()
{
    view->resetTransform();
    view->scale(1,1);
    if(scenePanel->zoomResult!=NULL){
        float percent=qFloor(this->view->transform().m11()*100);
        if(scenePanel->zoomResult!=NULL){
            scenePanel->zoomResult->setText(QString::number(percent)+"%");
        }
    }
}

//::::::::::::::::::::END::::::::::::::::::::

//::::::::::::::::::::DRAW EDIT MODE::::::::::::::::::::
void GraphicsScene::DrawEditMode_MousePress(QGraphicsSceneMouseEvent *mouseEvent)
{

    if(!eyedroppedItemJson.isEmpty()){
        if(startSelectionPoint)
            delete startSelectionPoint;
        startSelectionPoint=new QPointF(mouseEvent->scenePos());
        QPointF cpoint=mouseEvent->scenePos();
        if(isSnapMode){
            bool acenter=snapStyle==SnapStyle::withCenter ? true:false;
            cpoint=snapFilter(cpoint,gridSize,acenter);
        }
        QSizeF size=QSizeF(eyedroppedItemSize.width()-2,eyedroppedItemSize.height()-2);
        QList<GameItem *> collidedGameItems=collisionDedectiosnForDrawing(cpoint,size,eyedroppedItemJson);
        if(collidedGameItems.count()==0){
            QUndoCommand *cmd=new AddJsonBasedItemCommand(this,QList<QJsonObject>()<<eyedroppedItemJson,true,cpoint);
            stack->push(cmd);
            sceneChanged(); //Calling sceneChanged() function for save operations and warnings
        }
    }
}

void GraphicsScene::DrawEditMode_MouseMove(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPixmap cursor(":/mouseIcons/pencil_mouse_cursor.png");
    QCursor nCursor(cursor);
    this->view->setMouseCursor(nCursor);

    if(!eyedroppedItemJson.isEmpty()){
        if(startSelectionPoint!=NULL){
            //DDA (Dijital Differential Analyzer) Algorithm

            QList<QPointF> drawPoints=MathOperations::getDDAPoints(*startSelectionPoint,mouseEvent->scenePos(),eyedroppedItemSize);
            QSizeF size=QSizeF(eyedroppedItemSize.width()-2,eyedroppedItemSize.height()-2);
            for(int i=0;i<drawPoints.count();i++){
                QPointF cPoint=drawPoints.at(i);
                if(isSnapMode){
                    bool acenter=snapStyle==SnapStyle::withCenter ? true:false;
                    cPoint=snapFilter(cPoint,gridSize,acenter);
                }
                QList<GameItem *> collidedGameItems=collisionDedectiosnForDrawing(cPoint,size,eyedroppedItemJson);
                if(collidedGameItems.count()==0){
                    QUndoCommand *cmd=new AddJsonBasedItemCommand(this,QList<QJsonObject>()<<eyedroppedItemJson,true,cPoint);
                    stack->push(cmd);
                    sceneChanged(); //Calling sceneChanged() function for save operations and warnings
                }
            }
            if(drawPoints.count()>0){
                startSelectionPoint=new QPointF(drawPoints.at(drawPoints.count()-1));
            }

        }
    }

}

void GraphicsScene::DrawEditMode_MouseRelease(QGraphicsSceneMouseEvent *mouseEvent)
{
    startSelectionPoint=NULL;
}

QList<GameItem *> GraphicsScene::collisionDedectiosnForDrawing(QPointF point,QSizeF size,QJsonObject itemJson)
{
    QList<QGraphicsItem*> collidedItems;
    QList<GameItem*> collidedGameItems;
    if(drawCollisionDedection==DrawCollisionDedection::None){
        return collidedGameItems;
    }
    QPointF cPoint=point;
    if(isSnapMode){
       bool aCenter=snapStyle==SnapStyle::withCenter ? true:false;
       cPoint=snapFilter(cPoint,gridSize,aCenter);
    }
    collidedItems=this->items(QRect(cPoint.x()-size.width()/2,cPoint.y()-size.height()/2,size.width(),size.height()));
    for(int n=0;n<collidedItems.count();n++){
        QGraphicsItem *item=collidedItems.at(n);
        GameItem *gItem=dynamic_cast<GameItem*>(item);
        if(gItem){
            if(drawCollisionDedection==DrawCollisionDedection::DedectionForSimilarItems  && itemJson.isEmpty()==false){
                //Checking different between json object informations and game item informations
                if(gItem->itemType==GameItemTypes::Object){
                    //Condinations: objectClassPath and spritePath
                    if(itemJson.value("objectClassPath").toString()==gItem->objectClassPath && itemJson.value("spritePath").toString()==gItem->path){
                        collidedGameItems<<gItem;
                    }
                }else if(gItem->itemType==GameItemTypes::Sprite){
                    //Condinations:spritePath
                    //Defining sprite path. If sprite type SpriteSheetBased, we define sprite path with frameName method.
                    QString spritePath=itemJson.value("spritePath").toString();
                    if(itemJson.value("spriteType").toString()=="SpriteSheetBased")
                        spritePath=spritePath+"?frameName="+itemJson.value("spriteFrameName").toString();
                    if(spritePath==gItem->path){
                        collidedGameItems<<gItem;
                    }
                }
            }else{
                collidedGameItems<<gItem;
            }
        }
    }
    if(drawCollisionDedection==DrawCollisionDedection::DedectionForAllItems){
        return collidedGameItems;
    }
    return collidedGameItems;
}



//::::::::::::::::::::END::::::::::::::::::::

//::::::::::::::::::::EYEDROPPER EDIT MODE::::::::::::::::::::

void GraphicsScene::EyedropperEditMode_MousePress(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem *item=this->itemAt(mouseEvent->scenePos(),QTransform());
    GameItem *gItem=dynamic_cast<GameItem*>(item);
    if(gItem){
        if(gItem->isLocked){
            return;
        }
        eyedroppedItemJson=gItem->toJsonObject();
        if(scenePanel->eyedropperViewer!=NULL){
            QString sName=eyedroppedItemJson.value("name").toString();
            QString type=eyedroppedItemJson.value("type").toString();
            QString spriteType=eyedroppedItemJson.value("spriteType").toString();
            QString spritePath=eyedroppedItemJson.value("spritePath").toString();
            QString spriteFrameName=eyedroppedItemJson.value("spriteFrameName").toString();
            QString resourcesPath=this->mainWindow->projectFolderPath+this->mainWindow->resourcesPath+"/";
            QPixmap pix;
            if(spritePath==""){
                eyedroppedItemSize=QSizeF(32,32);
                pix=QPixmap(":/Icons/uiIcons_mini_object.png");
            }else{
                if(spriteType=="ImageFileBased"){
                    pix=QPixmap(resourcesPath+"/"+spritePath);
                }else if(spriteType=="SpriteSheetBased"){
                    SpriteSheet ss(resourcesPath+spritePath);
                    QPixmap resourceImage(ss.resourceImagePath);
                    //Cropping sprite frame
                    pix=resourceImage.copy(ss.frame[spriteFrameName]);
                }
                eyedroppedItemSize=pix.size();
                pix=pix.scaled(QSize(16,16));
            }
            scenePanel->eyedropperViewer->setPixmap(pix);
            scenePanel->eyedropperNameField->setText(sName);
        }
        qDebug()<<eyedroppedItemJson;
    }
}

void GraphicsScene::EyedropperEditMode_MouseMove(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPixmap cursor(":/mouseIcons/eyedropper_mouse_cursor.png");
    QCursor nCursor(cursor);
    this->view->setMouseCursor(nCursor);
}

void GraphicsScene::EyedropperEditMode_MouseRelease(QGraphicsSceneMouseEvent *mouseEvent)
{

}

//::::::::::::::::::::END::::::::::::::::::::

void GraphicsScene::clearTransformSelector(bool oneSided,bool savePropItem)
{
    //(i) If hierarchy panel call this function, that need oneSided boolean false for clear hierarchy panel selections.
    if(oneSided!=true){
        QItemSelectionModel* selModel=this->hierarchyPanel->hierarchyList->selectionModel();
        selModel->clearSelection();
    }
    QRectF rect=this->sceneRect();
    if(lastTransformItem!=NULL){
        if(savePropItem==false){
            this->propPanel->ClearGameProperties();
        }
        lastTransformItem->dropItems();
        this->removeItem(lastTransformItem);
        delete lastTransformItem;
        lastTransformItem=NULL;
    }
    this->setSceneRect(rect);
    clearColliderAnchors();
    if(editMode==SceneEditModes::ColliderEdit){
        this->setEditMode(SceneEditModes::GameItemsEdit);
    }
    mainWindow->updateCopyPasteActions();
}

void GraphicsScene::addTransformSelector(QList<QGraphicsItem *> items,PropertiesItem *propItem)
{
    // (i) Connecting hierarchy selection model and selecting hierarchy panel according to selected items
    QItemSelectionModel* selModel=this->hierarchyPanel->hierarchyList->selectionModel();
    for(int i=0;i<items.count();i++){
        GameItem *selectedGameItem=dynamic_cast<GameItem*>(items.at(i));
        if(selectedGameItem){
           selModel->select(selectedGameItem->hierarchyItem->index(),QItemSelectionModel::Select);
           selectedGameItem->prepareTransform();
        }
    }
    //(i) Adding new SelectionTransformItem to scene. And defining that as lastTransformItem
    lastTransformItem=new SelectionTransformItem(items,this,propItem);
    lastSelectedItems=items;
    mainWindow->updateCopyPasteActions();
}

void GraphicsScene::resetPropItem()
{
    if(lastTransformItem!=NULL){
        qDebug()<<"last transform var";
        propPanel->ClearGameProperties();
        PropertiesItem *propItem=new PropertiesItem(lastTransformItem,this->propPanel);
        this->propPanel->AddGameProperties(propItem);
        qDebug()<<propItem;
        lastTransformItem->propItem=propItem;
    }
}


QMap<QString, bool> GraphicsScene::setMouseIcons(QGraphicsSceneMouseEvent *mouseEvent)
{
    //(i) Creating QMap object and saving transform,scale,rotation icon values as boolen. Icon properties executing from SelectionTransformItems functions.
    QMap<QString, bool> results;
    bool selectedScale=false,selectedTransform=false,selectedRotate=false;
    if(lastTransformItem!=NULL){
        selectedScale=lastTransformItem->checkScaleCursor(mouseEvent);
        if(selectedScale==false){
            selectedTransform=lastTransformItem->checkTransformCursor(mouseEvent);
            if(selectedTransform==false){
                selectedRotate=lastTransformItem->checkRotateCursor(mouseEvent);
                if(selectedRotate==false){
                    this->view->setMouseCursor(QCursor(Qt::ArrowCursor));
                }
            }

        }
    }
    results.insert("selectedScale",selectedScale);
    results.insert("selectedTransform",selectedTransform);
    results.insert("selectedRotate",selectedRotate);
    return results;
}



QPointF GraphicsScene::snapFilter(QPointF point,QSizeF _gridSize,bool alignCenter)
{
    float x=round(point.x()/_gridSize.width())*_gridSize.width();
    float y=round(point.y()/_gridSize.height())*_gridSize.height();

    float deltaXnegative=qAbs((x-gridSize.width()/2)-point.x());
    float deltaXpositive=qAbs((x+gridSize.width()/2)-point.x());
    float deltaYnegative=qAbs((y-gridSize.height()/2)-point.y());
    float deltaYpositive=qAbs((y+gridSize.height()/2)-point.y());

    int xFactor=deltaXnegative<deltaXpositive ? -1:1;
    int yFactor=deltaYnegative<deltaYpositive ? -1:1;

    x=alignCenter==true ? x+xFactor*gridSize.width()/2:x;
    y=alignCenter==true ? y+yFactor*gridSize.height()/2:y;
    return QPointF(x,y);
}

void GraphicsScene::rePosForSnap(GameItem *item)
{
    QRectF rect=item->boundingRect();
    QPointF p=item->scenePos();
    float left=int(p.x()+rect.left())%int(gridSize.width());
    float right=int(p.x()+rect.right())%int(gridSize.width());
    float top=int(p.y()+rect.top())%int(gridSize.height());
    float bottom=int(p.y()+rect.bottom())%int(gridSize.height());

    qDebug()<<"left:"<<left<<" right:"<<right<<" top:"<<top<<" bottom:"<<bottom;
}

void GraphicsScene::rePosForSnap(SelectionTransformItem *item)
{
    if(isSnapMode==false){
        return;
    }else if(snapStyle==SnapStyle::withCenter){
        item->setPos(snapFilter(item->scenePos(),gridSize,true));
        item->UpdateContainer();
        return;
    }
    //Snap with bound
    QRectF rect=*item->currentRect;
    QPointF p=item->scenePos();
    float left=int(p.x()+rect.left())%int(gridSize.width());
    float right=int(p.x()+rect.right())%int(gridSize.width());
    float top=int(p.y()+rect.top())%int(gridSize.height());
    float bottom=int(p.y()+rect.bottom())%int(gridSize.height());

    qDebug()<<"left:"<<rect.left()<<" right:"<<rect.right()<<" top:"<<rect.top()<<" bottom:"<<rect.bottom();
    QPointF delta;
    if(qAbs(left)<qAbs(right)){
        delta.setX(left);
    }else{
        delta.setX(right);
    }
    if(qAbs(top)<qAbs(bottom)){
        delta.setY(top);
    }else{
        delta.setY(bottom);
    }
    item->setPos(item->scenePos()-delta);
    item->UpdateContainer();

}


//Snap Filter with point

void GraphicsScene::addColliderAnchors(QVariantMap *collider, GameItem *gameItem, ColliderShapeItem *inspectorItem)
{
    ColliderAnchor* anchorLeft=new ColliderAnchor(collider,"l",gameItem,inspectorItem);
    anchorLeft->setZValue(99999999999);
    ColliderAnchor* anchorTop=new ColliderAnchor(collider,"t",gameItem,inspectorItem);
    anchorTop->setZValue(99999999999);
    ColliderAnchor* anchorRight=new ColliderAnchor(collider,"r",gameItem,inspectorItem);
    anchorRight->setZValue(99999999999);
    ColliderAnchor* anchorBottom=new ColliderAnchor(collider,"b",gameItem,inspectorItem);
    anchorBottom->setZValue(99999999999);
    colliderAnchors->append(anchorLeft);
    colliderAnchors->append(anchorTop);
    colliderAnchors->append(anchorRight);
    colliderAnchors->append(anchorBottom);
    lastEditedCollider=collider;
}

void GraphicsScene::clearColliderAnchors()
{
    for(int i=0;i<colliderAnchors->count();i++){
        this->removeItem(colliderAnchors->at(i));
    }
    colliderAnchors->clear();
    lastEditedCollider=NULL;
}

void GraphicsScene::updateColliderAnchors()
{
    for(int i=0;i<colliderAnchors->count();i++){
        colliderAnchors->at(i)->updatePositions();
    }
    this->views().at(0)->update();
}

QJsonObject GraphicsScene::toJsonObject()
{
    QString snapStyleStr=snapStyle==SnapStyle::withBounds ? "withBounds":"withCenter";
    QJsonObject sceneProperties{
        {"type","Turquoise2D Scene"},
        {"gridSize",QJsonArray{gridSize.width(),gridSize.height()}},
        {"isSnapMode",isSnapMode},
        {"snapStyle",snapStyleStr}
    };
    QJsonObject resultObject{
        {"sceneProperties",sceneProperties},
        {"objects",this->hierarchyPanel->hierarchyList->objectsToJsonArray()},
        {"sprites",this->hierarchyPanel->hierarchyList->spritesToJsonArray()}
    };
    qDebug()<<resultObject;
    return resultObject;
}

QJsonObject *GraphicsScene::fromJsonFile(QString filePath)
{

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"scene dosyasi acilamadi";
    }
    QString val=file.readAll();
    file.close();
    QJsonDocument doc=QJsonDocument::fromJson(val.toUtf8());
    QJsonObject *rootObj=new QJsonObject(doc.object());
    return rootObj;
}



void GraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
     event->acceptProposedAction();
}
void GraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
     event->acceptProposedAction();
}

void GraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // (i) if the user drag file to scene, calling hierarchy panel AddItem function. Because all items are managing by hierarchy panel.
    QList<QUrl> urls=event->mimeData()->urls();
    //Checking file types
    QList<QUrl> localUrls;
    for(int i=0;i<urls.count();i++){
        QString urlPath=urls.at(i).toString();
        //Getting local project url and adding localUrls list
        localUrls.append(urlPath.split(this->mainWindow->resourcesPath+"/").at(1));
        if(urlPath.indexOf("?frameName=")>0){
            continue;
        }
        QString urlType=QFileInfo(urls.at(i).path()).suffix();
        qDebug()<<urlType;
        if(urlType!="jpg" && urlType!="png" && urlType!="jpeg"){
            return;
        }
    }
    QUndoCommand *cmd=new AddSpriteItemCommand(this,localUrls,event->scenePos());
    stack->push(cmd);
    sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    //Setting editmode and clearing collider anhors if collider anchors are in the scene
    this->clearColliderAnchors();
    this->setEditMode(SceneEditModes::GameItemsEdit);
    this->mainWindow->selectToolWithEditMode(SceneEditModes::GameItemsEdit);
    event->acceptProposedAction();
}
