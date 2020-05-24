#ifndef HIERARCHYPANEL_H
#define HIERARCHYPANEL_H

#include <QWidget>
#include "Canvas/graphicsscene.h"
#include "Hierarchy/hierarchylistview.h"
#include "turquoise2deditor.h"
#include "QPushButton"

class Turquoise2DEditor;
class HierarchyListView;
class HierarchyPanel : public QWidget
{
    Q_OBJECT
public:
    explicit HierarchyPanel(GraphicsScene *_scene,Turquoise2DEditor *_mainWindow, QWidget *parent = 0);
    ~HierarchyPanel();
    GraphicsScene *scene;
    Turquoise2DEditor *mainWindow;
    HierarchyListView *hierarchyList;
    QVBoxLayout *mainLayout;
    QPushButton *addButton;
    QPushButton *lockButton;

signals:

public slots:
};

#endif // HIERARCHYPANEL_H
