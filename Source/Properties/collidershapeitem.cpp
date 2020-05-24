#include "collidershapeitem.h"

ColliderShapeItem::ColliderShapeItem(PropertiesItem *propPanel,GameItem *_gameItem,QVariantMap *_collider,QWidget *parent) : QWidget(parent)
{
    gameItem=_gameItem;
    scene=propPanel->scene;

    mainLayout=new QVBoxLayout(this);
    this->setLayout(mainLayout);
    mainLayout->setMargin(2);

    //setting collider props
    collider=_collider;
    QRectF colliderRect=collider->value("rect").toRectF();
    QString colliderType=collider->value("type").toString();
    //Setting shape frame
    QFrame *shapeFrame=new QFrame();
    shapeFrame->setFrameStyle(QFrame::Box);
    shapeFrame->setProperty("itemBox",true);
    QGridLayout *shapeTable=new QGridLayout;
    shapeTable->setMargin(4);
    shapeTable->setSpacing(6);
    shapeFrame->setLayout(shapeTable);


    QLabel *iconLabel=new QLabel();
    QString shapeIconPath=colliderType=="box" ? ":/Icons/uiIcons_mini_colliderbox.png":":/Icons/uiIcons_mini_collidercircle.png";
    iconLabel->setPixmap(QPixmap(shapeIconPath));
    iconLabel->setMaximumWidth(16);
    shapeTable->addWidget(iconLabel,0,0);

    //Shape Properties x,y,w,h
    QStringList BoxColliderLabels;
    QStringList CircleColliderLabels;
    BoxColliderLabels<<"x"<<"y"<<"w"<<"h";
    CircleColliderLabels<<"x"<<"y"<<"r";
    if(colliderType=="box"){
        //For the box collider
        propertiesFields<<new QDoubleSpinBox()<<new QDoubleSpinBox()<<new QDoubleSpinBox()<<new QDoubleSpinBox();
    }else if(colliderType=="circle"){
         //For the circle collider
        propertiesFields<<new QDoubleSpinBox()<<new QDoubleSpinBox()<<new QDoubleSpinBox();
    }
    updateValues();


    for(int n=0;n<propertiesFields.count();n++){
        QLabel *nLabel=new QLabel();
        nLabel->setMaximumWidth(10);
        if(colliderType=="box"){
            nLabel->setText(BoxColliderLabels.at(n));
            shapeTable->addWidget(nLabel,0,n+n+1);
        }else if(colliderType=="circle"){
            nLabel->setText(CircleColliderLabels.at(n));
            shapeTable->addWidget(nLabel,0,n+n+1);
        }
        //Adding prop input field
        QDoubleSpinBox *nSpin=propertiesFields.at(n);
        //nSpin->setButtonSymbols(QDoubleSpinBox::NoButtons);
        //Setting original collider rect
        connect(nSpin,&QDoubleSpinBox::editingFinished,[=]{
            QRectF newRect;
            if(colliderType=="box"){
                newRect.setRect(propertiesFields.at(0)->value(),propertiesFields.at(1)->value(),propertiesFields.at(2)->value(),propertiesFields.at(3)->value());
            }else if(colliderType=="circle"){
                newRect.setRect(propertiesFields.at(0)->value(),propertiesFields.at(1)->value(),propertiesFields.at(2)->value(),propertiesFields.at(2)->value());
            }
            collider->insert("rect",newRect);
            scene->views().at(0)->update();
            scene->updateColliderAnchors();
            scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
        });
        nSpin->setRange(-1000000000,1000000000);
        shapeTable->addWidget(nSpin,0,n+n+2);

    }
    QHBoxLayout *toolButtonsLayout=new QHBoxLayout;
    toolButtonsLayout->setSpacing(1);
    //Edit Button
    editButton=new QPushButton();
    editButton->setCheckable(true);
    editButton->setIcon(QIcon(QPixmap(":/Icons/uiIcons_mini_editbutton.png")));
    editButton->setStyleSheet("padding: 2px;max-width:20;");
    toolButtonsLayout->addWidget(editButton);
    //Remove Button
    removeButton=new QPushButton();
    removeButton->setIcon(QIcon(QPixmap(":/Icons/uiIcons_mini_removebutton.png")));
    removeButton->setStyleSheet("padding: 2px;max-width:20;");
    toolButtonsLayout->addWidget(removeButton);

    shapeTable->addLayout(toolButtonsLayout,0,10);

    //If user click edit button
    connect(editButton,&QPushButton::clicked,[=]{
        propPanel->resetPhysicsEditButtons(this->editButton);
        if(editButton->isChecked()==true){
            checkEditButton();
        }else if(editButton->isChecked()==false){
            uncheckEditButton();
        }

    });

    connect(removeButton,&QPushButton::clicked,[=]{
        int reply=QMessageBox::question(this,"Remove Collider Shape","Do you want to remove collider?",QMessageBox::Yes,QMessageBox::No);
        if(reply==QMessageBox::Yes){

            scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings

            int propPanelId=propPanel->colliderShapeInspectors.indexOf(this);
            propPanel->colliderShapeInspectors.removeAt(propPanelId);

            int id=gameItem->colliderShapes.indexOf(collider);
            gameItem->colliderShapes.removeAt(id);
            delete this;
        }
    });

    mainLayout->addWidget(shapeFrame);

}

void ColliderShapeItem::updateValues()
{
    QRectF colliderRect=collider->value("rect").toRectF();
    QString colliderType=collider->value("type").toString();
    if(colliderType=="box"){
        //For the box collider
        propertiesFields.at(0)->setValue(colliderRect.x());
        propertiesFields.at(1)->setValue(colliderRect.y());
        propertiesFields.at(2)->setValue(colliderRect.width());
        propertiesFields.at(3)->setValue(colliderRect.height());
    }else if(colliderType=="circle"){
         //For the circle collider
        propertiesFields.at(0)->setValue(colliderRect.x());
        propertiesFields.at(1)->setValue(colliderRect.y());
        propertiesFields.at(2)->setValue(colliderRect.width());
    }
}

bool ColliderShapeItem::uncheckEditButton(bool modifyButton)
{
    scene->clearColliderAnchors();
    scene->setEditMode(SceneEditModes::GameItemsEdit);
    if(scene->lastTransformItem!=NULL){
        scene->lastTransformItem->setFlag(QGraphicsItem::ItemIsMovable,true);
    }
    if(modifyButton)
        editButton->setChecked(false);
    return true;
}

bool ColliderShapeItem::checkEditButton(bool modifyButton)
{
    qDebug()<<"editmode";
    scene->views().at(0)->update();
    scene->setEditMode(SceneEditModes::ColliderEdit);
    scene->addColliderAnchors(collider,gameItem,this);
    if(scene->lastTransformItem!=NULL){
        scene->lastTransformItem->setFlag(QGraphicsItem::ItemIsMovable,false);
        //scene->lastTransformItem->itemsContainer->setFlag(QGraphicsItem::ItemIsMovable,false);
    }
    if(modifyButton)
        editButton->setChecked(true);
    return true;
}


