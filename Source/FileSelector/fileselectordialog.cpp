#include "fileselectordialog.h"



FileSelectorDialog::FileSelectorDialog()
{

}

QString FileSelectorDialog::Sprite(QString targetFolderPath,QPoint pos,QString targetPath,bool noneOption,QWidget *parent)
{
    QString *result=new QString("");
    QDialog *dialog=new QDialog(parent);
    dialog->resize(QSize(400,500));
    dialog->setWindowTitle("All Sprites");
    dialog->move(pos);

    QHBoxLayout *mainLayout=new QHBoxLayout(dialog);

    QListView *explorer=new QListView();
    setExplorerProps(explorer);

    FileSelectorModel *fileModel=new FileSelectorModel("Sprite",targetFolderPath,noneOption);
    explorer->setModel(fileModel);

    SpriteSheetSelectorModel *spriteSheetModel=new SpriteSheetSelectorModel();

    mainLayout->addWidget(explorer);

    if(targetPath!=""){
        if(targetPath.indexOf("?frameName=")>0){
            QStringList splittedPath=targetPath.split("?frameName=");
            spriteSheetModel->setTarget(splittedPath.at(0));
            explorer->setModel(spriteSheetModel);
            dialog->setWindowTitle("Sprites of "+QFileInfo(splittedPath.at(0)).fileName());
            QList<QStandardItem*> findedItems=spriteSheetModel->findItems(splittedPath.at(1));
            if(findedItems.count()>0){
                QModelIndex findedIndex=findedItems.at(0)->index();
                explorer->setCurrentIndex(findedIndex);
            }
        }else{
            QString fileName=QFileInfo(targetPath).fileName();
            qDebug()<<fileName;
            QList<QStandardItem*> findedItems=fileModel->findItems(fileName);
            if(findedItems.count()>0){
                QModelIndex findedIndex=findedItems.at(0)->index();
                explorer->setCurrentIndex(findedIndex);
            }
        }
    }

    connect(explorer,&QListView::doubleClicked,[=](const QModelIndex &index){
        SpriteSheetSelectorModel *ssModel=dynamic_cast<SpriteSheetSelectorModel*>(explorer->model());
        QString path=explorer->model()->data(index,Qt::UserRole+1).toString();
        //Checking if user is in the SpriteSheet file
        if(ssModel){
            if(path=="<-back"){
                dialog->setWindowTitle("All Sprites");
                explorer->setModel(fileModel);
            }else{
                *result=path;
                dialog->accept();
            }
        }else{
            //Checking plist file
            if(QFileInfo(path).suffix()=="plist"){
                dialog->setWindowTitle("Sprites of "+QFileInfo(path).fileName());
                spriteSheetModel->setTarget(path);
                explorer->setModel(spriteSheetModel);
            }else{
                *result=path;
                dialog->accept();
            }
        }

    });

    connect(dialog,&QDialog::finished,[=]{

    });

    dialog->exec();
    dialog->close();
    delete dialog;
    return *result;
}

QString FileSelectorDialog::Class(QString targetFolderPath, QPoint pos, QString targetPath,bool noneOption, QWidget *parent)
{
    QString *result=new QString("");
    QDialog *dialog=new QDialog(parent);
    dialog->resize(QSize(400,500));
    dialog->setWindowTitle("All Project Classes");
    dialog->move(pos);

    QHBoxLayout *mainLayout=new QHBoxLayout(dialog);

    QListView *explorer=new QListView();
    setExplorerProps(explorer,false);

    FileSelectorModel *fileModel=new FileSelectorModel("Class",targetFolderPath,noneOption);
    explorer->setModel(fileModel);

    SpriteSheetSelectorModel *spriteSheetModel=new SpriteSheetSelectorModel();

    mainLayout->addWidget(explorer);

    if(targetPath!=""){
        if(targetPath.indexOf("?frameName=")>0){
            QStringList splittedPath=targetPath.split("?frameName=");
            spriteSheetModel->setTarget(splittedPath.at(0));
            explorer->setModel(spriteSheetModel);
            dialog->setWindowTitle("Sprites of "+QFileInfo(splittedPath.at(0)).fileName());
            QList<QStandardItem*> findedItems=spriteSheetModel->findItems(splittedPath.at(1));
            if(findedItems.count()>0){
                QModelIndex findedIndex=findedItems.at(0)->index();
                explorer->setCurrentIndex(findedIndex);
            }
        }else{
            QString fileName=QFileInfo(targetPath).fileName();
            qDebug()<<fileName;
            QList<QStandardItem*> findedItems=fileModel->findItems(fileName);
            if(findedItems.count()>0){
                QModelIndex findedIndex=findedItems.at(0)->index();
                explorer->setCurrentIndex(findedIndex);
            }
        }
    }

    connect(explorer,&QListView::doubleClicked,[=](const QModelIndex &index){
        SpriteSheetSelectorModel *ssModel=dynamic_cast<SpriteSheetSelectorModel*>(explorer->model());
        QString path=explorer->model()->data(index,Qt::UserRole+1).toString();
        //Checking if user is in the SpriteSheet file
        if(ssModel){
            if(path=="<-back"){
                dialog->setWindowTitle("All Sprites");
                explorer->setModel(fileModel);
            }else{
                *result=path;
                dialog->accept();
            }
        }else{
            //Checking plist file
            if(QFileInfo(path).suffix()=="plist"){
                dialog->setWindowTitle("Sprites of "+QFileInfo(path).fileName());
                spriteSheetModel->setTarget(path);
                explorer->setModel(spriteSheetModel);
            }else{
                *result=path;
                dialog->accept();
            }
        }

    });

    connect(dialog,&QDialog::finished,[=]{

    });

    dialog->exec();
    dialog->close();
    delete dialog;
    return *result;
}

void FileSelectorDialog::setExplorerProps(QListView *listview,bool iconMode)
{
    if(iconMode==true){
        listview->setViewMode(QListView::IconMode);
        listview->setGridSize(QSize(80,80));
        listview->setIconSize(QSize(48,48));
        listview->setSpacing(20);
        listview->setUniformItemSizes(false);
        listview->setWordWrap(true);
    }
    listview->setAcceptDrops(false);
    listview->setResizeMode(QListView::Adjust);
    listview->setTextElideMode(Qt::ElideMiddle);
}
