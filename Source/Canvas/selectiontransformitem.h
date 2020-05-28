#pragma once
#include "QGraphicsItemGroup"
#include "QGraphicsScene"
#include "Canvas/gameitem.h"
#include "Canvas/transformanchor.h"
#include "Canvas/transformtypes.h"
#include "Canvas/selectiontransformrect.h"
#include "QPainter"
#include "QPoint"
#include "mathoperations.h"
#include "Properties/propertiesitem.h"
#include "Canvas/graphicsscene.h"

class TransformAnchor;
class SelectionTransformRect;
class PropertiesItem;
class SelectionTransformItem : public QGraphicsItemGroup
{
public:
    SelectionTransformItem(QList<QGraphicsItem*> _items,GraphicsScene * _scene,PropertiesItem *_propItem=NULL);
    ~SelectionTransformItem();
    QSizeF *selectorSize;
    QRectF *transformRect;
    QRectF *currentRect;
    GraphicsScene * scene;
    QGraphicsItemGroup *itemsContainer;
    void UpdateContainer();
    void UpdateContainerZOrder();
    PropertiesItem* propItem;
    QPointF *originPoint=new QPointF();
    void updateOrigin();
    SelectionTransformRect* setRectItem();
    void addItems();
    void  dropItems();
    void addTransformTools();
    SelectionTransformRect *rectItem;
    QList<QGraphicsItem*> items;
    TransformAnchor* rightBottomAnchor;
    TransformAnchor* leftBottomAnchor;
    TransformAnchor* rightTopAnchor;
    TransformAnchor* leftTopAnchor;
    bool checkScaleCursor(QGraphicsSceneMouseEvent *mouseEvent);
    bool checkRotateCursor(QGraphicsSceneMouseEvent *mouseEvent);
    float lastRotation=0;
    float deltaRotation=0;
    QSizeF lastScale=QSizeF(1,1);
    QSizeF deltaScale=QSizeF(1,1);
    void updateLastRotation();
    bool checkTransformCursor(QGraphicsSceneMouseEvent *mouseEvent);
    void rotateTransform(QPointF p1,QPointF p2);
    void rotateTransform(float angle);
    void UpdateBounds();
    void applyRotations();
    void applyScales();
    void rePrepareItems();
    void updateAnchorTransforms(QPolygonF points);
    void setNewScale(QPointF newScale,bool digitFilter=true);

};
