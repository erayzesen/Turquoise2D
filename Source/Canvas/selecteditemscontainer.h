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


class SelectedItemsContainer: public QGraphicsItemGroup
{
public:
    SelectedItemsContainer(QList<QGraphicsItem*> _items,GraphicsScene * _scene,PropertiesItem *_propItem=NULL);
    ~SelectedItemsContainer();
    void addItems();
    void  dropItems();
};

