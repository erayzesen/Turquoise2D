#pragma once 

#include "QGraphicsRectItem"
#include "QGraphicsItemGroup"
#include "selectiontransformitem.h"

class selectionScaleAnchor : public QGraphicsRectItem
{
public:
    selectionScaleAnchor(int rectSize,SelectionTransformItem *transformGroup);
};
