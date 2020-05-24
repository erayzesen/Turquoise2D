#include "collectionfoldersystemmodel.h"

CollectionFolderSystemModel::CollectionFolderSystemModel()
{

}

QVariant CollectionFolderSystemModel::data ( const QModelIndex & index, int role )const{
    if( role == Qt::DecorationRole )
    {
        QFileInfo info = CollectionFolderSystemModel::fileInfo(index);

        if(info.isFile()){
            if(info.suffix() == "plist"){
                if(SpriteSheet::isSpritSheet(info.filePath())){
                    QPixmap pix(":/Icons/fileIcon_spritesheet_16x16");
                    return pix;
                }
            }
        }
    }
    return QFileSystemModel::data(index, role);
}
