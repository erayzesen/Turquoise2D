#include "gameitem.h"
#include "Canvas/graphicsscene.h"

GameItem::GameItem(QString _name,HierarchyItem *_hierarchyItem,GraphicsScene *_scene,QString filePath)
{
    this->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    name=_name;
    path=filePath;

    hierarchyItem=_hierarchyItem;
    itemType=hierarchyItem->itemType;
    rotate=0;
    scene=_scene;
    colliderRenderer=new ColliderRenderer(this);
    if(itemType==GameItemTypes::Object){
        setTransparentSprite(QSize(32,32));
        objectGizmosRenderer=new ObjectGizmosRenderer(this);
    }
    if(path!=""){
        setSprite(path);
    }
}

GameItem::GameItem(QJsonObject jsonObject, HierarchyItem *_hierarchyItem, GraphicsScene *_scene)
{
    this->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    scene=_scene;
    hierarchyItem=_hierarchyItem;
    //Type
    itemType=hierarchyItem->itemType;
    tag=jsonObject.value("tag").toString();
    name=jsonObject.value("name").toString();
    QJsonArray posArray=jsonObject.value("position").toArray();
    //Position
    //Reversing y position because of qt coordinate system
    this->setPos(posArray.at(0).toDouble(),-posArray.at(1).toDouble());
    //Scale
    QJsonArray scaleArray=jsonObject.value("scale").toArray();
    this->scale.setWidth(scaleArray.at(0).toDouble());
    this->scale.setHeight(scaleArray.at(1).toDouble());
    //Rotate
    rotate=jsonObject.value("rotation").toDouble();
    this->setRotation(rotate);

    //Locked
    this->isLocked=jsonObject.value("isLocked").toBool();
    QIcon hIcon=isLocked ? QIcon(":/Icons/item_lock_type_16x16.png"):QIcon(":/Icons/item_type_16x16.png");
    this->hierarchyItem->setIcon(hIcon);

    //Paths
    objectClassPath=jsonObject.value("objectClassPath").toString("None");
    objectType=jsonObject.value("objectType").toString("DefaultObject");

    QString spriteType=jsonObject.value("spriteType").toString();
    QString spritePath=jsonObject.value("spritePath").toString();
    QString spriteFrameName=jsonObject.value("spriteFrameName").toString();
    if(spriteType=="SpriteSheetBased"){
        this->path=spritePath+"?frameName="+spriteFrameName;
    }else{
        this->path=spritePath;
    }

    if(path!=""){
        setSprite(path);
    }else{
        setTransparentSprite(QSize(32,32));
    }
    spriteAntialias=jsonObject.value("spriteAntialias").toBool();

    //Physics and Shapes
    QJsonObject physicsJsonObj=jsonObject.value("physics").toObject();
    this->enablePyhsics=physicsJsonObj.value("enable").toBool();
    this->dynamic=physicsJsonObj.value("dynamic").toBool();
    this->mass=physicsJsonObj.value("mass").toDouble();
    this->density=physicsJsonObj.value("density").toDouble();
    this->friction=physicsJsonObj.value("friction").toDouble();
    this->density=physicsJsonObj.value("density").toDouble();
    QJsonArray shapesArray=physicsJsonObj.value("colliderShapes").toArray();
    for(int i=0;i<shapesArray.count();i++){
        QJsonObject shape=shapesArray.at(i).toObject();
        QString shapeType=shape.value("type").toString();
        QJsonArray rectArr=shape.value("rect").toArray();
        QRectF shapeRect(rectArr.at(0).toDouble(),rectArr.at(1).toDouble(),rectArr.at(2).toDouble(),rectArr.at(3).toDouble());
        QVariantMap *shapeMap=new QVariantMap;
        shapeMap->insert("type",shapeType);
        shapeMap->insert("rect",shapeRect);

        colliderShapes.append(shapeMap);
    }
    colliderRenderer=new ColliderRenderer(this);
    //Object Gizmos Renderer (If item type object)
    if(itemType==GameItemTypes::Object){
        objectGizmosRenderer=new ObjectGizmosRenderer(this);
    }
    this->setZValue(jsonObject.value("zOrder").toInt());
    qDebug()<<"obje eklendi: "<< name;

}

QSizeF GameItem::actualSize()
{
    // (i) Actual size is the GameItem's scale*original size value. It gives us actual size everytime.
    QPointF gsize(this->originalSize.width(),this->originalSize.height()); //Getting item's original size
    QPointF gscale(this->scale.width(),this->scale.height()); // Getting scale value from scale size variable
    QSizeF cgsize=QSizeF(gsize.x()*gscale.x(),gsize.y()*gscale.y()); // Crossing original size and scale
    return cgsize; //returning new calculated size
}

QPolygonF GameItem::actualBounds(bool globalBounds)
{
    // (i) Actual Bounds gives us bound points from item everytime. If you scaled or rotated object, item's bound points changes.

    QSizeF asize=actualSize(); //Gettin actual size
    float asw=asize.width()/2; // actualsize's width/2.
    float ash=asize.height()/2; // actualsize's height/2.
    float x=this->pos().x(); // x,y are the position variables
    float y=this->pos().y();
    qreal rot=qDegreesToRadians(this->rotate); // Getting item's rotate variable and converting to radian.
    QPolygonF npoly; //npoly has 4 point for borders.
    npoly<<QPointF(-asw,-ash)<<QPointF(asw,-ash)<<QPointF(asw,ash)<<QPointF(-asw,ash); // Defining 4 point without rotation factor.
    if(rot==0){ // if object not rotated
        //(i) if we need global bounds, we must add scene position to points.
        if(globalBounds==true){
            npoly[0]=npoly.at(0)+this->scenePos();
            npoly[1]=npoly.at(1)+this->scenePos();
            npoly[2]=npoly.at(2)+this->scenePos();
            npoly[3]=npoly.at(3)+this->scenePos();
        }
        return npoly; // Returning unrotated points
    }
    QPolygonF rotatedPoly; // Defining rotatepoly named new QPolygonF variable. It will be rotated point container after calculates.
    /*(i) Firstly, we are calculating the angle between the origin point and the border point and adding this angle to item's rotation angle.
    After we're crossing Cos(angle) Sin(angle) value with origin-border point distance.
    */
    for(int i=0;i<npoly.count();i++){ // For all unrotated points
        QPointF deltaPoint=npoly.at(i);
        qreal dist=qSqrt(deltaPoint.x()*deltaPoint.x()+deltaPoint.y()*deltaPoint.y()); // Getting distance between origin point and border point with pythagorean teorem.
        qreal defaultRot=qAtan2(npoly.at(i).y(),npoly.at(i).x()); // Getting angle value between origin point and border point with atan2(y,x) method.
        qreal totalRot=defaultRot+rot; // Getting total angle with add item's angle to point-origin angle.
        QPointF nPoint(qCos(totalRot)*dist,qSin(totalRot)*dist); // Cos(totalRot) for new point's x value, Sin(totalRot) for new point's y value
        nPoint=globalBounds==true ? nPoint+this->scenePos():nPoint;
        rotatedPoly<<nPoint; // Adding point to rotatedPoly
    }
    return rotatedPoly;

}

QString GameItem::getClassType(QString classPath)
{
    //Checking node type classes
    QString result;
    QFile file(classPath);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0, "error", file.errorString());
    }
    QString text(file.readAll());
    QRegularExpression re("class\\s+([\\w]+)\\s?\\:\\s?public");
    QRegularExpressionMatch match=re.match(text);
    if(match.hasMatch()){
        result=match.captured(1);
    }
    return result;
}



void GameItem::prepareTransform()
{
    // Calls the function before when item selected. The function prepares item for transforms and creates new temp scale and temp rotate variables.
    this->tempScale=this->scale;
    this->tempRotate=this->rotate;
}

void GameItem::setSprite(QString filePath)
{
    //Checking Sprite Info Keys
    if(SpriteInfo.keys().contains("type")==false)
        SpriteInfo.insert("type",QVariant());
    if(SpriteInfo.keys().contains("filePath")==false)
        SpriteInfo.insert("filePath",QVariant());
    if(SpriteInfo.keys().contains("frameName")==false)
        SpriteInfo.insert("frameName",QVariant());


    //Checking sprite file type (from Spritesheet or from direct image file)
    QString resourcesPath=scene->mainWindow->projectFolderPath+scene->mainWindow->resourcesPath+"/";
    if(filePath.indexOf("?frameName=")>0){
        path=filePath;
        SpriteInfo["type"]=QVariant("SpriteSheetBased");
        QStringList urls=path.split("?frameName=");
        SpriteInfo["filePath"]=QVariant(resourcesPath+urls.at(0));
        SpriteInfo["frameName"]=QVariant(urls.at(1));
        SpriteSheet ss(resourcesPath+urls.at(0));
        QPixmap resourceImage(ss.resourceImagePath);
        QPixmap cropped=resourceImage.copy(ss.frame[urls.at(1)]);
        this->setPixmap(cropped);

    }else{
        path=filePath;
        SpriteInfo["type"]=QVariant("ImageFileBased");
        SpriteInfo["filePath"]=QVariant(path);
        //qDebug()<<path;
        QPixmap resourceImage(resourcesPath+path);
        this->setPixmap(resourceImage);
    }
    originalSize=this->pixmap().size(); // Defined size as pixmap size.
    this->setOffset(-originalSize.width()/2,-originalSize.height()/2);
}

void GameItem::setTransparentSprite(QSizeF spriteSize)
{
    QPixmap pix(spriteSize.width(),spriteSize.height());
    pix.fill(Qt::transparent);
    this->setPixmap(pix);
    originalSize=pix.size();
    this->setOffset(-originalSize.width()/2,-originalSize.height()/2);
    this->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    qDebug()<<this->boundingRect();
}

QJsonObject GameItem::toJsonObject()
{
    QJsonObject resultObject;
    QString itemTypeStr;
    if(this->itemType==GameItemTypes::Object){
        itemTypeStr="object";
    }else if(this->itemType==GameItemTypes::Sprite){
        itemTypeStr="sprite";
    }
    //Adding main game item properties
    resultObject.insert("type",itemTypeStr);
    resultObject.insert("tag",this->tag);
    resultObject.insert("name",this->name);
    resultObject.insert("position",QJsonArray {this->scenePos().x(),-this->scenePos().y()});
    resultObject.insert("rotation",toDB(this->rotate));
    resultObject.insert("scale",QJsonArray {this->scale.width(),this->scale.height()});
    resultObject.insert("zOrder",this->zValue());
    resultObject.insert("isLocked",this->isLocked);

    QJsonObject physicsObject;
    QJsonArray physicsShapes;
    //Setting physics properties
    physicsObject.insert("enable",this->enablePyhsics);
    physicsObject.insert("dynamic",this->dynamic);
    physicsObject.insert("mass",this->mass);
    physicsObject.insert("density",this->density);
    physicsObject.insert("friction",this->friction);
    physicsObject.insert("restitution",this->restitution);
    physicsObject.insert("colliderShapes",this->colliderShapesToJsonArray());
    resultObject.insert("physics",physicsObject);

    resultObject.insert("objectClassPath",this->objectClassPath);
    resultObject.insert("objectType",this->objectType);

    //Sprite properties
    QString spriteType=path.indexOf("?frameName=")>0 ? "SpriteSheetBased":"ImageFileBased";
    QString spritePath,spriteFrame;
    if(spriteType=="SpriteSheetBased"){
        QStringList urls=path.split("?frameName=");
        spritePath=urls.at(0);
        spriteFrame=urls.at(1);
    }else{
        spritePath=path;
        spriteFrame="";
    }

    resultObject.insert("spriteType",spriteType);
    resultObject.insert("spritePath",spritePath);
    resultObject.insert("spriteFrameName",spriteFrame);
    resultObject.insert("spriteAntialias",this->spriteAntialias);
    qDebug()<<resultObject;
    return resultObject;
}

QJsonArray GameItem::colliderShapesToJsonArray()
{
    QJsonArray resultArray;
    for(int i=0;i<colliderShapes.count();i++){
        //Getting collider shape properties
        QVariantMap *shape=colliderShapes.at(i);
        QString shapeType=shape->value("type").toString();
        QRectF shapeRect=shape->value("rect").toRectF();
        //Adding to the array as QJsonObject
        QJsonObject shapeJsonObject;
        shapeJsonObject.insert("type",shapeType);
        shapeJsonObject.insert("rect",QJsonArray{shapeRect.x(),shapeRect.y(),shapeRect.width(),shapeRect.height()});
        resultArray.append(shapeJsonObject);
    }
    return resultArray;
}

//It return 2 digit decimal double
double GameItem::toDB(float num)
{
    return QString::number(num,'d',2).toDouble();
}



