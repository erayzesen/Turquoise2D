#include "hierarchyitem.h"

HierarchyItem::HierarchyItem(QString _name,GameItemTypes _itemType)
{
    itemType=_itemType;
    this->setText(_name);
    this->setIcon(QPixmap(":/Icons/item_type_16x16.png"));
}

void HierarchyItem::setData(const QVariant &value, int role)
{
    if(role==Qt::EditRole){
        gameItem->name=value.toString();
    }
    QStandardItem::setData(value, role);
}
