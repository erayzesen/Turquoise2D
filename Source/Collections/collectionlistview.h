#pragma once

#include <QListView>
#include <QDropEvent>
#include <QDebug>
#include <QMimeData>
#include <QFileSystemModel>
#include <QMessageBox>
#include <fileoperations.h>
#include <QShortcut>
#include <QMenu>
#include <contextmenu.h>
#include <Collections/collectionbrowser.h>
#include <Collections/collectiontreeview.h>

class collectionTreeView;
class collectionListView : public QListView
{
public:
    collectionListView(QWidget *parent = 0);
    void copyFiles(QList<QUrl> fileList,QString targetDirectory);
    void copyFiles(QString filePath,QString targetDirectory);
    QStringList *fileTypes;
    collectionTreeView *joinedBrowser;
public slots:

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent*event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
};
