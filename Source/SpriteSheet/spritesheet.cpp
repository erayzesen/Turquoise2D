#include "spritesheet.h"

SpriteSheet::SpriteSheet(QString plistFile)
{
    plistFilePath=plistFile;
    QFile file(plistFilePath);
    parsedPlist=PListParser::parsePList(&file).toMap();

    //Getting texture infos from file
    QMap<QString,QVariant> parsedMetadata=parsedPlist["metadata"].toMap();
    if(parsedMetadata.keys().count()>0){
        format=parsedMetadata["format"].toString();
        textureFileName=parsedMetadata["textureFileName"].toString();
        realTextureFileName=parsedMetadata["realTextureFileName"].toString();
        QPoint pSize=toPoint(parsedMetadata["size"].toString());
        size=QSize(pSize.x(),pSize.y());
    }
    QFileInfo info(file);
    resourceImagePath=info.absolutePath()+"/"+realTextureFileName;

    //Getting all spritesheet frames from file
    QMap<QString,QVariant> parsedFrames=parsedPlist["frames"].toMap();
    for(int i=0;i<parsedFrames.keys().count();i++){
        QString frameName=parsedFrames.keys().at(i);
        QMap<QString,QVariant> frameProps=parsedFrames[frameName].toMap();

        QRect frameRect=toRect(frameProps["frame"].toString());
        frame.insert(frameName,frameRect);

        QString frameRotated=frameProps["rotated"].toString();
        rotated.insert(frameName,(frameRotated=="true" ? true:false));

        QPoint frameOffset=toPoint(frameProps["offset"].toString());
        offset.insert(frameName,frameOffset);

        QPoint frameSize=toPoint(frameProps["sourceSize"].toString());
        sourceSize.insert(frameName,QSize(frameSize.x(),frameSize.y()));
    }
}

bool SpriteSheet::isSpritSheet(QString plistFile)
{
    QFile file(plistFile);
    QFileInfo fileInfo(file);
    if(fileInfo.suffix()!="plist"){
        return false;
    }
    //qDebug()<<fileInfo.suffix();
    QVariantMap mainFile=PListParser::parsePList(&file).toMap();
    bool condition1=mainFile.contains("frames");
    bool condition2=mainFile.contains("metadata");
    if(condition1==false || condition2==false){
        return false;
    }
    //qDebug()<<"hasFrames:"<<condition1<<" hasMetadata:"<<condition2;
    QMap<QString,QVariant> fileMetadata=mainFile["metadata"].toMap();
    //qDebug()<<fileMetadata["format"].toInt();
    if(fileMetadata["format"].toInt()!=2){
        return false;
    }
    //qDebug()<<"it's a spritesheet";
    return true;

}

QRect SpriteSheet::toRect(QString str)
{
    QRegExp rx("(\\d+[,]\\d+)");
    int pos=0;
    QList<int> intList;
    while((pos=rx.indexIn(str,pos))!=-1){
        QStringList points=rx.cap(1).split(",");
        int p1=points.at(0).toInt();
        int p2=points.at(1).toInt();
        intList<<p1;
        intList<<p2;
        pos+=rx.matchedLength();
    }
    return QRect(intList.at(0),intList.at(1),intList.at(2),intList.at(3));
}

QPoint SpriteSheet::toPoint(QString str)
{
    QRegExp rx("(\\d+[,]\\d+)");
    int pos=0;
    QPoint resultPoint(0,0);
    QList<int> intList;
    if(rx.indexIn(str,pos)!=-1){
        QStringList points=rx.cap(1).split(",");
        int p1=points.at(0).toInt();
        int p2=points.at(1).toInt();
        resultPoint=QPoint(p1,p2);
    }else{
        qDebug()<<"PLIST FILE ERROR! | "<< plistFilePath <<" named file is damaged!";
    }
    return resultPoint;

}

