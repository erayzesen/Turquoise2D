#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QListView>
#include <QDropEvent>
#include <QDebug>
#include <QMimeData>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QList>
#include <QApplication>

class FileOperations
{
public:
    FileOperations();
    static bool copy(QWidget *parent,QList<QUrl> files,QString targetDir,QStringList *validExtensions);
    static bool copyFolder(QString src, QString dst);
    static void move(QWidget *parent,QList<QUrl> files,QString targetDir,QStringList *validExtensions);
    static void remove(QWidget *parent,QStringList files,bool question=false);
    static QStringList toStringList(QModelIndexList list);
    static QStringList toStringList(QList<QUrl> list);
    static QList<QUrl> toUrlList(QStringList list);
    static QString getValidName(QString targetDirectory,QString name);
    static bool checkInterferingPath(QList<QUrl> files,QString targetDir);
    static QString cutUrlFrom(QString referenceString,QString originalPath);
    static QString getAppRootPath();
};

#endif // FILEOPERATIONS_H
