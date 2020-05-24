#include "appsettings.h"

AppSettings::AppSettings(QWidget *parent) : QWidget(parent)
{

}

QList<QVariantMap> AppSettings::getRecentProjects()
{
    QList<QVariantMap> result;
    QSettings settings("AppSettings.ini",QSettings::IniFormat);
    qDebug()<<settings.value("RecentProjects").toString();
    int size=settings.beginReadArray("RecentProjects");
    qDebug()<<size;
    if(size>0){
        for (int i = size-1; i >= 0; i--){
            settings.setArrayIndex(i);
            QVariantMap map;
            QString projectPath=settings.value("projectPath").toString();
            QString frameworkName=settings.value("frameworkName").toString();
            projectPath=projectPath.replace(">>","/");
            if(QDir(projectPath).exists()){
                map.insert("projectPath",projectPath);
                map.insert("frameworkName",frameworkName);
                result<<map;
            }
        }
    }

    settings.endArray();
    return result;
}

QList<QVariantMap> AppSettings::addRecentProject(QString projectPath,QString frameworkName)
{

    QString formattedProjectPath=projectPath;
    formattedProjectPath=formattedProjectPath.replace("/",">>");

    QSettings settings("AppSettings.ini",QSettings::IniFormat);

    //Checking if there is the same project entry.
    int size= settings.beginReadArray("RecentProjects");
    QList<QVariantMap> tempRecentProjects;
    for(int i=0;i<size;i++){
        settings.setArrayIndex(i);
        QString nProjectPath=settings.value("projectPath").toString();
        QString nDecodedProjectPath=nProjectPath.replace(">>","/");
        QString nFrameworkName=settings.value("frameworkName").toString();
        QVariantMap nMap;
        // Don't add item to temp if this project path the same as the project path.
        if(nDecodedProjectPath!=projectPath && QDir(nDecodedProjectPath).exists()){
            nMap.insert("projectPath",nProjectPath);
            nMap.insert("frameworkName",nFrameworkName);
            tempRecentProjects<<nMap;
        }
    }
    settings.endArray();
    //Keeping maximum recent project count and removing extra project items
    while(tempRecentProjects.count()>=maxRecentProjectsCount){
        tempRecentProjects.removeAt(0);
    }
     //Clearing RecentProjects array and adding edited recentProjects list
    settings.beginWriteArray("RecentProjects");
    settings.clear();
    for(int i=0;i<tempRecentProjects.count();i++){
        settings.setArrayIndex(i);
        QVariantMap nItem=tempRecentProjects.at(i);
        settings.setValue("projectPath",nItem.value("projectPath"));
        settings.setValue("frameworkName",nItem.value("frameworkName"));
    }
    settings.endArray();

    //Getting new recent projects count
    size= settings.beginReadArray("RecentProjects");
    settings.endArray();

    settings.beginWriteArray("RecentProjects");
    settings.setArrayIndex(size);

    settings.setValue("projectPath",formattedProjectPath);
    settings.setValue("frameworkName",frameworkName);

    settings.endArray();


    return getRecentProjects();

}

void AppSettings::keepRecentProjectsCount()
{
    int maxCount=15;


}

