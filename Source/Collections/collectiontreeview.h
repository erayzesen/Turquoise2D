#ifndef COLLECTIONTREEVIEW_H
#define COLLECTIONTREEVIEW_H
#include <QTreeView>
#include <QDropEvent>
#include <QDebug>
#include <QMimeData>
#include <QFileSystemModel>
#include <QMessageBox>
#include <fileoperations.h>
#include <QShortcut>
#include <QMenu>
#include <contextmenu.h>
#include <Collections/customfoldermodel.h>
#include <Collections/collectionbrowser.h>
#include <Collections/collectionlistview.h>

class collectionListView;
class collectionTreeView : public QTreeView
{
public:
    collectionTreeView(QWidget *parent = 0);
    QStringList *fileTypes;
    collectionListView *joinedBrowser;
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
     void dragLeaveEvent(QDragLeaveEvent*event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // COLLECTIONTREEVIEW_H
