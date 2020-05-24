#include "collectionfilesystemmodel.h"

CollectionFileSystemModel::CollectionFileSystemModel()
{

}

QVariant CollectionFileSystemModel::data ( const QModelIndex & index, int role )const{
    if( role == Qt::DecorationRole )
    {
        QFileInfo info = CollectionFileSystemModel::fileInfo(index);

        if(info.isFile()){
            if(info.suffix() == "jpg" || info.suffix() == "png" || info.suffix() == "jpeg"){
                QPixmap pix(info.filePath());
                //qDebug()<<info.filePath();
                pix=pix.scaled(48,48,Qt::KeepAspectRatio, Qt::SmoothTransformation);
                return QPixmap(pix);//I pick the icon depending on the extension
            }else if(info.suffix() == "plist"){
                //Checking plist if it's a spritesheet.
                if(SpriteSheet::isSpritSheet(info.filePath())){
                    QPixmap pix(":/Icons/fileIcon_spritesheet_48x48");
                    return pix;
                }
            }
        }
    }
    return QFileSystemModel::data(index, role);
}
