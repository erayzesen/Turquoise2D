#pragma once
#include "QStandardItemModel"
#include "QStandardItem"
#include "SpriteSheet/spritesheet.h"

class SpriteSheetSelectorModel: public QStandardItemModel
{
public:
    explicit SpriteSheetSelectorModel(QObject *parent=0);
    void setTarget(QString targetPath);
};

