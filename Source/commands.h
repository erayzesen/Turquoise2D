#ifndef COMMANDS_H
#define COMMANDS_H
#include <QUndoCommand>
#include "Hierarchy/hierarchylistview.h"
#include "QUrl"
#include "QRectF"
#include "QJsonArray"

// ADD JSON BASED ITEM COMMAND
class GameItem;
enum class GameItemTypes;
class GraphicsScene;
class HierarchyListView;
class AddJsonBasedItemCommand : public QUndoCommand
{
public:
    QPointF _pos;
    GraphicsScene *_scene;
    QList<QJsonObject> _jsonObjects;
    QList<GameItem*> items;
    AddJsonBasedItemCommand(GraphicsScene *scene,QList<QJsonObject> jsonObjects,bool toNewPosition=false,QPointF pos=QPoint(0,0),QUndoCommand *parent = 0) : QUndoCommand(parent) {
        // Saving general properties
        _scene=scene;
        _pos=pos;
        _jsonObjects=jsonObjects;
        //(i) Getting all game items from url list and adding 'items' named GameItem list.
        for(int i=0;i<_jsonObjects.count();i++){
            //Getting properties
            QJsonObject jsonObject=_jsonObjects.at(i);
            QString _name=jsonObject.value("name").toString();
            QString _typeStr=jsonObject.value("type").toString();
            GameItemTypes _type;
            if(_typeStr=="sprite"){
                _type=GameItemTypes::Sprite;
            }else if(_typeStr=="object"){
                _type=GameItemTypes::Object;
            }
            //Creating hierarchy Item and GameItem
            HierarchyItem  *hItem=new HierarchyItem(_name,_type);
            GameItem *gItem=new GameItem(jsonObject,hItem,_scene);
            //Setting properties
            if(toNewPosition==true){
                gItem->setPos(_pos);
            }else{
                QJsonArray jPositionArr=jsonObject.value("position").toArray();
                gItem->setPos(jPositionArr.at(0).toDouble(),-jPositionArr.at(1).toDouble());
            }
            gItem->setOffset(-0.5*QPointF(gItem->pixmap().size().width(),gItem->pixmap().size().height()));
            hItem->gameItem=gItem;
            gItem->hierarchyItem=hItem;
            //Adding items to the items list
            items.append(gItem);
        }


    }
    ~AddJsonBasedItemCommand(){
        //
    }
    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

};


// ADD SPRITE ITEM COMMAND
class GameItem;
enum class GameItemTypes;
class GraphicsScene;
class HierarchyListView;
class AddSpriteItemCommand : public QUndoCommand
{
public:
    QPointF _pos;
    QPointF pos;
    GraphicsScene *_scene;
    QList<QUrl> _urls;
    QList<GameItem*> items;
    AddSpriteItemCommand(GraphicsScene *scene,QList<QUrl> urls,QPointF pos,QUndoCommand *parent = 0) : QUndoCommand(parent) {
        // Saving general properties
        _scene=scene;
        _pos=pos;
        _urls=urls;
        //(i) Getting all game items from url list and adding 'items' named GameItem list.
        for(int i=0;i<_urls.count();i++){
            //Getting properties
            QString _path=_urls.at(i).toString();
            QString _name;
            //Checking Spritesheet Factor
            if(_path.indexOf("?frameName=")>0){
                QStringList splittedPath=_path.split("?frameName=");
                _name=splittedPath.at(1);
            }else{
                _name=QFileInfo(_urls.at(i).url()).baseName();
                _path=_urls.at(i).path();
            }
            //Creating hierarchy Item and GameItem
            HierarchyItem  *hItem=new HierarchyItem(_name,GameItemTypes::Sprite);
            GameItem *gItem=new GameItem(_name,hItem,scene,_path);
            //Setting properties
            gItem->setPos(_pos);
            gItem->setOffset(-0.5*QPointF(gItem->pixmap().size().width(),gItem->pixmap().size().height()));
            hItem->gameItem=gItem;
            gItem->hierarchyItem=hItem;
            //Adding items to the items list
            items.append(gItem);
        }


    }
    ~AddSpriteItemCommand(){
        //
    }
    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

};

// ADD OBJECT ITEM COMMAND
class GameItem;
enum class GameItemTypes;
class GraphicsScene;
class AddObjectItemCommand : public QUndoCommand
{
public:
    QPointF _pos;
    GraphicsScene *_scene;
    GameItem* item;
    AddObjectItemCommand(GraphicsScene *scene,QPointF pos,QUndoCommand *parent = 0) : QUndoCommand(parent) {
        // Saving general properties
        _scene=scene;
        _pos=pos;
        //(i) Creating Object.
        QString _name="EmptyObject";
        HierarchyItem  *hItem=new HierarchyItem(_name,GameItemTypes::Object);
        GameItem *gItem=new GameItem(_name,hItem,_scene,"");
        //Setting properties
        gItem->setPos(_pos);
        gItem->setOffset(-0.5*QPointF(gItem->pixmap().size().width(),gItem->pixmap().size().height()));
        hItem->gameItem=gItem;
        //Adding items to the items list
        item=gItem;
    }
    ~AddObjectItemCommand(){
        //
    }
    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

};




//REMOVE ITEM COMMAND
class GraphicsScene;
class RemoveItemCommand : public QUndoCommand
{
public:
    GraphicsScene *_scene;
    QList<QGraphicsItem*> _items;
    QList<GameItem*> removedItems;
    RemoveItemCommand(GraphicsScene *scene,QList<QGraphicsItem*> items,QUndoCommand *parent = 0) : QUndoCommand(parent) {
        // Saving item properties
        _scene=scene;
        _items=items;

    }

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

    GameItemTypes GetItemType(QString path){
        QString suffix=QFileInfo(path).suffix();
        if(suffix=="png" || suffix=="jpg" || suffix=="jpeg"){
            return GameItemTypes::Sprite;
        }else if(suffix=="t2dobj"){
            return GameItemTypes::Object;
        }
        return GameItemTypes::Sprite;
    }

};




//MOVE ITEM COMMAND
class MoveItemCommand : public QUndoCommand
{
public:
    QPointF _firstPos;
    QPointF _finalPos;
    GraphicsScene *_scene;
    QList<QGraphicsItem*> _items;
    bool beginning;
    MoveItemCommand(GraphicsScene *scene,QList<QGraphicsItem*> items,QPointF firstPos,QPointF finalPos,bool beginningMode=true,QUndoCommand *parent = 0) : QUndoCommand(parent) {
        // Saving item properties
        _scene=scene;
        _items=items;
        _firstPos=firstPos;
        _finalPos=finalPos;
        beginning=beginningMode;

    }

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;


};

//SCALE ITEM COMMAND
class ScaleItemCommand : public QUndoCommand
{
public:
    QSizeF _firstScale;
    QSizeF _finalScale;
    GraphicsScene *_scene;
    QList<QGraphicsItem*> _items;
    bool beginning;
    ScaleItemCommand(GraphicsScene *scene,QList<QGraphicsItem*> items,QSizeF firstScale,QSizeF finalScale,bool beginningMode=true,QUndoCommand *parent = 0) : QUndoCommand(parent) {
        // Saving item properties
        _scene=scene;
        _items=items;
        _firstScale=firstScale;
        _finalScale=finalScale;
        beginning=beginningMode;

    }

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;


};

//ROTATE ITEM COMMAND
class RotateItemCommand : public QUndoCommand
{
public:
    float _deltaAngle;
    GraphicsScene *_scene;
    QList<QGraphicsItem*> _items;
    bool beginning;

    RotateItemCommand(GraphicsScene *scene,QList<QGraphicsItem*> items,float deltaAngle,bool beginningMode=true,QUndoCommand *parent = 0) : QUndoCommand(parent) {
        // Saving item properties
        _scene=scene;
        _items=items;
        _deltaAngle=deltaAngle;
        beginning=beginningMode;

    }

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;


};

//SET MULTIPLE ITEM POSX COMMAND
class SetMultipleItemPosX : public QUndoCommand
{
public:
    float _targetPosX;
    GraphicsScene *_scene;
    QList<QGraphicsItem*> _items;
    QMap<QGraphicsItem*,float> oldPositionsX;
    bool beginning;

    SetMultipleItemPosX(GraphicsScene *scene,QList<QGraphicsItem*> items,float targetPosX,bool beginningMode=true,QUndoCommand *parent = 0) : QUndoCommand(parent) {
        // Saving item properties
        _scene=scene;
        _items=items;
        _targetPosX=targetPosX;
        beginning=beginningMode;
        for(int i=0;i<items.count();i++){
            oldPositionsX.insert(items.at(i),items.at(i)->scenePos().x());
        }

    }

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;


};
//SET MULTIPLE ITEM POSY COMMAND
class SetMultipleItemPosY : public QUndoCommand
{
public:
    float _targetPosY;
    GraphicsScene *_scene;
    QList<QGraphicsItem*> _items;
    QMap<QGraphicsItem*,float> oldPositionsY;
    bool beginning;

    SetMultipleItemPosY(GraphicsScene *scene,QList<QGraphicsItem*> items,float targetPosY,bool beginningMode=true,QUndoCommand *parent = 0) : QUndoCommand(parent) {
        // Saving item properties
        _scene=scene;
        _items=items;
        _targetPosY=targetPosY;
        beginning=beginningMode;
        for(int i=0;i<items.count();i++){
            oldPositionsY.insert(items.at(i),items.at(i)->scenePos().y());
        }

    }

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;


};

//SET MULTIPLE ITEM SPRITE COMMAND
class SetMultipleItemSprite : public QUndoCommand
{
public:
    QString _targetPath;
    GraphicsScene *_scene;
    QList<QGraphicsItem*> _items;
    QMap<QGraphicsItem*,QString> oldPathes;
    bool beginning;

    SetMultipleItemSprite(GraphicsScene *scene,QList<QGraphicsItem*> items,QString targetPath,bool beginningMode=true,QUndoCommand *parent = 0) : QUndoCommand(parent) {
        // Saving item properties
        _scene=scene;
        _items=items;
        _targetPath=targetPath;
        beginning=beginningMode;
        for(int i=0;i<items.count();i++){
            GameItem* toGameItem=dynamic_cast<GameItem*>(items.at(i));
            if(toGameItem){
                oldPathes.insert(items.at(i),toGameItem->path);
            }

        }

    }

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;


};

#endif // COMMANDS_H
