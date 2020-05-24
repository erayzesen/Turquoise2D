#ifndef COLLECTIONBROWSER_H
#define COLLECTIONBROWSER_H

#include <QWidget>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <Collections/customfoldermodel.h>
#include <Collections/collectionfilesystemmodel.h>
#include <Collections/collectionspritesheetmodel.h>
#include "SpriteSheet/spritesheet.h"
#include "Collections/collectionfoldersystemmodel.h"

class Turquoise2DEditor;
namespace Ui {
class CollectionBrowser;
}

class CollectionBrowser : public QWidget
{
    Q_OBJECT

public:
    Turquoise2DEditor *mainWindow;
    explicit CollectionBrowser(QString _targetFolder,QStringList _nameFilter,Turquoise2DEditor *_mainWindow,QWidget *parent = 0);
    ~CollectionBrowser();
    void initBrowser();
    void enterFolder(QString _targetFolder);
    void enterSpriteSheet(QString _targetFile);
    Ui::CollectionBrowser *ui;

private slots:
    void on_browser_doubleClicked(const QModelIndex &index);

    void on_folderBrowser_clicked(const QModelIndex &index);

    void on_press_KeyDelete();

    void on_press_KeyEnter();

private:
    QString targetFolder;
    QStringList nameFilter;
    CollectionFileSystemModel *fileModel;
    CollectionFolderSystemModel *folderModel;
    customFolderModel* customModel;
    CollectionSpriteSheetModel* spriteSheetModel;
};

#endif // COLLECTIONBROWSER_H
