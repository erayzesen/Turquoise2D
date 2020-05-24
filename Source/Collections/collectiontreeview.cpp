#include "collectiontreeview.h"

collectionTreeView::collectionTreeView(QWidget *parent)
{
     //setDropIndicatorShown(true);
     //viewport()->setAcceptDrops(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
}

void collectionTreeView::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::RightButton){
        ContextMenu::newForCollectionFolderBrowser(this,event->pos());
        //qDebug()<<"right click";
    }else{
        QTreeView::mousePressEvent(event);
    }
}

void collectionTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    QModelIndex index = this->indexAt(event->pos());
    QSortFilterProxyModel *spm=qobject_cast<QSortFilterProxyModel*>(this->model());
    QFileSystemModel *fsm=qobject_cast<QFileSystemModel*>(spm->sourceModel());
    QString rootPath=fsm->rootPath();
    if(index.data(QFileSystemModel::FilePathRole).toString()==rootPath){
        return;
    }else{
        event->acceptProposedAction();
    }
}
void collectionTreeView::dragMoveEvent(QDragMoveEvent *event)
{
     QModelIndex index = this->indexAt(event->pos());

     QSortFilterProxyModel *spm=qobject_cast<QSortFilterProxyModel*>(this->model());
     QFileSystemModel *fsm=qobject_cast<QFileSystemModel*>(spm->sourceModel());
     QString rootPath=fsm->rootPath();

     QString droppedLocation=index.data(QFileSystemModel::FilePathRole).toString();
     QList<QUrl> urls=event->mimeData()->urls();
     if(QFileInfo(urls.at(0).path()).isDir() && urls.count()==1){
         if(urls.at(0).path()==droppedLocation){
             return;
         }
     }
     if(urls.at(0).path()==rootPath){
         return;
     }
     if(QFileInfo(droppedLocation).isDir() || droppedLocation=="" ){
         this->setCurrentIndex(index);
     }
     event->acceptProposedAction();
}
void collectionTreeView::dropEvent(QDropEvent *event)
{
    QModelIndex index = this->indexAt(event->pos());
    QString droppedLocation=index.data(QFileSystemModel::FilePathRole).toString();
    if(droppedLocation==""){
        return;
    }
    QSortFilterProxyModel *spm=qobject_cast<QSortFilterProxyModel*>(this->model());
    QFileSystemModel *fsm=qobject_cast<QFileSystemModel*>(spm->sourceModel());
    QString rootPath=fsm->rootPath();
    QList<QUrl> urls=event->mimeData()->urls();
    qDebug()<<"dropped location" <<droppedLocation;
    if(QFileInfo(urls.at(0).path()).isDir() && urls.count()==1){
        if(urls.at(0).path()==droppedLocation){
            return;
        }
    }
    if(urls.at(0).path()==rootPath){
        event->ignore();
        return;
    }

    if(QFileInfo(droppedLocation).isDir() && droppedLocation!=""){
        this->setCurrentIndex(index);
    }else{
        droppedLocation=rootPath;
    }
    if(event->source()==this || event->source()==this->joinedBrowser){
        FileOperations::move(this,urls,droppedLocation,fileTypes);
    }else{
        FileOperations::copy(this,urls,droppedLocation,fileTypes);
    }
    event->acceptProposedAction();
}
void collectionTreeView::dragLeaveEvent(QDragLeaveEvent*event)
{
    event->accept();
}

