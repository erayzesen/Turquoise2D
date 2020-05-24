#include "contextmenu.h"

ContextMenu::ContextMenu()
{

}
void ContextMenu::newForCollection(collectionListView *parent,QPoint point){
    QModelIndex targetIndex=parent->indexAt(point);
    QModelIndex directoryIndex=parent->rootIndex();
    //File locations
    QString targetLoc=targetIndex.data(QFileSystemModel::FilePathRole).toString();
    QString directoryLoc=directoryIndex.data(QFileSystemModel::FilePathRole).toString();
    //
    //Creating new context menu object
    QMenu contextMenu("Context Menu",parent);
    //
    //Get collection type
    QString collectionType=getCollectionType(parent->fileTypes);
    //

    //ALL ACTIONS
    QAction openObjAct("Open with Editor",parent);
    QAction editCppAct("Edit Cpp File",parent);
    QAction editHeaderAct("Edit Header File",parent);
    QAction copyAct("Copy",parent);
    QAction pasteAct("Paste",parent);
    QAction moveAct("Move Files Here",parent);
    QAction renameAct("Rename",parent);
    QAction removeAct("Remove",parent);
    QAction newObjectAct("New Object",parent);
    QAction newSceneAct("New Scene",parent);
    QAction importSpritesAct("Import Sprites...",parent);
    QAction importSoundsAct("Import Sounds...",parent);
    QAction newFolderAct("New Folder",parent);
    QAction exploreFolderAct("Explore Folder",parent);
    //

    // Collection File Featured Options
    if(targetLoc!=""){
        //Collection Type Featured Options
        if(!QFileInfo(targetLoc).isDir()){
            if(collectionType=="Objects"){
                contextMenu.addAction(&openObjAct);
                contextMenu.addAction(&editCppAct);
                contextMenu.addAction(&editHeaderAct);
                contextMenu.addSeparator();
            }else if(collectionType=="Scenes"){
                contextMenu.addAction(&openObjAct);
                contextMenu.addAction(&editCppAct);
                contextMenu.addAction(&editHeaderAct);
                contextMenu.addSeparator();
            }
        }

        //
        //Default File Options
        contextMenu.addAction(&copyAct); // Copy
        if(QApplication::clipboard()->mimeData()->urls().count()>0){
            contextMenu.addAction(&pasteAct); //Paste
            contextMenu.addAction(&moveAct); //Move
        }
        if(parent->selectionModel()->selectedIndexes().count()<=1)
        contextMenu.addAction(&renameAct); // Rename
        contextMenu.addAction(&removeAct); //Remove
        contextMenu.addSeparator();
        //

        //selecting index
        if(parent->selectionModel()->selectedIndexes().count()<=1)
        parent->setCurrentIndex(targetIndex);

        //parent->connect(&renameAct,SIGNAL(triggered()),parent,SLOT(Rename()));
    }else{
        if(QApplication::clipboard()->mimeData()->urls().count()>0){
            contextMenu.addAction(&pasteAct); //Paste
            contextMenu.addAction(&moveAct); // Move
        }
    }
    //

    //General Menu Options

    if(collectionType=="Objects"){
        contextMenu.addAction(&newObjectAct);
    }else if(collectionType=="Scenes"){
        contextMenu.addAction(&newSceneAct);
    }else if(collectionType=="Sprites"){
        contextMenu.addAction(&importSpritesAct);
    }else if(collectionType=="Sounds"){
        contextMenu.addAction(&importSoundsAct);
    }
    //
    //Folder Options
    contextMenu.addAction(&newFolderAct);
    contextMenu.addSeparator();
    contextMenu.addAction(&exploreFolderAct);
    //
    //Execute Menu
    QAction *selected=contextMenu.exec(parent->mapToGlobal(point));
    //
    //Check Selected
    if(selected){
        //Check Which Selected Action
        QString targetDir=QFileInfo(targetLoc).isDir() && targetLoc!="" ? targetLoc:directoryLoc;
        if(selected==&openObjAct){

        }else if(selected==&editCppAct){

        }else if(selected==&editHeaderAct){

        }else if(selected==&copyAct){
            //Copy Files
            QMimeData *filesData=new QMimeData();
            QStringList selectedFiles=FileOperations::toStringList(parent->selectionModel()->selectedIndexes());
            filesData->setUrls(FileOperations::toUrlList(selectedFiles));
            QApplication::clipboard()->setMimeData(filesData);

        }else if(selected==&pasteAct){
            //Paste Files

            QList<QUrl> filesData=QApplication::clipboard()->mimeData()->urls();
            FileOperations::copy(parent,filesData,targetDir,parent->fileTypes);


        }else if(selected==&moveAct){
            //Paste Files

            QList<QUrl> filesData=QApplication::clipboard()->mimeData()->urls();
            FileOperations::move(parent,filesData,targetDir,parent->fileTypes);


        }else if(selected==&renameAct){
            //Rename File
            parent->edit(targetIndex);

        }else if(selected==&removeAct){
            //Remove Files
            FileOperations::remove(parent,FileOperations::toStringList(parent->selectionModel()->selectedIndexes()),true);

        }else if(selected==&newObjectAct){

        }else if(selected==&newSceneAct){

        }else if(selected==&importSpritesAct){

        }else if(selected==&importSoundsAct){

        }else if(selected==&newFolderAct){
            //Create New Folder
            QString folderName=FileOperations::getValidName(directoryLoc,"New Folder");
            QDir(directoryLoc).mkdir(folderName);
            //Rename Folder
            QFileSystemModel *fModel=static_cast<QFileSystemModel*>(parent->model());
            parent->edit(fModel->index(directoryLoc+"/"+folderName));

        }else if(selected==&exploreFolderAct){
            //Explore Directory
            QDesktopServices::openUrl(QUrl("file:///"+targetDir));
        }

        //
    }
    //
}
void ContextMenu::newForCollectionFolderBrowser(collectionTreeView *parent,QPoint point){
    QModelIndex targetIndex=parent->indexAt(point);
    QModelIndex directoryIndex=parent->rootIndex();
    //File locations
    QString targetLoc=targetIndex.data(QFileSystemModel::FilePathRole).toString();
    QString directoryLoc=directoryIndex.data(QFileSystemModel::FilePathRole).toString();
    //
    //Folder Browser File System Models and Root Path
    QSortFilterProxyModel *spm=qobject_cast<QSortFilterProxyModel*>(parent->model());
    QFileSystemModel *fsm=qobject_cast<QFileSystemModel*>(spm->sourceModel());
    QString rootPath=fsm->rootPath();
    //
    //File Browser's File System Model
    QFileSystemModel *fileBrowserModel=static_cast<QFileSystemModel*>(parent->joinedBrowser->model());
    //
    //If user right click empty space exit function
    if(targetLoc==""){
        return;
    }else{
        parent->setCurrentIndex(targetIndex);
        parent->joinedBrowser->setRootIndex(fileBrowserModel->index(targetLoc));
    }
    //
    //Creating new context menu object
    QMenu contextMenu("Context Menu",parent);
    //
    //Get collection type
    QString collectionType=getCollectionType(parent->fileTypes);
    //
    //ALL ACTIONS
    QAction copyAct("Copy",parent);
    QAction pasteAct("Paste",parent);
    QAction moveAct("Move Files Here",parent);
    QAction renameAct("Rename",parent);
    QAction removeAct("Remove",parent);
    QAction newFolderAct("New Folder",parent);
    QAction exploreFolderAct("Explore Folder",parent);
    //

    // New Folder Option
    contextMenu.addAction(&newFolderAct);
    contextMenu.addSeparator();
    //
    //Move Paste Oparation Options
    if(QApplication::clipboard()->mimeData()->urls().count()>0){
        contextMenu.addAction(&pasteAct); //Paste
        contextMenu.addAction(&moveAct); //Move
    }
    //
    //If Selected Item Isn't Root Path
    if(targetLoc!=rootPath){
        contextMenu.addAction(&copyAct);
        contextMenu.addAction(&renameAct);
        contextMenu.addAction(&removeAct);
    }
    contextMenu.addSeparator();
    //
    //Folder Explore
    contextMenu.addAction(&exploreFolderAct);
    //
    //Execute Menu
    QAction *selected=contextMenu.exec(parent->mapToGlobal(point));
    //
    //Check Selected
    if(selected){
        //Check Which Selected Action
        QString targetDir=QFileInfo(targetLoc).isDir() && targetLoc!="" ? targetLoc:directoryLoc;
        if(selected==&copyAct){
            //Copy Files
            QMimeData *filesData=new QMimeData();
            QStringList selectedFiles=FileOperations::toStringList(parent->selectionModel()->selectedIndexes());
            filesData->setUrls(FileOperations::toUrlList(selectedFiles));
            QApplication::clipboard()->setMimeData(filesData);

        }else if(selected==&pasteAct){
            //Paste Files

            QList<QUrl> filesData=QApplication::clipboard()->mimeData()->urls();
            FileOperations::copy(parent,filesData,targetDir,parent->fileTypes);


        }else if(selected==&moveAct){
            //Move Files

            QList<QUrl> filesData=QApplication::clipboard()->mimeData()->urls();
            FileOperations::move(parent,filesData,targetDir,parent->fileTypes);


        }else if(selected==&renameAct){
            //Rename File
            parent->edit(parent->currentIndex());

        }else if(selected==&removeAct){
            //Remove Files
            qDebug()<<QFileInfo(targetLoc).absolutePath();
            QDir parentFolder=QDir(targetLoc);
            parentFolder.cdUp();
            parent->setCurrentIndex(fsm->index(parentFolder.absolutePath()));
            if(parentFolder==rootPath)
                parent->setCurrentIndex(parent->rootIndex());
            parent->joinedBrowser->setRootIndex(fileBrowserModel->index(parentFolder.absolutePath()));
            FileOperations::remove(parent,QStringList()<<targetLoc,true);

        }else if(selected==&newFolderAct){
            //Create New Folder
            QString folderName=FileOperations::getValidName(targetLoc,"New Folder");
            QDir(targetLoc).mkdir(folderName);
            //Rename Folder
            qDebug()<<targetLoc+"/"+folderName;
            QModelIndex newFolderIndex=fileBrowserModel->index(QFileInfo(targetLoc+"/"+folderName).absoluteFilePath());
            parent->joinedBrowser->setCurrentIndex(newFolderIndex);
            parent->joinedBrowser->edit(newFolderIndex);

        }else if(selected==&exploreFolderAct){
            //Explore Directory
            QDesktopServices::openUrl(QUrl("file:///"+targetDir));
        }

        //
    }
    //
}
QString ContextMenu::getCollectionType(QStringList *typeList){
    for(int i=0;i<typeList->count();i++){
        QString type=typeList->at(i);
        if(type=="*.t2dobj"){
            return "Objects";
        }else if(type=="*.png"){
            return "Sprites";
        }else if(type=="*.mp3"){
            return "Sounds";
        }else if(type=="*.t2dscene"){
            return "Scenes";
        }else if(type=="*.h"){
            return "Classes";
        }
    }
}

