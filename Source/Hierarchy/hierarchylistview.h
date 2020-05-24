#ifndef HIERARCHYLISTVIEW_H
#define HIERARCHYLISTVIEW_H

#include <QWidget>
#include "QApplication"
#include "Canvas/graphicsscene.h"
#include "hierarchyitem.h"
#include "QTreeView"
#include "groupitem.h"

class GraphicsScene;
class GameItem;
class HierarchyListView : public QTreeView
{
public:
    HierarchyListView(QJsonObject *sceneJsonObject,GraphicsScene *_scene,QWidget *parent=0);
    GraphicsScene *scene;
    QStandardItemModel *itemModel;
    void isSelectedChanged();
    ~HierarchyListView();
    QString setUniqueName(QString name,CategoryTypes itemType);
    //Default Parts
    void AddStandartParts();
    GroupItem *sprites;
    GroupItem *objects;
    void expandGroup(CategoryTypes groupType);
    //Item add delete operations
    void addItem(QString itemName,GameItemTypes itemType,QJsonObject jsonObject);
    //Json Helpers
    QJsonArray objectsToJsonArray();
    QJsonArray spritesToJsonArray();
    void fromJsonObject(QJsonObject *sceneJsonObject);
    //Lock operation
    void setItemsLock(QList<HierarchyItem*> itemList,bool lock=true);
};

#endif // HIERARCHYLISTVIEW_H
