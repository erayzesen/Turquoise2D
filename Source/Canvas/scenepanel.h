#pragma once 

#include <QWidget>
#include "Properties/propertiespanel.h"
#include "graphicsscene.h"
#include "QLayout"
#include "turquoise2deditor.h"
#include "Canvas/graphicsview.h"
#include "QPalette"

class Turquoise2DEditor;
class PropertiesPanel;
class ScenePanel : public QWidget
{
    Q_OBJECT
public:
    Turquoise2DEditor *mainWindow;
    PropertiesPanel *propPanel;
    explicit ScenePanel(QString _sceneFilePath,PropertiesPanel *_propPanel,Turquoise2DEditor *_mainWindow,QWidget *parent = 0);
    ~ScenePanel();
    QVariantMap * createGridOptions();
    GraphicsScene *scene;
    QString sceneFilePath;
    QString sceneFileName;
    QHBoxLayout *toolsLayout;
    QHBoxLayout *drawToolsLayout;
    void clearToolsLayout();
    void addDrawTools();
    void addZoomTools();

    //tools elements
    QPushButton *eyedropperButton=NULL;
    QLabel *eyedropperViewer=NULL;
    QLabel *eyedropperNameField=NULL;
    QPushButton *percent100=NULL;
    QLabel *zoomResult=NULL;

    //Scene prop elements
    QPushButton *snapButton;

    bool isSceneChanged=false;
    void sceneChanged();


signals:

public slots:
};
