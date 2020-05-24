#include "fileselectormodel.h"

FileSelectorModel::FileSelectorModel(QString type,QString targetFolderPath,bool noneOption,QObject *parent): QStandardItemModel(parent)
{
    //Setting needed type,folder..etc variables
    QString rootPath=targetFolderPath;
    QStringList typeList;
    if(type=="Sprite"){
        typeList<<"*.jpg"<<"*.png"<<"*.jpeg"<<"*.plist";
    }else if(type=="Class"){
        typeList<<"*.h";
    }
    //Getting files and add fileList
    QStringList fileList;
    QDir dir(rootPath);
    QDirIterator it(dir.absolutePath(), typeList, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while (it.hasNext()){
        //Filtering files
        it.next();
        if(it.fileInfo().absoluteFilePath()=="")
            continue;
        if(it.fileInfo().suffix()=="plist"){
            if(SpriteSheet::isSpritSheet(it.fileInfo().absoluteFilePath())==false){
                it.next();
                continue;
            }
        }
        fileList.append(it.fileInfo().absoluteFilePath());
    }
    qSort(fileList.begin(),fileList.end());
    if(type=="Class"){
        fileList=CheckNodeClasses(fileList);
    }
    //Adding None Option
    qDebug()<<"noneOption:"<<noneOption;
    if(noneOption==true){
        QStandardItem *dn=new QStandardItem();
        dn->setText("None");
        dn->setEditable(false);
        dn->setData(QVariant("None"));
        QFileIconProvider fIcon;
        dn->setIcon(QIcon(QPixmap(":/Icons/fileIcon_none_48x48.png")));
        this->appendRow(dn);
    }
    //Adding Files
    for(int i=0;i<fileList.count();i++){
        //nss is a QStandartItem object
        QFileInfo file(fileList.at(i));
        QStandardItem *nss=new QStandardItem();
        nss->setText(file.fileName());
        nss->setEditable(false);
        nss->setData(QVariant(file.absoluteFilePath()));
        QPixmap iconPix;
        if(file.suffix()=="png" || file.suffix()=="jpg" || file.suffix()=="jpeg"){
            iconPix=QPixmap(file.absoluteFilePath()).scaled(48,48,Qt::KeepAspectRatio);
        }else if(file.suffix()=="plist"){
            if(SpriteSheet::isSpritSheet(file.absoluteFilePath())){
                iconPix=QPixmap(":/Icons/fileIcon_spritesheet_48x48");
            }
        }else{
            QFileIconProvider fIcon;
            iconPix=fIcon.icon(QFileInfo(file.absoluteFilePath())).pixmap(QSize(48,48));
        }
        nss->setIcon(QIcon(iconPix));
        this->appendRow(nss);
    }
}

QStringList FileSelectorModel::CheckNodeClasses(QStringList classFiles)
{
    //Checking node type classes
    QStringList result;
    for(int i=0;i<classFiles.count();i++){
        QString filePath=classFiles.at(i);
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::information(0, "error", file.errorString());
        }
        QString text(file.readAll());
        text.simplified();
        if(text.indexOf(": public cocos2d::Node")>0){
            result.append(filePath);
        }
    }
    return result;
}

