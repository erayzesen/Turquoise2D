#include "collectionlistview.h"

collectionListView::collectionListView(QWidget *parent){
    setDropIndicatorShown(true);
    viewport()->setAcceptDrops(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
}


void collectionListView::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::RightButton){
        QFileSystemModel *fsm=dynamic_cast<QFileSystemModel*>(this->model());
        if(!fsm){
            return;
        }
        ContextMenu::newForCollection(this,event->pos());
        //qDebug()<<"right click";
    }else{
        QListView::mousePressEvent(event);
    }
}

void collectionListView::dragEnterEvent(QDragEnterEvent *event)
{
     event->acceptProposedAction();
}
void collectionListView::dragMoveEvent(QDragMoveEvent *event)
{
    //trying filesystemmodel type with fsm variable for understanding if current model filesystemmodel.
    QFileSystemModel *fsm=dynamic_cast<QFileSystemModel*>(this->model());
    //qDebug()<<event->mimeData()->urls();
    if(fsm){
        QModelIndex index = this->indexAt(event->pos());
        QString droppedLocation=index.data(QFileSystemModel::FilePathRole).toString();

        QList<QUrl> urls=event->mimeData()->urls();
        if(QFileInfo(urls.at(0).path()).isDir() && urls.count()==1){
            if(urls.at(0).path()==droppedLocation){
                return;
            }
        }
        if(QFileInfo(droppedLocation).isDir() || droppedLocation=="" ){
            this->setCurrentIndex(index);
        }
    }


     event->acceptProposedAction();
}
void collectionListView::dropEvent(QDropEvent *event)
{
    //trying filesystemmodel type with fsm variable for understanding if current model filesystemmodel.
    QFileSystemModel *fsm=dynamic_cast<QFileSystemModel*>(this->model());
    if(!fsm){
        return;
    }
    QModelIndex index = this->indexAt(event->pos());
    QString droppedLocation=index.data(QFileSystemModel::FilePathRole).toString();

    QList<QUrl> urls=event->mimeData()->urls();

    if(QFileInfo(urls.at(0).path()).isDir() && urls.count()==1){
        if(urls.at(0).path()==droppedLocation){
            return;
        }
    }
    if(QFileInfo(droppedLocation).isDir() && droppedLocation!=""){
        this->setCurrentIndex(index);
    }else{
        droppedLocation=this->rootIndex().data(QFileSystemModel::FilePathRole).toString();
    }

    if(event->source()==this){
        FileOperations::move(this,urls,droppedLocation,fileTypes);
    }else{
        FileOperations::copy(this,urls,droppedLocation,fileTypes);
    }
    event->acceptProposedAction();
}
void collectionListView::dragLeaveEvent(QDragLeaveEvent*event)
{
    event->accept();
}



