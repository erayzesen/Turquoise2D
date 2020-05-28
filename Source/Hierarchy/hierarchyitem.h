#pragma once
#include <QWidget>
#include <QObject>
#include <QStandardItem>
#include <QPoint>
#include "Canvas/gameitem.h"

class GameItem;
enum class GameItemTypes;
class HierarchyItem : public QStandardItem
{
public:
    GameItem *gameItem;
    HierarchyItem(QString _name,GameItemTypes _itemType);
    GameItemTypes itemType;
    virtual void setData(const QVariant & value, int role = Qt::UserRole + 1);
};
