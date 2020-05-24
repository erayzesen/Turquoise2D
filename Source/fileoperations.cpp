#include "fileoperations.h"

FileOperations::FileOperations()
{

}

bool FileOperations::copy(QWidget *parent,QList<QUrl> files,QString targetDir,QStringList *validExtensions){
    //Overwrite Boolean
    bool overwriteAll=false;
    //
    //Check For If Directories is the Same as TargetDir, Cancel Function And Call Error
    if(!FileOperations::checkInterferingPath(files,targetDir)){
        QMessageBox::critical(parent,"Copy Error!","You can't paste '"+QFileInfo(targetDir).fileName()+"' at this location because you can't paste an item into itself!");
        return false;
    }
    //
    //Check The All Files
    bool isError=false;
    for(int i=0;i<files.count();i++){
        //Url
        QUrl url=files.at(i);
        //Url Path
        QString filePath=url.path();
        filePath=filePath.lastIndexOf("/")==filePath.length()-1 ? filePath.mid(0,filePath.length()-1):filePath;
        filePath=QDir::toNativeSeparators(filePath);
        //qDebug()<<QDir(targetDir).path();
        // If File Come From Target Directory Cancel Copy File
        if(QFileInfo(filePath).absoluteDir().absolutePath()==QDir(targetDir).path()){
            isError=true;
            break;
        }
        //If File is a Directory, Re-Calling Copy Function for It.

        if(QFileInfo(filePath).isDir()){
            // Get Files From Directory
            QDir dir(filePath);
            dir.setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot );
            QFileInfoList list=dir.entryInfoList();
            QList<QUrl> tempfiles;
            for(int f=0;f<list.count();f++){
                QUrl fUrl(list.at(f).absoluteFilePath());
                tempfiles.append(fUrl.toString());
                qDebug()<<fUrl.toString();
            }
            //
            //Get the folder's name
            QString folderName=QFileInfo(filePath).fileName();
            //
            //Create New Folder into Target Directory
            qDebug()<<"folder path: "<< folderName;
            if(!QDir(targetDir).mkdir(folderName)){
                QMessageBox::critical(parent,"Folder can't creating!","Can't creating folder: "+folderName);
                isError=true;
                return isError;
            }
            //
            //Copy Files from to be Copied Folder
            if(tempfiles.count()>0) {
                bool isCopied=FileOperations::copy(parent,tempfiles,targetDir+"/"+folderName,validExtensions);
                //if isError currently false, isError variable equate to isCopied return variable.
                if(isError==false ) isError=isCopied;
            }
            //
        }else{
            QString fileExtension="*."+QFileInfo(filePath).completeSuffix();
            qDebug()<<fileExtension;
            bool extensionControl=false;
            // Checking is the file Extensions Valid
            for(int n=0;n<validExtensions->count();n++){
                qDebug()<<validExtensions->at(n);
                if(fileExtension==validExtensions->at(n)){
                    extensionControl=true;
                }
            }
            //If File Extension is Invalid Call the Error Message
            if(extensionControl==false) {
                isError=true;
                QMessageBox msgError;
                msgError.setText("This file extension is not valid for this collection. \n '"+url.fileName()+"'");
                msgError.exec();
                continue;
            }
            // If File is Exist in Target Directory, Asking Question to User
            if(QFile::exists(targetDir+"/"+url.fileName()) && overwriteAll==false ){
                QString textMsg="The file '"+url.fileName() +"'' already exist! Do you want replace it?";
                int msgOverwrite;
                if(files.count()>1){
                    msgOverwrite=QMessageBox::question(parent,"File already exist!",textMsg,QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::YesAll,QMessageBox::Yes);
                }else{
                    msgOverwrite=QMessageBox::question(parent,"File already exist!",textMsg,QMessageBox::Yes | QMessageBox::Cancel,QMessageBox::Yes);
                }

                if(msgOverwrite==QMessageBox::YesAll) {
                    overwriteAll=true;
                }else if(msgOverwrite==QMessageBox::Yes){
                    //
                }else{
                    qDebug()<<"dosya kopyalama iptal edildi!";
                    isError=true;
                    break;
                }
                // If Overwrite Confirm by User, Removing Old File and Copying New File
                QFile::remove(targetDir+"/"+url.fileName());

            }else if(QFile::exists(targetDir+"/"+url.fileName()) && overwriteAll==true){
                QFile::remove(targetDir+"/"+url.fileName());
            }
            // Final; File Copying to Target Directory
            QFile::copy(filePath,targetDir+"/"+url.fileName());

        }
    }
    return isError;
}

void FileOperations::move(QWidget *parent,QList<QUrl> files,QString targetDir,QStringList *validExtensions){
    //Overwrite Boolean
    bool overwriteAll=false;
    //Check For If Directories is the Same as TargetDir, Cancel Function And Call Error
    if(!FileOperations::checkInterferingPath(files,targetDir)){
        QMessageBox::critical(parent,"Copy Error!","You can't move '"+QFileInfo(targetDir).fileName()+"' at this location because you can't move an item into itself!");
        return;
    }
    //
    //Check The All Files
    for(int i=0;i<files.count();i++){
        //Url
        bool isError=false;
        QUrl url=files.at(i);
        //Url Path
        QString filePath=url.path();
        filePath=filePath.lastIndexOf("/")==filePath.length()-1 ? filePath.mid(0,filePath.length()-1):filePath;
        filePath=QDir::toNativeSeparators(filePath);
        // If File Come From Target Directory Cancel Copy File
        if(QFileInfo(filePath).absoluteDir().absolutePath()==QDir(targetDir).path()){
            break;
        }
        //If File is a Directory, Re-Call Move Function for It.
        if(QFileInfo(filePath).isDir()){
            // Get Files From Directory
            QDir dir(filePath);
            dir.setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
            QFileInfoList list=dir.entryInfoList();
            QList<QUrl> tempfiles;
            for(int f=0;f<list.count();f++){
                QUrl fUrl(list.at(f).absoluteFilePath());
                tempfiles.append(fUrl.toString());
            }
            //
            //Get the folder's name
            QString folderName=QFileInfo(filePath).fileName();
            //
            //Create New Folder into Target Directory
            if(!QDir(targetDir).mkdir(folderName)){
                QMessageBox::critical(parent,"Folder can't creating!","Can't creating folder: "+filePath);
            }
            //
            //Move Files from to be Move Folder
            if(tempfiles.count()>0){
                isError=FileOperations::copy(parent,tempfiles,targetDir+"/"+folderName,validExtensions);
            }
            //
            //Remove Old Location Folder
            if(isError==false){
                FileOperations::remove(parent,QStringList() << filePath,false);
            }
            /*
            if(!dir.rmdir(filePath)){
                QMessageBox::critical(parent,"Folder can't removing!","Can't removing folder: "+filePath);
            }
            */
            //
        }else{
            QString fileExtension="*."+QFileInfo(filePath).completeSuffix();
            qDebug()<<fileExtension;
            bool extensionControl=false;
            // Checking is the file Extensions Valid
            for(int n=0;n<validExtensions->count();n++){
                qDebug()<<validExtensions->at(n);
                if(fileExtension==validExtensions->at(n)){
                    extensionControl=true;
                }
            }
            //If File Extension is Invalid Call the Error Message
            if(extensionControl==false) {
                QMessageBox msgError;
                msgError.setText("This file extension is not valid for this collection. \n '"+url.fileName()+"'");
                msgError.exec();
                continue;
            }
            // If File is Exist in Target Directory, Asking Question to User
            if(QFile::exists(targetDir+"/"+url.fileName()) && overwriteAll==false ){
                QString textMsg="The file '"+url.fileName() +"'' already exist! Do you want replace it?";
                int msgOverwrite;
                if(files.count()>1){
                    msgOverwrite=QMessageBox::question(parent,"File already exist!",textMsg,QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::YesAll,QMessageBox::Yes);
                }else{
                    msgOverwrite=QMessageBox::question(parent,"File already exist!",textMsg,QMessageBox::Yes | QMessageBox::Cancel,QMessageBox::Yes);
                }

                if(msgOverwrite==QMessageBox::YesAll) {
                    overwriteAll=true;
                }else if(msgOverwrite==QMessageBox::Yes){
                    //
                }else{
                    qDebug()<<"Cancaled copy file!";
                    break;
                }
                // If Overwrite Confirm by User, Removing Old File and Copying New File
                QFile::remove(targetDir+"/"+url.fileName());

            }else if(QFile::exists(targetDir+"/"+url.fileName()) && overwriteAll==true){
                QFile::remove(targetDir+"/"+url.fileName());
            }
            // Final; File Copying to Target Directory
            //QFile::copy(filePath,targetDir+"/"+url.fileName());
            QFile::rename(filePath,targetDir+"/"+url.fileName());

        }
    }
}

void FileOperations::remove(QWidget *parent,QStringList files,bool question){
    if(files.count()==0)return;
    qDebug()<<files;
    QString typeTarget=QFileInfo(files.at(0)).isDir()==true ? "folder":"file";
    QString firstFileName=files.at(0);
    firstFileName.remove(0,firstFileName.lastIndexOf("/")+1);
    if(question==true){
        QString textMsg=files.count()==1 ? "Are you sure you want to permanently remove '" + firstFileName + "' named " +typeTarget + "?" : "Are you sure you want to permanently remove these " + QString::number(files.count())+ " files?";
        int msgQuestion=QMessageBox::warning(parent,"Delete Files!",textMsg,QMessageBox::Yes | QMessageBox::Cancel,QMessageBox::Yes);
        if(msgQuestion!=QMessageBox::Yes){
            return;
        }
    }

    for(int i=0;i<files.count();i++){
        QString filePath=files.at(i);
        qDebug()<<"Removing file is : "+ filePath;
        if(QFileInfo(filePath).isDir()){
            QDir dir(filePath);
            QFileInfoList inFilesInfo=dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::Dirs | QDir::NoDotAndDotDot);
            QStringList inFiles;
            qDebug()<<"**folderin icindeki dosyalar:" ;
            for(int n=0;n<inFilesInfo.count();n++){
                qDebug()<<QString::number(n)<<"-" << inFilesInfo.at(n).absoluteFilePath();
                inFiles<<inFilesInfo.at(n).absoluteFilePath();
            }
            //qDebug()<<"folderin icindeki dosyalar: " << inFiles;
            if(inFiles.count()>0) FileOperations::remove(parent,inFiles);
            if(!dir.rmdir(filePath)){
                QMessageBox::critical(parent,"Folder can't removing!","Can't removing folder: "+filePath);
            }
            continue;
        }
        QFile::remove(filePath);
    }

}
QStringList FileOperations::toStringList(QModelIndexList list){
    QStringList stringList;
    for(int i=0;i<list.count();i++){
        stringList<<list.at(i).data(QFileSystemModel::FilePathRole).toString();
    }
    return stringList;
}
QStringList FileOperations::toStringList(QList<QUrl> list){
    QStringList stringList;
    for(int i=0;i<list.count();i++){
        stringList<<QFileInfo(list.at(i).url()).absoluteFilePath();
    }
    return stringList;
}
QString FileOperations::getValidName(QString targetDirectory,QString name){
    QDir fileDir(targetDirectory);
    QStringList fileList=fileDir.entryList(QDir::AllEntries);
    if(!QFileInfo(targetDirectory+"/"+name).exists())
        return name;
    int n=1;
    QString newName=name+" "+QString::number(n);
    do{
        n++;
        newName=name+" "+QString::number(n);
    }
    while(QFileInfo(targetDirectory+"/"+newName).exists());
    return newName;
}
QList<QUrl> FileOperations::toUrlList(QStringList list){
    QList<QUrl> temp;
    for(int i=0;i<list.count();i++){
        QUrl nUrl(list.at(i));
        temp.append(nUrl);
    }
    return temp;
}
//This Function can Check Interfering Between Directory and Target Directories
bool FileOperations::checkInterferingPath(QList<QUrl> files,QString targetDir){
    for(int i=0;i<files.count();i++){
        QString url=files.at(i).path();
        if(QFileInfo(url).isDir() && url==QFileInfo(targetDir).absoluteFilePath()){
            return false;
        }
    }
    return true;
}

QString FileOperations::cutUrlFrom(QString referenceString, QString originalPath)
{

}

QString FileOperations::getAppRootPath()
{
    QDir appRootPath=QDir(QApplication::applicationDirPath());
    appRootPath.cdUp();appRootPath.cdUp();appRootPath.cdUp();
    return appRootPath.absolutePath();
}

bool FileOperations::copyFolder(QString src, QString dst)
{
    qDebug()<<"Starting copy "<<src<<" named folder to "<<dst<<".";
    QDir dir(src);
    if (! dir.exists())
        return false;
    if(QDir(dst).exists()==false){
        dir.mkpath(dst);
    }
    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = dst + QDir::separator() + d;
        dir.mkpath(dst_path);
        copyFolder(src+ QDir::separator() + d, dst_path);
    }

    foreach (QString f, dir.entryList(QDir::Files)) {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }
    qDebug()<<"Finished copy folder: "<<dst;
    return true;

}
//
