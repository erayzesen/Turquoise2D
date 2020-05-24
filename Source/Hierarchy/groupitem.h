#ifndef GROUPITEM_H
#define GROUPITEM_H
#include <QStandardItem>

enum class CategoryTypes{Sprites,Objects};
class GroupItem: public QStandardItem
{
public:
    CategoryTypes categoryType;
    GroupItem(QString groupTitle,CategoryTypes _categoryType);
};

#endif // GROUPITEM_H
