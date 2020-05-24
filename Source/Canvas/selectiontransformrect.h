#ifndef SELECTIONTRANSFORMRECT_H
#define SELECTIONTRANSFORMRECT_H
#include "QGraphicsItem"
#include "selectiontransformitem.h"
#include "mathoperations.h"
#include "QRectF"
#include "QGraphicsDropShadowEffect"
#include "sceneeditmodes.h"

class SelectionTransformItem;
class SelectionTransformRect : public QGraphicsItem
{
public:
    SelectionTransformRect(SelectionTransformItem *_transformGroup);
    SelectionTransformItem* transformGroup;
    //Overriding paint()
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    QRectF boundingRect() const;
};

#endif // SELECTIONTRANSFORMRECT_H
