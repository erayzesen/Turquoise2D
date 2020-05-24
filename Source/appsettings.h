#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QVariantMap>
#include <QDebug>
#include <QDir>


class AppSettings : public QWidget
{
    Q_OBJECT
public:
    explicit AppSettings(QWidget *parent = 0);
    static QList<QVariantMap> getRecentProjects();
    static QList<QVariantMap> addRecentProject(QString projectPath,QString frameworkName);
    static void keepRecentProjectsCount();

    //general definations
    static const int maxRecentProjectsCount=15;

signals:

public slots:
};

#endif // APPSETTINGS_H
