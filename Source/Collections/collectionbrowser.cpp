#include "collectionbrowser.h"
#include "ui_collectionbrowser.h"
#include "turquoise2deditor.h"

CollectionBrowser::CollectionBrowser(QString _targetFolder,QStringList _nameFilter,Turquoise2DEditor *_mainWindow,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CollectionBrowser)
{
    ui->setupUi(this);
    mainWindow=_mainWindow;
    targetFolder=_targetFolder;
    nameFilter=_nameFilter;
    fileModel=new CollectionFileSystemModel();
    folderModel=new CollectionFolderSystemModel();
    spriteSheetModel=new CollectionSpriteSheetModel();
    initBrowser();

    //SHORTCUTS
    //new QShortcut(QKeySequence(Qt::Key_Backspace),this,SLOT(on_press_KeyDelete()) ); // Collection Shortcut: KEY_BACKSPACE
    //new QShortcut(QKeySequence(Qt::Key_Enter),this,SLOT(on_press_KeyEnter()) );
    //new QShortcut(QKeySequence(Qt::Key_Return),this,SLOT(on_press_KeyEnter()) );


}

CollectionBrowser::~CollectionBrowser()
{
    delete ui;
}
void CollectionBrowser::on_press_KeyDelete(){
    if(QApplication::focusWidget()==ui->browser){
        QModelIndexList indexes=ui->browser->selectionModel()->selectedIndexes();
        QStringList selectedFiles;
        for(int i=0;i<indexes.count();i++){
            selectedFiles << indexes.at(i).data(QFileSystemModel::FilePathRole).toString();
        }
        //qDebug()<<"Deleted this files: "<<selectedFiles;
        FileOperations::remove(this,selectedFiles,true);

    }

}

void CollectionBrowser::on_press_KeyEnter(){
    if(QApplication::focusWidget()==ui->browser){
        QModelIndexList indexes=ui->browser->selectionModel()->selectedIndexes();
        if(indexes.count()!=1){
            return;
        }
        QModelIndex index=ui->browser->currentIndex();
        ui->browser->edit(index);

    }else{
        if(QApplication::focusObject()->parent()->parent()->objectName()=="browser")
        ui->browser->setFocus();
    }

}


void CollectionBrowser::initBrowser(){
    QDir targetUpFolder(targetFolder);
    targetUpFolder.cdUp();
    folderModel->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    folderModel->setNameFilters(QStringList()<<"*.plist");
    folderModel->setNameFilterDisables(false);
    folderModel->setRootPath(targetFolder);
    folderModel->setReadOnly(false);
    customModel=new customFolderModel(targetFolder);
    customModel->setSourceModel(folderModel);
    ui->folderBrowser->setModel(customModel);

    ui->folderBrowser->setColumnHidden(1,true);
    ui->folderBrowser->setColumnHidden(2,true);
    ui->folderBrowser->setColumnHidden(3,true);

    ui->folderBrowser->setRootIndex(customModel->mapFromSource(folderModel->index(targetUpFolder.absolutePath())));
    ui->folderBrowser->joinedBrowser=this->ui->browser;


    fileModel->setRootPath(targetFolder);
    fileModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    fileModel->setNameFilters(nameFilter);
    fileModel->setReadOnly(false);
    ui->browser->setModel(fileModel);
    ui->browser->setRootIndex(fileModel->index(targetFolder));
    ui->browser->fileTypes=&nameFilter;
    ui->browser->setWordWrap(true);
    ui->browser->joinedBrowser=this->ui->folderBrowser;

    //ui->browser->setAcceptDrops(true);
}

void CollectionBrowser::enterFolder(QString _targetFolder){
    ui->browser->setModel(fileModel);
    ui->browser->setRootIndex(fileModel->index(_targetFolder));
}

void CollectionBrowser::enterSpriteSheet(QString _targetFile)
{
    spriteSheetModel->setTarget(_targetFile);
    ui->browser->setModel(spriteSheetModel);
}

void CollectionBrowser::on_browser_doubleClicked(const QModelIndex &index)
{
    QFileSystemModel *fsm=dynamic_cast<QFileSystemModel*>(ui->browser->model());
    if(!fsm){
        return;
    }
    QString filePath=fileModel->fileInfo(index).absoluteFilePath();
    if(fileModel->fileInfo(index).isDir()){
        enterFolder(filePath);
        ui->folderBrowser->setCurrentIndex(customModel->mapFromSource(folderModel->index(filePath)));
    }else if(fileModel->fileInfo(index).suffix()=="plist"){
        if(SpriteSheet::isSpritSheet(filePath)){
            enterSpriteSheet(filePath);
            ui->folderBrowser->setCurrentIndex(customModel->mapFromSource(folderModel->index(filePath)));
        }
    }else if(fileModel->fileInfo(index).suffix()=="t2dscene"){
        qDebug()<<filePath;
        mainWindow->addNewSceneTab(filePath);
    }
}

void CollectionBrowser::on_folderBrowser_clicked(const QModelIndex &index)
{
    QFileInfo info(customModel->data(index,QFileSystemModel::FilePathRole).toString());
    if(info.isDir()){
        QString filePath=customModel->data(index,QFileSystemModel::FilePathRole).toString();
        enterFolder(filePath);
    }else if(info.suffix()=="plist"){
        if(SpriteSheet::isSpritSheet(info.absoluteFilePath())){
            qDebug()<<"spritesheet'e tıklandi";
            enterSpriteSheet(info.absoluteFilePath());
        }
    }
}

