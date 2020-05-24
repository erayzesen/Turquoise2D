#include "hierarchypanel.h"

HierarchyPanel::HierarchyPanel(GraphicsScene *_scene,Turquoise2DEditor *_mainWindow,QWidget *parent) : QWidget(parent)
{
    scene=_scene;
    mainWindow=_mainWindow;
    hierarchyList=new HierarchyListView(scene->sceneJsonObject,scene);
    mainLayout=new QVBoxLayout(this);
    mainLayout->setContentsMargins(4,0,4,4);

    mainLayout->addWidget(hierarchyList);

    QHBoxLayout *toolsLayout=new QHBoxLayout();
    toolsLayout->setAlignment(Qt::AlignRight);

    //LOCK BUTTON
    lockButton=new QPushButton(QIcon(QPixmap(":/Icons/toolicon_lock.png")),"");
    lockButton->setMaximumWidth(30);
    lockButton->setCheckable(true);
    toolsLayout->addWidget(lockButton);

    toolsLayout->addStretch();

     //ADD BUTTON
    addButton=new QPushButton(QIcon(QPixmap(":/Icons/uiIcons_mini_addbutton.png")),"Add");
    addButton->setMinimumWidth(100);
    toolsLayout->addWidget(addButton);
    QMenu *addMenu=new QMenu(this);
    //SUB-MENUS AND ACTIONS
    //QMenu *levelMenu=addMenu->addMenu("Level Elements");
    QAction *actObject=addMenu->addAction(QIcon(":/Icons/object_type_16x16.png"),"Object");
    QAction *actSprite=addMenu->addAction(QIcon(":/Icons/sprite_type_16x16.png"),"Sprite");

    //QMenu *uiMenu=addMenu->addMenu("UI Elements");

    addButton->setMenu(addMenu);

    connect(addMenu,&QMenu::triggered,[=](QAction *action){
       QString name;
       if(action==actObject){
           qDebug()<<"adding object";
           name="EmptyObject";
           QUndoCommand *cmd=new AddObjectItemCommand(scene,QPointF(0,0));
           scene->stack->push(cmd);


       }else if(action==actSprite){
           qDebug()<<"adding sprite";
       }
    });


    connect(lockButton,&QPushButton::toggled,[=](bool checked){
        if(scene->lastTransformItem){
            QList<QGraphicsItem*> items=scene->lastTransformItem->items;
            QList<HierarchyItem*> hItemList;
            for(int i=0;i<items.count();i++){
                GameItem *gItem=dynamic_cast<GameItem*>(items.at(i));
                if(gItem){
                    hItemList<<gItem->hierarchyItem;
                }
            }
            hierarchyList->setItemsLock(hItemList,checked);

        }
    });





    mainLayout->addLayout(toolsLayout);
}

HierarchyPanel::~HierarchyPanel()
{
    delete hierarchyList;
}

