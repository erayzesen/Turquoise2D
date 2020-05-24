#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H
#include <Qt>
#include <QApplication>
#include <QClipboard>
#include <QWidget>
#include <QMenu>
#include <QListView>
#include <Collections/collectionlistview.h>
#include <Collections/collectiontreeview.h>
#include <QDesktopServices>


class collectionListView;
class collectionTreeView;
class ContextMenu
{
public:
    ContextMenu();
    static void newForCollection(collectionListView *parent,QPoint point);
    static void newForCollectionFolderBrowser(collectionTreeView *parent,QPoint point);
    static QString getCollectionType(QStringList *typeList);
};

#endif // CONTEXTMENU_H
