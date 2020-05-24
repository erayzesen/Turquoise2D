#include "collectionspritesheetmodel.h"

CollectionSpriteSheetModel::CollectionSpriteSheetModel(QObject *parent) : QStandardItemModel(parent)
{

}

void CollectionSpriteSheetModel::setTarget(QString targetPath)
{
    this->clear();
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
QMimeData *CollectionSpriteSheetModel::mimeData(const QModelIndexList &indexes) const
{
    QList<QUrl> urls;

    for (int i=0; i<indexes.count(); i++){
        QString data=this->data(indexes.at(i),Qt::UserRole+1).toString();
        urls << QUrl(data);
    }
    QMimeData *data = new QMimeData();
    data->setUrls(urls);
    return data;
}
