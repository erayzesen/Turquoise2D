#include "hierarchylistview.h"

HierarchyListView::HierarchyListView(QJsonObject *sceneJsonObject,GraphicsScene *_scene,QWidget *parent)
{
    scene=_scene;
    itemModel=new QStandardItemModel(this);
    this->setModel(itemModel);
    this->setColumnHidden(1,true);
    this->setColumnHidden(2,true);
    this->setColumnHidden(3,true);
    this->setHeaderHidden(true);
    this->setObjectName("hierarchyList");
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //Adding standart sprites and objects group items
    AddStandartParts();
    //Adding all items from scene json object
    fromJsonObject(sceneJsonObject);
    //this->setDragEnabled(true);
    //this->setDragDropMode(QAbstractItemView ::InternalMove);
    connect(this->selectionModel(),&QItemSelectionModel::selectionChanged,this,&HierarchyListView::isSelectedChanged);
}

void HierarchyListView::fromJsonObject(QJsonObject *sceneJsonObject)
{
    QJsonArray objectsListJson=sceneJsonObject->value("objects").toArray();
    QJsonArray spritesListJson=sceneJsonObject->value("sprites").toArray();
    for(int i=0;i<objectsListJson.count();i++){
        QJsonObject nObject=objectsListJson.at(i).toObject();
        QString itemName=nObject.value("name").toString();
        this->addItem(itemName,GameItemTypes::Object,nObject);
    }
    for(int i=0;i<spritesListJson.count();i++){
        QJsonObject nObject=spritesListJson.at(i).toObject();
        QString itemName=nObject.value("name").toString();
        this->addItem(itemName,GameItemTypes::Sprite,nObject);
    }
    qDebug()<<"hiyerarchy list created with json object";
}

void HierarchyListView::setItemsLock(QList<HierarchyItem*> itemList,bool lock)
{
    for(int i=0;i<itemList.count();i++){
        GameItem *gi=itemList.at(i)->gameItem;
        gi->isLocked=lock;
        HierarchyItem *hi=itemList.at(i);
        QIcon nIcon=lock ? QIcon(":/Icons/item_lock_type_16x16.png"):QIcon(":/Icons/item_type_16x16.png");
        hi->setIcon(nIcon);
    }
}

void HierarchyListView::addItem(QString itemName, GameItemTypes itemType, QJsonObject jsonObject)
{
    HierarchyItem *hItem=new HierarchyItem(itemName,itemType);
    GameItem *gItem=new GameItem(jsonObject,hItem,scene);
    hItem->gameItem=gItem;
    gItem->hierarchyItem=hItem;
    GroupItem *validGroup=itemType==GameItemTypes::Sprite ? sprites:objects;
    validGroup->appendRow(hItem);
    scene->addItem(gItem);
}

HierarchyListView::~HierarchyListView(){

}

QString HierarchyListView::setUniqueName(QString name,CategoryTypes itemType)
{
    qDebug()<<"setting unique name:"<<name;
    GroupItem *cg;
    if(itemType==CategoryTypes::Objects){
        cg=objects;
    }else if(itemType==CategoryTypes::Sprites){
        cg=sprites;
    }
    qDebug()<<cg->model()->findItems(name,Qt::MatchRecursive).count();
    if(cg->model()->findItems(name,Qt::MatchRecursive).count()==0){
        return name;
    }
    QString newName=name;
    int n=0;
    while(cg->model()->findItems(newName,Qt::MatchRecursive).count()!=0){
        n++;
        newName=name+"_"+QString::number(n);
        //qDebug()<<"tried: "<<newName;
    }
    return newName;
}

void HierarchyListView::AddStandartParts()
{
    objects=new GroupItem("Objects",CategoryTypes::Objects);
    itemModel->appendRow(objects);
    sprites=new GroupItem("Sprites",CategoryTypes::Sprites);
    itemModel->appendRow(sprites);
    this->expandAll();
}

void HierarchyListView::expandGroup(CategoryTypes groupType)
{
    GroupItem *cg;
    if(groupType==CategoryTypes::Objects){
        cg=objects;
    }else if(groupType==CategoryTypes::Sprites){
        cg=sprites;
    }
    if(!this->isExpanded(cg->index())){
        this->expand(cg->index());
    }
}



QJsonArray HierarchyListView::objectsToJsonArray()
{
    QJsonArray resultArray;
    for(int i=0;i<objects->rowCount();i++){
        HierarchyItem *hi=dynamic_cast<HierarchyItem*>(objects->child(i));
        if(hi){
            GameItem *gi=hi->gameItem;
            resultArray.append(gi->toJsonObject());
        }


    }
    return resultArray;
}

QJsonArray HierarchyListView::spritesToJsonArray()
{
    QJsonArray resultArray;
    for(int i=0;i<sprites->rowCount();i++){
        HierarchyItem *hi=dynamic_cast<HierarchyItem*>(sprites->child(i));
        if(hi){
            GameItem *gi=hi->gameItem;
            resultArray.append(gi->toJsonObject());
        }


    }
    return resultArray;
}


void HierarchyListView::isSelectedChanged(){
    if(QApplication::focusWidget()){
        if(QApplication::focusWidget()->objectName()!="hierarchyList"){
            return;
        }
    }else{
        return;
    }
    QModelIndexList selectedItems=this->selectionModel()->selectedIndexes();
    this->scene->clearColliderAnchors();
    this->scene->mainWindow->selectToolWithEditMode(SceneEditModes::GameItemsEdit);
    this->scene->clearTransformSelector(true);
    //Checking selected item count
    if(selectedItems.count()==0){
        //Setting hierarchyPanel->lockButton as false
        this->scene->hierarchyPanel->lockButton->setChecked(false);
        return;
    }
    QList<QGraphicsItem*> selectedList;
    bool lockedStatus=false;
    for(int i=0;i<selectedItems.count();i++){
        QModelIndex index=selectedItems.at(i);
        HierarchyItem *item=dynamic_cast<HierarchyItem*>(itemModel->itemFromIndex(index));
        if(item){
            GameItem *gItem=item->gameItem;
            selectedList=selectedList<<gItem;
            if(gItem->isLocked)
                lockedStatus=true;
            //gItem->setSelected(true);
        }
    }
    //Setting hierarchyPanel->lockButton as lockedStatus value
    this->scene->hierarchyPanel->lockButton->setChecked(lockedStatus);
    if(selectedList.count()>0){
        this->scene->addTransformSelector(selectedList);
    }
}




