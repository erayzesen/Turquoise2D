#include "commands.h"
// ADD JSON BASED ITEM COMMAND
void AddJsonBasedItemCommand::undo()
{
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    for(int i=0;i<items.count();i++){
        GameItem *gItem=items.at(i);
        HierarchyItem *hItem=gItem->hierarchyItem;
        _scene->hierarchyPanel->hierarchyList->sprites->removeRow(hItem->row());
        _scene->removeItem(gItem);
    }
    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

void AddJsonBasedItemCommand::redo()
{
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    QList<QGraphicsItem*> gItemsTemp;
    QRectF rect=_scene->sceneRect();
    for(int i=0;i<items.count();i++){
        GameItem *gItem=items.at(i);
        CategoryTypes cType;
        if(gItem->itemType==GameItemTypes::Sprite){
            cType=CategoryTypes::Sprites;
        }else if(gItem->itemType==GameItemTypes::Object){
            cType=CategoryTypes::Objects;
        }

        gItem->name=_scene->hierarchyPanel->hierarchyList->setUniqueName(gItem->name,cType);
        //Getting path and name values from GameItem
        QString _path=gItem->path;
        QString _name=gItem->name;
        GameItemTypes _type=gItem->itemType;
        //Creating new HierarcyItem object
        HierarchyItem *hItem=new HierarchyItem(_name,_type);

        gItem->hierarchyItem=hItem;
        hItem->gameItem=gItem;


        //Adding Hieararchy Item to Hierarchy List Model
        GroupItem *validGroup=_scene->hierarchyPanel->hierarchyList->sprites;
        validGroup=cType==CategoryTypes::Objects ? _scene->hierarchyPanel->hierarchyList->objects:validGroup;
        validGroup->appendRow(hItem);
        _scene->hierarchyPanel->hierarchyList->expandGroup(cType);
        //Adding items to scene
        _scene->addItem(gItem);
        gItemsTemp<<dynamic_cast<QGraphicsItem*>(gItem);
    }
    _scene->setSceneRect(rect);
    if(_scene->editMode==SceneEditModes::GameItemsEdit){
        _scene->addTransformSelector(gItemsTemp);
        _scene->rePosForSnap(_scene->lastTransformItem);
    }
    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}


//ADD SPRITE ITEM COMMAND
void AddSpriteItemCommand::undo()
{
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    for(int i=0;i<items.count();i++){
        GameItem *gItem=items.at(i);
        HierarchyItem *hItem=gItem->hierarchyItem;
        _scene->hierarchyPanel->hierarchyList->sprites->removeRow(hItem->row());
        _scene->removeItem(gItem);
    }
    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

void AddSpriteItemCommand::redo()
{
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    QList<QGraphicsItem*> gItemsTemp;
    QRectF rect=_scene->sceneRect();
    for(int i=0;i<items.count();i++){
        GameItem *gItem=items.at(i);
        gItem->name=_scene->hierarchyPanel->hierarchyList->setUniqueName(gItem->name,CategoryTypes::Sprites);
        //Getting path and name values from GameItem
        QString _path=gItem->path;
        QString _name=gItem->name;
        GameItemTypes _type=gItem->itemType;
        //Creating new HierarcyItem object
        HierarchyItem *hItem=new HierarchyItem(_name,_type);

        gItem->hierarchyItem=hItem;
        hItem->gameItem=gItem;


        //Adding Hieararchy Item to Hierarchy List Model
        _scene->hierarchyPanel->hierarchyList->sprites->appendRow(hItem);
        _scene->hierarchyPanel->hierarchyList->expandGroup(CategoryTypes::Sprites);
        //Adding items to scene
        _scene->addItem(gItem);
        gItemsTemp<<dynamic_cast<QGraphicsItem*>(gItem);
    }
    _scene->setSceneRect(rect);
    _scene->addTransformSelector(gItemsTemp);
    _scene->rePosForSnap(_scene->lastTransformItem);
    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

//ADD OBJECT ITEM COMMAND

void AddObjectItemCommand::undo()
{
    //Undo Add Object Item
    HierarchyItem *hItem=item->hierarchyItem;
    _scene->hierarchyPanel->hierarchyList->objects->removeRow(hItem->row());
    _scene->removeItem(item);
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

void AddObjectItemCommand::redo()
{
    QRectF rect=_scene->sceneRect();
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    //Redo Add Object Item
    item->name=_scene->hierarchyPanel->hierarchyList->setUniqueName(item->name,CategoryTypes::Objects);
    HierarchyItem *hItem=new HierarchyItem(item->name,item->itemType);
    item->hierarchyItem=hItem;
    hItem->gameItem=item;
    _scene->hierarchyPanel->hierarchyList->objects->appendRow(hItem);
    _scene->hierarchyPanel->hierarchyList->expandGroup(CategoryTypes::Objects);
    _scene->addItem(item);

    _scene->setSceneRect(rect);
    _scene->addTransformSelector(QList<QGraphicsItem*>()<<item);
    _scene->rePosForSnap(_scene->lastTransformItem);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}


//REMOVE ITEM COMMAND
void RemoveItemCommand::undo()
{
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    QList<QGraphicsItem*> gItemsTemp;
    for(int i=0;i<removedItems.count();i++){
        GameItem *gItem=removedItems.at(i);
        HierarchyItem *hItem=new HierarchyItem(gItem->name,gItem->itemType);
        GroupItem *group;
        if(gItem->itemType==GameItemTypes::Object){
            group=_scene->hierarchyPanel->hierarchyList->objects;
        }else if(gItem->itemType==GameItemTypes::Sprite){
            group=_scene->hierarchyPanel->hierarchyList->sprites;
        }
        group->appendRow(hItem);
        gItem->hierarchyItem=hItem;
        hItem->gameItem=gItem;
        _scene->addItem(gItem);
        gItemsTemp<<dynamic_cast<QGraphicsItem*>(gItem);
    }
    _scene->addTransformSelector(gItemsTemp);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

void RemoveItemCommand::redo()
{
    QRectF rect=_scene->sceneRect();
    removedItems.clear();
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    for(int i=0;i<_items.count();i++){
        //Creating items
        GameItem *gItem=dynamic_cast<GameItem*>(_items.at(i));
        HierarchyItem *hItem=gItem->hierarchyItem;
        //Setting GameItem and HierarchyItem properties
        _scene->removeItem(gItem);
        GroupItem *group;
        if(gItem->itemType==GameItemTypes::Object){
            group=_scene->hierarchyPanel->hierarchyList->objects;
        }else if(gItem->itemType==GameItemTypes::Sprite){
            group=_scene->hierarchyPanel->hierarchyList->sprites;
        }
        group->removeRow(hItem->row());

        removedItems.append(gItem);
        //Adding Hieararchy Item to Hierarchy List Model
    }
    _scene->setSceneRect(rect);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

//MOVE ITEM COMMAND
void MoveItemCommand::undo()
{
    if(beginning==true){
        beginning=false;
        return;
    }
    QRectF rect=_scene->sceneRect();
    //Closing active transform selector
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    //Adding new transform selector and setting the position of items.
    _scene->addTransformSelector(_items);
    if(_scene->lastTransformItem!=NULL){
        _scene->lastTransformItem->setPos(_firstPos);
        _scene->lastTransformItem->propItem->UpdateTransformProp();
        _scene->lastTransformItem->UpdateContainer();
        _scene->lastTransformItem->updateOrigin();
    }
    _scene->setSceneRect(rect);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings

}

void MoveItemCommand::redo()
{
    if(beginning==true){
        beginning=false;
        return;
    }
    QRectF rect=_scene->sceneRect();
    //Closing active transform selector
    if(_scene->lastTransformItem!=NULL){
        if(_scene->lastTransformItem->scenePos()==_finalPos){
            return;
        }
        _scene->clearTransformSelector();
    }
    //Adding new transform selector and setting the position of items.
    _scene->addTransformSelector(_items);
    if(_scene->lastTransformItem!=NULL){
        _scene->lastTransformItem->setPos(_finalPos);
        _scene->lastTransformItem->propItem->UpdateTransformProp();
        _scene->lastTransformItem->UpdateContainer();
    }
    _scene->setSceneRect(rect);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

//SCALE ITEM COMMAND
void ScaleItemCommand::undo()
{
    if(beginning==true){
        beginning=false;
        return;
    }
    QRectF rect=_scene->sceneRect();
    //Closing active transform selector
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    //Adding new transform selector.
    _scene->addTransformSelector(_items);
    //(i)Scale ratio is finalScale/firstScale. Example, first scale 1, final scale 4, scale ratio=4/1=4. The items scaled 1/4 because of undo operation;
    QSizeF scaleRatio=QSizeF(_finalScale.width()/_firstScale.width(),_finalScale.height()/_firstScale.height());
    if(_scene->lastTransformItem!=NULL){
        QTransform trans;
        trans.scale(1/scaleRatio.width(),1/scaleRatio.width());
        _scene->lastTransformItem->setTransform(trans);
        _scene->lastTransformItem->applyScales();
        _scene->lastTransformItem->UpdateBounds();
        _scene->lastTransformItem->propItem->UpdateTransformProp();
        _scene->lastTransformItem->UpdateContainer();
    }
    _scene->setSceneRect(rect);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

void ScaleItemCommand::redo()
{
    if(beginning==true){
        beginning=false;
        return;
    }
    QRectF rect=_scene->sceneRect();
    QSizeF currentSize(_scene->lastTransformItem->transform().m11(),_scene->lastTransformItem->transform().m22());
    //Closing active transform selector
    if(_scene->lastTransformItem!=NULL){
        if(currentSize==_finalScale){
            return;
        }
        _scene->clearTransformSelector();
    }
    //Adding new transform selector.
    _scene->addTransformSelector(_items);
    QSizeF scaleRatio=QSizeF(_finalScale.width()/_firstScale.width(),_finalScale.height()/_firstScale.height());
    if(_scene->lastTransformItem!=NULL){
        QTransform trans;
        trans.scale(scaleRatio.width(),scaleRatio.height());
        _scene->lastTransformItem->setTransform(trans);
        _scene->lastTransformItem->applyScales();
        _scene->lastTransformItem->UpdateBounds();
        _scene->lastTransformItem->propItem->UpdateTransformProp();
        _scene->lastTransformItem->UpdateContainer();
    }
    _scene->setSceneRect(rect);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

//ROTATE ITEM COMMAND
void RotateItemCommand::undo()
{
    if(beginning==true){
        beginning=false;
        return;
    }
    QRectF rect=_scene->sceneRect();
    //Closing active transform selector
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    //Adding new transform selector.
    _scene->addTransformSelector(_items);

    _scene->lastTransformItem->rotateTransform(-_deltaAngle);

    _scene->clearTransformSelector();

    _scene->addTransformSelector(_items);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings

}

void RotateItemCommand::redo()
{
    if(beginning==true){
        beginning=false;
        return;
    }
    qDebug()<<"asdas"<<_deltaAngle;
    QRectF rect=_scene->sceneRect();
    //Closing active transform selector
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    //Adding new transform selector.
    _scene->addTransformSelector(_items);

    _scene->lastTransformItem->rotateTransform(_deltaAngle);

    _scene->clearTransformSelector();

    _scene->addTransformSelector(_items);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

//SET MULTIPLE ITEM POSITION X COMMAND

void SetMultipleItemPosX::undo()
{
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    for(int i=0;i<_items.count();i++){
        QGraphicsItem *item=_items.at(i);
        //Setting old positions with getting from oldPositionX QMap
        item->setPos(oldPositionsX[item],item->pos().y());
    }

    _scene->addTransformSelector(_items);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings

}

void SetMultipleItemPosX::redo()
{
    // Saving propItem for using when create new transformSelector. (Otherwise we get bad_exc error)
    PropertiesItem* tempProp=NULL;
    if(beginning==true){
        if(_scene->lastTransformItem!=NULL){
            tempProp=_scene->lastTransformItem->propItem;
            //Calling clearTransformSelector with saving properties item
            _scene->clearTransformSelector(false,true);
        }
        beginning=false;

    }else{
        if(_scene->lastTransformItem!=NULL){
            _scene->clearTransformSelector();
        }
    }

    //Setting new positions
    for(int i=0;i<_items.count();i++){
        QGraphicsItem *item=_items.at(i);
        item->setPos(_targetPosX,item->pos().y());
    }
    _scene->addTransformSelector(_items,tempProp);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings

}
//SET MULTIPLE ITEM POSY COMMAND

void SetMultipleItemPosY::undo()
{
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    for(int i=0;i<_items.count();i++){
        QGraphicsItem *item=_items.at(i);
        //Setting old positions with getting from oldPositionX QMap
        item->setPos(item->pos().x(),oldPositionsY[item]);
    }

    _scene->addTransformSelector(_items);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

void SetMultipleItemPosY::redo()
{
    PropertiesItem* tempProp=NULL;
    if(beginning==true){
        if(_scene->lastTransformItem!=NULL){
            tempProp=_scene->lastTransformItem->propItem;
            //Calling clearTransformSelector with saving properties item
            _scene->clearTransformSelector(false,true);
        }
        beginning=false;

    }else{
        if(_scene->lastTransformItem!=NULL){
            _scene->clearTransformSelector();
        }
    }

    //Setting new positions
    for(int i=0;i<_items.count();i++){
        QGraphicsItem *item=_items.at(i);
        item->setPos(item->pos().x(),_targetPosY);
    }
    _scene->addTransformSelector(_items,tempProp);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

//SET MULTIPLE ITEM SPRITE COMMAND

void SetMultipleItemSprite::undo()
{
    if(_scene->lastTransformItem!=NULL){
        _scene->clearTransformSelector();
    }
    for(int i=0;i<_items.count();i++){
        QGraphicsItem *item=_items.at(i);
        GameItem *gItem=dynamic_cast<GameItem*>(item);
        //Setting old positions with getting from oldPositionX QMap
        if(gItem){
            if(oldPathes[item]!=""){
                gItem->setSprite(oldPathes[item]);
            }else{
                gItem->setTransparentSprite(gItem->scene->gridSize);
            }
        }
    }

    _scene->addTransformSelector(_items);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}

void SetMultipleItemSprite::redo()
{
    PropertiesItem* tempProp=NULL;
    if(beginning==true){
        if(_scene->lastTransformItem!=NULL){
            tempProp=_scene->lastTransformItem->propItem;
            //Calling clearTransformSelector with saving properties item
            _scene->clearTransformSelector(false,true);
        }
        beginning=false;

    }else{
        if(_scene->lastTransformItem!=NULL){
            _scene->clearTransformSelector();
        }
    }

    //Setting new positions
    for(int i=0;i<_items.count();i++){
        GameItem *gItem=dynamic_cast<GameItem*>(_items.at(i));
        if(gItem){
            if(_targetPath!=""){
                gItem->setSprite(_targetPath);
            }else{
                gItem->setTransparentSprite(QSizeF(32,32));
            }
        }
    }
    _scene->addTransformSelector(_items,tempProp);

    _scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
}



