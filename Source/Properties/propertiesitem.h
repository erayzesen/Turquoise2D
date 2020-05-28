#pragma once

#include <QWidget>
#include <Canvas/selectiontransformitem.h>
#include <QLabel>
#include <QLayout>
#include <QToolBox>
#include <QLineEdit>
#include <commands.h>
#include "mathoperations.h"
#include "QIntValidator"
#include "QFrame"
#include "QPushButton"
#include "FileSelector/fileselectordialog.h"
#include "QStandardItem"
#include "Properties/collidershapeitem.h"

class ColliderShapeItem;
class PropertiesItem : public QWidget
{
    Q_OBJECT
    QVBoxLayout *mainLayout;
public:
    explicit PropertiesItem(SelectionTransformItem *_transformItem,QWidget *parent = 0);
    void AddTransformProp();
    void AddSpriteProp();
    void AddObjectProp();
    void AddMultipleProp();
    void AddPhysicsProp();
    void resetPhysicsEditButtons(QPushButton *except=NULL);
    void UpdateTransformProp();
    void UpdateGameItemPositionX();
    void UpdateGameItemPositionY();
    void UpdateGameItemScales();
    void UpdateGameItemRotations();
    void UpdateGameItemZOrders();
    bool areAllPositionsSame(bool checkXPos,bool checkYPos);
    bool areAllSpritesSame();
    bool areAllItemsSprite();
    bool areAllItemsObject();
    bool areAllTagsSame();
    SelectionTransformItem *transformItem;
    QLineEdit *posX_ui;
    QLineEdit *posY_ui;
    QLineEdit *scale_ui;
    QLineEdit *rotation_ui;
    QSpinBox *zOrder_ui;
    QLineEdit *nameField;
    QString *spriteSource=NULL;
    GraphicsScene *scene;
    QList<ColliderShapeItem*> colliderShapeInspectors;

signals:

public slots:
};

