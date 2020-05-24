#include "spritesheetselectormodel.h"


SpriteSheetSelectorModel::SpriteSheetSelectorModel(QObject *parent) : QStandardItemModel(parent)
{

}

void SpriteSheetSelectorModel::setTarget(QString targetPath)
{
    this->clear();

    QStandardItem *backItem=new QStandardItem();
    backItem->setText("..");
    backItem->setData("<-back");
    backItem->setIcon(QIcon(QPixmap(":/Icons/fileIcon_back_48x48")));
    this->appendRow(backItem);

    SpriteSheet *spriteSheet=new SpriteSheet(targetPath);
    QPixmap resourceImage(spriteSheet->resourceImagePath);
    for(int i=0;i<spriteSheet->frame.keys().count();i++){
        //nss is a QStandartItem object
        QStandardItem *nss=new QStandardItem();
        nss->setText(spriteSheet->frame.keys().at(i));
        QPixmap iconImage=resourceImage.copy(spriteSheet->frame[spriteSheet->frame.keys().at(i)]);
        iconImage=iconImage.scaled(48,48,Qt::KeepAspectRatio);
        QIcon itemIcon(iconImage);
        nss->setIcon(itemIcon);
        nss->setDragEnabled(true);
        nss->setEditable(false);
        nss->setData(targetPath+"?frameName="+spriteSheet->frame.keys().at(i));
        this->appendRow(nss);
    }

}

