#include "hierarchylist.h"
#include "ui_hierarchylist.h"

HierarchyList::HierarchyList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HierarchyList)
{
    ui->setupUi(this);
    itemModel=new QStandardItemModel(this);
    ui->list->setModel(itemModel);
    ui->list->setColumnHidden(1,true);
    ui->list->setColumnHidden(2,true);
    ui->list->setColumnHidden(3,true);
}

HierarchyList::~HierarchyList()
{
    delete ui;
}
HierarchyItem* HierarchyList::AddItem(QString name,GameItemTypes type,QString path){
    HierarchyItem *item=new HierarchyItem(name,type);
    itemModel->appendRow(item);
    return item;
}
