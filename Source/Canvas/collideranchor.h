#ifndef COLLIDERANCHOR_H
#define COLLIDERANCHOR_H
#include "QGraphicsItem"
#include "Canvas/gameitem.h"
#include "mathoperations.h"
#include "Properties/collidershapeitem.h"


class ColliderShapeItem;
class GraphicsScene;
class ColliderAnchor : public QGraphicsItem
{
public:
    //frame side can be these options; l(left),r(right),t(top),b(bottom)
    ColliderAnchor(QVariantMap* _collider,QString _frameSide,GameItem* _gameItem,ColliderShapeItem* _inspectorItem);
    void setNewPosition(QPointF startPosition,QPointF lastPosition);
    //needed parameters
    QVariantMap* collider;
    QString frameSide;
    GameItem* gameItem;
    GraphicsScene *scene;
    ColliderShapeItem *inspectorItem;
    //anchor size
    float anchorSize=9;
    //temponary position
    QPointF tempPosition;
    //reimplements
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    QRectF boundingRect() const;
    bool isHover=false;
    bool updatePositions();
    void prepareAnchor();
protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // COLLIDERANCHOR_H
