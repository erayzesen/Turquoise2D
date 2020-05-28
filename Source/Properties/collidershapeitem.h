#pragma once

#include <QWidget>
#include <QtWidgets>
#include <QLabel>
#include <Canvas/gameitem.h>
#include <Canvas/graphicsscene.h>
#include <Properties/propertiesitem.h>

class PropertiesItem;
class GraphicsScene;
class ColliderShapeItem : public QWidget
{
    Q_OBJECT
public:
    explicit ColliderShapeItem(PropertiesItem *propPanel,GameItem *_gameItem,QVariantMap *_collider,QWidget *parent = 0);
    GraphicsScene *scene;
    GameItem *gameItem;
    QVariantMap *collider;
    QVBoxLayout *mainLayout;
    QList<QDoubleSpinBox*> propertiesFields;
    QPushButton *editButton;
    QPushButton *removeButton;
    void updateValues();
    bool uncheckEditButton(bool modifyButton=false);
    bool checkEditButton(bool modifyButton=false);


signals:

public slots:
};

