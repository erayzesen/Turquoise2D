#ifndef HIERARCHYLIST_H
#define HIERARCHYLIST_H

#include <QWidget>
#include "hierarchyitem.h"

namespace Ui {
class HierarchyList;
}

class HierarchyList : public QWidget
{
    Q_OBJECT

public:
    explicit HierarchyList(QWidget *parent = 0);
    HierarchyItem* AddItem(QString name,GameItemTypes type,QString path);
    QStandardItemModel *itemModel;
    ~HierarchyList();

private:
    Ui::HierarchyList *ui;
};

#endif // HIERARCHYLIST_H
