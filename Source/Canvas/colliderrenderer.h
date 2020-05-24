#ifndef COLLIDERRENDERER_H
#define COLLIDERRENDERER_H
#include "QGraphicsItem"
#include "QPainter"
#include "mathoperations.h"
#include "Canvas/gameitem.h"

class GameItem;
class GraphicsScene;
class ColliderRenderer : public QGraphicsItem
{
public:
    GameItem *gameItem;
    GraphicsScene *scene=NULL;
    ColliderRenderer(GameItem* _gameItem);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    QRectF boundingRect() const;
    QPolygonF setSceneZoomFactor(QPolygonF points);
};

#endif // COLLIDERRENDERER_H
