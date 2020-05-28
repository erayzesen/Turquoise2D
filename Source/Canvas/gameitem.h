#pragma once
#include "QGraphicsPixmapItem"
#include "QString"
#include "QDebug"
#include "QtMath"
#include "QVariantMap"
#include "SpriteSheet/spritesheet.h"
#include "Canvas/colliderrenderer.h"
#include "Canvas/objectgizmosrenderer.h"
#include "Hierarchy/hierarchyitem.h"

enum class SpriteTypes{ImageFileBased,SpriteSheetBased};
enum class GameItemTypes{Sprite,Object};
class ColliderRenderer;
class HierarchyItem;
class GraphicsScene;
class ObjectGizmosRenderer;
class GameItem : public QGraphicsPixmapItem
{
public:
    GraphicsScene *scene=NULL;
    GameItem(QString _name,HierarchyItem *_hierarchyItem,GraphicsScene *_scene,QString filePath="");
    GameItem(QJsonObject jsonObject,HierarchyItem *_hierarchyItem,GraphicsScene *_scene);
    GameItemTypes itemType;
    HierarchyItem *hierarchyItem;
    QSizeF actualSize();
    QPolygonF actualBounds(bool globalBounds=false);

    //Main properties
    QString name="";
    float rotate=0;
    QSizeF scale=QSizeF(1,1);
    QString path; //Sprite Path
    bool spriteAntialias=false;
    bool isLocked=false;
    QString tag="";

    //Physic Properties
    bool enablePyhsics=false;
    float density=0.1f;
    float friction=1.0f;
    float restitution=0.0f;
    bool dynamic=false;
    float mass=5.0f;
    QList<QVariantMap*> colliderShapes;

    //Collider Renderer
    ColliderRenderer *colliderRenderer;

    //Object Properties
    ObjectGizmosRenderer* objectGizmosRenderer;
    QString objectClassPath="None";
    QString objectType="DefaultObject";
    static QString getClassType(QString classPath);

    float tempRotate=0;
    QSizeF tempScale=QSizeF(1,1);
    QSizeF originalSize;
    void prepareTransform();
    QVariantMap SpriteInfo;
    void setSprite(QString filePath);
    void setTransparentSprite(QSizeF spriteSize=QSizeF(32,32));

    //Generating JSON Format Helpers
    QJsonObject toJsonObject();
    QJsonArray colliderShapesToJsonArray();

    //Extra Helpers
    double toDB(float num);

};
