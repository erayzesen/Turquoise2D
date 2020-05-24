#ifndef SPRITESHEET_H
#define SPRITESHEET_H
#include "QWidget"
#include "QFile"
#include "QSettings"
#include "QDebug"
#include <qtPlist/plistparser.h>
#include <qtPlist/plistserializer.h>
#include <QJsonObject>
#include <QRegExp>
#include <QFileInfo>

class SpriteSheet
{
public:
    SpriteSheet(QString plistFile);
    static bool isSpritSheet(QString plistFile);
    QString plistFilePath;
    QVariantMap parsedPlist;
    QString resourceImagePath;

    QString format;
    QString textureFileName;
    QString realTextureFileName;
    QSize size;

    QMap<QString,QRect> frame;
    QMap<QString,QPointF> offset;
    QMap<QString,bool> rotated;
    QMap<QString,QSize> sourceSize;
    QRect toRect(QString str);
    QPoint toPoint(QString str);

};

#endif // SPRITESHEET_H
