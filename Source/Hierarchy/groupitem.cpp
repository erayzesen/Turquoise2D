#include "groupitem.h"

GroupItem::GroupItem(QString groupTitle,CategoryTypes _categoryType)
{
    categoryType=_categoryType;
    this->setText(groupTitle);
    if(categoryType==CategoryTypes::Objects){
        this->setIcon(QIcon(":/Icons/object_type_16x16.png"));
    }else if(categoryType==CategoryTypes::Sprites){
        this->setIcon(QIcon(":/Icons/sprite_type_16x16.png"));
    }
}

