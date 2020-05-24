#ifndef SELECTIONSCALEANCHOR_H
#define SELECTIONSCALEANCHOR_H
#include "QGraphicsRectItem"
#include "QGraphicsItemGroup"
#include "selectiontransformitem.h"

class selectionScaleAnchor : public QGraphicsRectItem
{
public:
    selectionScaleAnchor(int rectSize,SelectionTransformItem *transformGroup);
};

#endif // SELECTIONSCALEANCHOR_H
