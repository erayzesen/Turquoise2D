#include "propertiesitem.h"

PropertiesItem::PropertiesItem(SelectionTransformItem *_transformItem,QWidget *parent) : QWidget(parent)
{
    transformItem=_transformItem;
    mainLayout=new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setAlignment(Qt::AlignTop);


    scene=dynamic_cast<GraphicsScene*>(transformItem->scene);

    if(transformItem->items.count()==0){
        return;
    }
    if(areAllItemsSprite()){
        AddSpriteProp();
    }else if(areAllItemsObject()){
        AddObjectProp();
        //Adding Object properties
    }else{
        AddMultipleProp();
    }
    AddTransformProp();
    AddPhysicsProp();

}


//TRANSFORM PROP
void PropertiesItem::AddTransformProp()
{
    QList<QLineEdit*> inputFields;
    QGroupBox* groupBox=new QGroupBox("Transform");

    //Valitadors

    QRegExpValidator* posValidator = new QRegExpValidator( QRegExp("[+-]*[0-9]*"), 0);

    //ADD POSITION
    QVBoxLayout *vert=new QVBoxLayout;
    vert->setAlignment(Qt::AlignTop);
    QHBoxLayout *horz0=new QHBoxLayout;
    horz0->addWidget(new QLabel("Position"));
    QHBoxLayout *horz1=new QHBoxLayout;

    horz1->addWidget(new QLabel("X"));

    posX_ui=new QLineEdit;
    posX_ui->setText("0");
    posX_ui->setValidator(posValidator);
    horz1->addWidget(posX_ui);

    horz1->addWidget(new QLabel("Y"));
    posY_ui=new QLineEdit;
    posY_ui->setText("0");
    posY_ui->setValidator(posValidator);
    horz1->addWidget(posY_ui);

    vert->addLayout(horz0);
    vert->addLayout(horz1);

    //ADD ROTATION AND SCALE
    horz1=new QHBoxLayout;

    horz1->addWidget(new QLabel("Rotation"));
    rotation_ui=new QLineEdit;
    rotation_ui->setText("0");
    horz1->addWidget(rotation_ui);

    horz1->addWidget(new QLabel("Scale"));
    scale_ui=new QLineEdit;
    scale_ui->setText("0");
    horz1->addWidget(scale_ui);

    vert->addLayout(horz1);

    //ADD Z-INDEX
    horz1=new QHBoxLayout;

    horz1->addWidget(new QLabel("Z-Order"));
    zOrder_ui=new QSpinBox;
    zOrder_ui->setRange(0,999999);
    horz1->addWidget(zOrder_ui);
    horz1->addStretch();

    vert->addLayout(horz1);



    groupBox->setLayout(vert);
    mainLayout->addWidget(groupBox);
    UpdateTransformProp();
    //If edited PosX field
    connect(posX_ui,&QLineEdit::editingFinished,[=]{
        UpdateGameItemPositionX();
        scene->updateColliderAnchors();
    });
    //If edited PosY field
    connect(posY_ui,&QLineEdit::editingFinished,[=]{
        UpdateGameItemPositionY();
        scene->updateColliderAnchors();
    });
    //If edited Rotation field
    connect(rotation_ui,&QLineEdit::editingFinished,[=]{
        UpdateGameItemRotations();
        scene->updateColliderAnchors();
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });
    //If edited Scale field
    connect(scale_ui,&QLineEdit::editingFinished,[=]{
        UpdateGameItemScales();
        scene->updateColliderAnchors();
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });
    //If edited z-order field
    connect(zOrder_ui,&QSpinBox::editingFinished,[=]{
        UpdateGameItemZOrders();
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });
    //Setting general inputField Properties
    inputFields<<posX_ui<<posY_ui<<scale_ui<<rotation_ui;

    for(int i=0;i<inputFields.count();i++){
        QLineEdit *ifield=inputFields.at(i);
        connect(ifield,&QLineEdit::returnPressed,[=]{
            //return pressed
        });

        connect(ifield,&QLineEdit::textEdited,[=](const QString str){
            //text edited
        });
    }



}

//SPRITE PROP
void PropertiesItem::AddSpriteProp()
{
    GameItem *gi=dynamic_cast<GameItem*>(transformItem->items.at(0));
    if(gi==NULL){
        return;
    }
    QList<QLineEdit*> inputFields;

    QHBoxLayout* mainHorz=new QHBoxLayout;
    mainHorz->setAlignment(Qt::AlignTop);

    QGridLayout *vertLeft=new QGridLayout;
    vertLeft->setAlignment(Qt::AlignTop);
    QVBoxLayout *vertRight=new QVBoxLayout;
    vertRight->setAlignment(Qt::AlignTop);

    //RIGHT SIDE

    QLabel *typeIcon=new QLabel;
    typeIcon->setAlignment(Qt::AlignCenter);
    QPixmap iconPixmap(":/Icons/sprite_type_32x32.png");
    typeIcon->setPixmap(iconPixmap);
    vertRight->addWidget(typeIcon);

    QLabel *typeTitle=new QLabel("Sprite",this);
    typeTitle->setAlignment(Qt::AlignCenter);
    vertRight->addWidget(typeTitle);

    //LEFT SIDE
    vertLeft->addWidget(new QLabel("Tag",this),0,0);
    QLineEdit *tagField=new QLineEdit;
    if(transformItem->items.count()==1 || areAllTagsSame()){
        tagField->setText(gi->tag);
    }else{
    }
    vertLeft->addWidget(tagField,0,1,1,2);

    nameField=new QLineEdit;
    if(transformItem->items.count()==1){
        nameField->setText(gi->name);
    }else{
        nameField->setEnabled(false);
    }
    vertLeft->addWidget(new QLabel("Name",this),1,0);
    vertLeft->addWidget(nameField,1,1,1,2);

    vertLeft->addWidget(new QLabel("Sprite Type",this),2,0);
    QString typeText;
    if(transformItem->items.count()==1){
        typeText=gi->SpriteInfo["type"].toString()=="SpriteSheetBased" ? "Sprite Sheet File":"Image File";
    }else{
        typeText="--";
    }
    QLabel *typeField=new QLabel(typeText,this);
    typeField->setEnabled(false);
    vertLeft->addWidget(typeField,2,1,1,2);

    vertLeft->addWidget(new QLabel("Resource",this),3,0);
    QLineEdit *filePathField=new QLineEdit(this);
    if(areAllSpritesSame()){
        filePathField->setText(gi->path);
    }else{
        filePathField->setText("--");
    }
    filePathField->setReadOnly(true);
    vertLeft->addWidget(filePathField,3,1);
    QPushButton *selectButton=new QPushButton("Select");
    vertLeft->addWidget(selectButton,3,2);

    //Antialias checkbox
    vertLeft->addWidget(new QLabel("Antialias"),4,0);
    QCheckBox *antialiasField=new QCheckBox;
    vertLeft->addWidget(antialiasField,4,1,1,2);
    antialiasField->setChecked(gi->spriteAntialias);

    mainHorz->addLayout(vertLeft);
    QFrame *vFrame=new QFrame();
    mainHorz->addSpacing(1);
    mainHorz->addWidget(vFrame);
    mainHorz->addLayout(vertRight);

    mainLayout->addLayout(mainHorz);

    connect(selectButton,&QPushButton::clicked,[=]{
        QPoint targetPosition(this->mapToGlobal(selectButton->geometry().bottomRight()-QPoint(400,0)));
        QString targetPath=scene->mainWindow->projectFolderPath+scene->mainWindow->resourcesPath+"/"+gi->path;
        QString targetFolderPath=scene->mainWindow->projectFolderPath+scene->mainWindow->resourcesPath+"/Sprites";
        QString dialogResult=FileSelectorDialog::Sprite(targetFolderPath,targetPosition,targetPath,false,this);
        qDebug()<<dialogResult;
        if(dialogResult!=""){
            dialogResult=dialogResult.split(scene->mainWindow->resourcesPath+"/").at(1);
            GraphicsScene *scn=dynamic_cast<GraphicsScene*>(transformItem->scene);
            QUndoCommand *cmd=new SetMultipleItemSprite(scn,transformItem->items,dialogResult,true);
            scn->stack->push(cmd);
            filePathField->setText(dialogResult);
        }
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });


    connect(nameField,&QLineEdit::editingFinished,[=]{
        if(transformItem->items.count()==1){
            GameItem *ngi=dynamic_cast<GameItem*>(transformItem->items.at(0));
            ngi->name=nameField->text();
            ngi->hierarchyItem->setText(nameField->text());
        }
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });

    connect(tagField,&QLineEdit::editingFinished,[=]{
        for(int i=0;i<transformItem->items.length();i++){
            GameItem *ngi=dynamic_cast<GameItem*>(transformItem->items.at(i));
            ngi->tag=tagField->text();
        }
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });

    connect(antialiasField,&QCheckBox::toggled,[=](bool isChecked){
       for(int i=0;i<transformItem->items.length();i++){
           GameItem *cgi=dynamic_cast<GameItem*>(transformItem->items.at(i));
           cgi->spriteAntialias=isChecked;
       }
       scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });


}

//OBJECT PROP
void PropertiesItem::AddObjectProp()
{
    GameItem *gi=dynamic_cast<GameItem*>(transformItem->items.at(0));
    if(gi==NULL){
        return;
    }
    QList<QLineEdit*> inputFields;

    QHBoxLayout* mainHorz=new QHBoxLayout;
    mainHorz->setAlignment(Qt::AlignTop);

    QGridLayout *vertLeft=new QGridLayout;
    vertLeft->setAlignment(Qt::AlignTop);
    QVBoxLayout *vertRight=new QVBoxLayout;
    vertRight->setAlignment(Qt::AlignTop);

    //RIGHT SIDE

    QLabel *typeIcon=new QLabel;
    typeIcon->setAlignment(Qt::AlignCenter);
    QPixmap iconPixmap(":/Icons/object_type_32x32.png");
    typeIcon->setPixmap(iconPixmap);
    vertRight->addWidget(typeIcon);

    QLabel *typeTitle=new QLabel("Object",this);
    typeTitle->setAlignment(Qt::AlignCenter);
    vertRight->addWidget(typeTitle);

    //LEFT SIDE

    vertLeft->addWidget(new QLabel("Tag",this),0,0);
    QLineEdit *tagField=new QLineEdit;
    if(transformItem->items.count()==1 || areAllTagsSame()){
        tagField->setText(gi->tag);
    }else{
    }
    vertLeft->addWidget(tagField,0,1,1,2);


    nameField=new QLineEdit;
    if(transformItem->items.count()==1){
        nameField->setText(gi->name);
    }else{
        nameField->setEnabled(false);
    }
    vertLeft->addWidget(new QLabel("Name",this),1,0);
    vertLeft->addWidget(nameField,1,1,1,2);

    //Select Resource Class
    vertLeft->addWidget(new QLabel("Resource Class",this),2,0);
    QLineEdit *classPathField=new QLineEdit(this);
    if(areAllSpritesSame()){
        classPathField->setText(gi->objectClassPath);
    }else{
        classPathField->setText("--");
    }
    classPathField->setReadOnly(true);
    vertLeft->addWidget(classPathField,2,1);
    QPushButton *selectClassButton=new QPushButton("Select");
    vertLeft->addWidget(selectClassButton,2,2);

    //Select Symbolic Sprite
    vertLeft->addWidget(new QLabel("Symbolic Sprite",this),3,0);
    QLineEdit *spritePathField=new QLineEdit(this);
    if(areAllSpritesSame()){
        spritePathField->setText(gi->path);
    }else{
        spritePathField->setText("--");
    }
    spritePathField->setReadOnly(true);
    vertLeft->addWidget(spritePathField,3,1);
    QPushButton *selectSpriteButton=new QPushButton("Select");
    vertLeft->addWidget(selectSpriteButton,3,2);

    mainHorz->addLayout(vertLeft);
    QFrame *vFrame=new QFrame();
    mainHorz->addSpacing(1);
    mainHorz->addWidget(vFrame);
    mainHorz->addLayout(vertRight);

    mainLayout->addLayout(mainHorz);

    //if user clicked select class button
    connect(selectClassButton,&QPushButton::clicked,[=]{
        QPoint targetPosition(this->mapToGlobal(selectClassButton->geometry().bottomRight()-QPoint(400,0)));
        QString targetPath=gi->objectClassPath!="None" ? scene->mainWindow->projectFolderPath+scene->mainWindow->classesPath+"/"+gi->objectClassPath:"";
        QString targetFolderPath=scene->mainWindow->projectFolderPath+scene->mainWindow->classesPath;
        QString dialogResult=FileSelectorDialog::Class(targetFolderPath,targetPosition,targetPath,true,this);
        qDebug()<<dialogResult;
        if(dialogResult!=""){
            dialogResult=dialogResult=="DefaultNode" ? "":dialogResult;
            for(int i=0;i<transformItem->items.count();i++){
                QString objectType=dialogResult=="None" ? "DefaultObject":GameItem::getClassType(dialogResult);
                objectType=objectType=="" ? "DefaultObject":objectType;
                qDebug()<<objectType;
                if(dialogResult.indexOf(scene->mainWindow->classesPath)>0)
                    dialogResult=dialogResult.split(scene->mainWindow->classesPath+"/").at(1);
                GameItem *nGameItem=dynamic_cast<GameItem*>(transformItem->items.at(i));
                nGameItem->objectClassPath=dialogResult;
                nGameItem->objectType=objectType;
            }
            classPathField->setText(dialogResult);
        }
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });

    //if user clicked select sprite button
    connect(selectSpriteButton,&QPushButton::clicked,[=]{
        QPoint targetPosition(this->mapToGlobal(selectSpriteButton->geometry().bottomRight()-QPoint(400,0)));
        QString targetPath=scene->mainWindow->projectFolderPath+scene->mainWindow->resourcesPath+"/"+gi->path;
        QString targetFolderPath=scene->mainWindow->projectFolderPath+scene->mainWindow->resourcesPath+"/Sprites";
        QString dialogResult=FileSelectorDialog::Sprite(targetFolderPath,targetPosition,targetPath,true,this);
        qDebug()<<dialogResult;
        if(dialogResult!=""){
            if(dialogResult=="None"){
                dialogResult="";
            }else{
                dialogResult=dialogResult.split(scene->mainWindow->resourcesPath+"/").at(1);
            }
            GraphicsScene *scn=dynamic_cast<GraphicsScene*>(transformItem->scene);
            QUndoCommand *cmd=new SetMultipleItemSprite(scn,transformItem->items,dialogResult,true);
            scn->stack->push(cmd);
            spritePathField->setText(dialogResult);
        }
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });


    connect(nameField,&QLineEdit::editingFinished,[=]{
        if(transformItem->items.count()==1){
            GameItem *gi=dynamic_cast<GameItem*>(transformItem->items.at(0));
             gi->name=nameField->text();
            gi->hierarchyItem->setText(nameField->text());
        }
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });

    connect(tagField,&QLineEdit::editingFinished,[=]{
        for(int i=0;i<transformItem->items.length();i++){
            GameItem *ngi=dynamic_cast<GameItem*>(transformItem->items.at(i));
            ngi->tag=tagField->text();
        }
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });


}
//MULTIPLE PROP
void PropertiesItem::AddMultipleProp()
{
    GameItem *gi=dynamic_cast<GameItem*>(transformItem->items.at(0));
    if(gi==NULL){
        return;
    }
    QList<QLineEdit*> inputFields;

    QHBoxLayout* mainHorz=new QHBoxLayout;
    mainHorz->setAlignment(Qt::AlignTop);

    QGridLayout *vertLeft=new QGridLayout;
    vertLeft->setAlignment(Qt::AlignTop);
    QVBoxLayout *vertRight=new QVBoxLayout;
    vertRight->setAlignment(Qt::AlignTop);

    //RIGHT SIDE

    QLabel *typeIcon=new QLabel;
    typeIcon->setAlignment(Qt::AlignCenter);
    QPixmap iconPixmap(":/Icons/multipleitems_type_32x32.png");
    typeIcon->setPixmap(iconPixmap);
    vertRight->addWidget(typeIcon);

    QLabel *typeTitle=new QLabel("Multiple\nTypes",this);
    typeTitle->setWordWrap(true);
    typeTitle->setAlignment(Qt::AlignCenter);
    vertRight->addWidget(typeTitle);

    //LEFT SIDE
    vertLeft->addWidget(new QLabel("Tag",this),0,0);
    QLineEdit *tagField=new QLineEdit;
    if(transformItem->items.count()==1 || areAllTagsSame()){
        tagField->setText(gi->tag);
    }else{
    }
    vertLeft->addWidget(tagField,0,1,1,2);

    nameField=new QLineEdit;
    nameField->setEnabled(false);
    nameField->setText("...");
    vertLeft->addWidget(new QLabel("Name",this),1,0);
    vertLeft->addWidget(nameField,1,1);

    mainHorz->addLayout(vertLeft);
    QFrame *vFrame=new QFrame();
    mainHorz->addSpacing(1);
    mainHorz->addWidget(vFrame);
    mainHorz->addLayout(vertRight);

    mainLayout->addLayout(mainHorz);

    connect(tagField,&QLineEdit::editingFinished,[=]{
        for(int i=0;i<transformItem->items.length();i++){
            GameItem *ngi=dynamic_cast<GameItem*>(transformItem->items.at(i));
            ngi->tag=tagField->text();
        }
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });
}

//PHYSICS PROP
void PropertiesItem::AddPhysicsProp()
{
    QGroupBox* groupBox=new QGroupBox("Physic Body");
    GameItem *gi=dynamic_cast<GameItem*>(transformItem->items.at(0));
    GraphicsScene *scene=dynamic_cast<GraphicsScene*>(transformItem->scene);
    if(gi==NULL){
        return;
    }
    QList<QLineEdit*> inputFields;

    QVBoxLayout *physicsMainLayout=new QVBoxLayout;

    //Enable-Disable physics
    QCheckBox *physicsCheckbox=new QCheckBox("Enable",this);
    if(gi->enablePyhsics==true){
        physicsCheckbox->setChecked(true);
    }
    physicsMainLayout->addWidget(physicsCheckbox);
    connect(physicsCheckbox,&QCheckBox::toggled,[=](bool checked){
        for(int i=0;i<transformItem->items.count();i++){
            GameItem * cgi=dynamic_cast<GameItem*>(transformItem->items.at(i));
            cgi->enablePyhsics=checked;
        }
         scene->views().at(0)->update();
         scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });

    QGridLayout *tableLayout=new QGridLayout;

    //Type and mask lines
    tableLayout->addWidget(new QLabel("Body Type"),1,0);
    QComboBox *typeComboBox=new QComboBox(this);
    typeComboBox->addItem("Static",QVariant("static"));
    typeComboBox->addItem("Dynamic",QVariant("dynamic"));
    if(gi->dynamic==true){
        typeComboBox->setCurrentText("Dynamic");
    }else{
        typeComboBox->setCurrentText("Static");
    }
    tableLayout->addWidget(typeComboBox,1,1,1,2);

    connect(typeComboBox,static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),[=](QString str){
        qDebug()<<str;
        for(int i=0;i<transformItem->items.count();i++){
            GameItem * cgi=dynamic_cast<GameItem*>(transformItem->items.at(i));
            if(str=="Dynamic"){
                cgi->dynamic=true;
            }else{
                cgi->dynamic=false;
            }
        }
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings

    });

    tableLayout->addWidget(new QLabel("Mass"),1,3);
    QDoubleSpinBox *massField=new QDoubleSpinBox();
    massField->setRange(0,9999999);
    massField->setSingleStep(0.10f);
    massField->setValue(gi->mass);
    tableLayout->addWidget(massField,1,4,1,2);

    connect(massField,&QDoubleSpinBox::editingFinished,[=]{
        for(int i=0;i<transformItem->items.count();i++){
            GameItem * cgi=dynamic_cast<GameItem*>(transformItem->items.at(i));
            cgi->mass=massField->value();
        }
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });


    physicsMainLayout->addLayout(tableLayout);

    //Physics Body Properties
    QLabel* densityLabel=new QLabel("Density");
    densityLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QLabel* frictionLabel=new QLabel("Friction");
    frictionLabel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    QLabel* restitutionLabel=new QLabel("Restitution");
    restitutionLabel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);

    tableLayout->addWidget(densityLabel,2,0); //Density
    QDoubleSpinBox *densityField=new QDoubleSpinBox();
    densityField->setRange(0,1);
    densityField->setSingleStep(0.10f);
    densityField->setValue(gi->density);
    tableLayout->addWidget(densityField,2,1);
    tableLayout->addWidget(frictionLabel,2,2); //Friction
    QDoubleSpinBox *frictionField=new QDoubleSpinBox();
    frictionField->setRange(0,1);
    frictionField->setSingleStep(0.10f);
    frictionField->setValue(gi->friction);
    tableLayout->addWidget(frictionField,2,3);
    tableLayout->addWidget(restitutionLabel,2,4); //Restitution
    QDoubleSpinBox *restitutionField=new QDoubleSpinBox();
    restitutionField->setRange(0,1);
    restitutionField->setSingleStep(0.10f);
    restitutionField->setValue(gi->restitution);
    tableLayout->addWidget(restitutionField,2,5);

    connect(densityField,&QDoubleSpinBox::editingFinished,[=]{
        for(int i=0;i<transformItem->items.count();i++){
            GameItem * cgi=dynamic_cast<GameItem*>(transformItem->items.at(i));
            cgi->density=densityField->value();
        }
        qDebug()<<gi->density;
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });
    connect(frictionField,&QDoubleSpinBox::editingFinished,[=]{
        for(int i=0;i<transformItem->items.count();i++){
            GameItem * cgi=dynamic_cast<GameItem*>(transformItem->items.at(i));
            cgi->friction=frictionField->value();
        }

        qDebug()<<gi->friction;
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });
    connect(restitutionField,&QDoubleSpinBox::editingFinished,[=]{
        for(int i=0;i<transformItem->items.count();i++){
            GameItem * cgi=dynamic_cast<GameItem*>(transformItem->items.at(i));
            cgi->restitution=restitutionField->value();
        }
        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings
    });

    if(transformItem->items.count()>1){
        groupBox->setLayout(physicsMainLayout);
        mainLayout->addWidget(groupBox);
        return;
    }

    //Physics Shapes
    QGroupBox* shapesGroup=new QGroupBox("Collider Shapes");
    QVBoxLayout *shapesGroupLayout=new QVBoxLayout;
    shapesGroup->setLayout(shapesGroupLayout);
    physicsMainLayout->addWidget(shapesGroup);
    shapesGroupLayout->setMargin(6);
    shapesGroupLayout->setSpacing(0);

    //Physics Shape Add Button
    QHBoxLayout *shapeToolsLayout=new QHBoxLayout;
    shapeToolsLayout->setAlignment(Qt::AlignRight);
    QPushButton *addShapeButton=new QPushButton(QIcon(QPixmap(":/Icons/uiIcons_mini_addbutton.png")),"Add");
    addShapeButton->setMinimumWidth(100);
    QMenu *addShapeMenu=new QMenu(this);
    addShapeMenu->addAction(QIcon(QPixmap(":/Icons/uiIcons_mini_colliderbox.png")),"Box Collider");
    addShapeMenu->addAction(QIcon(QPixmap(":/Icons/uiIcons_mini_collidercircle.png")),"Circle Collider");
    addShapeButton->setMenu(addShapeMenu);
    shapeToolsLayout->addWidget(addShapeButton);
    shapesGroupLayout->addLayout(shapeToolsLayout);

    connect(addShapeMenu,&QMenu::triggered,[=](QAction *action){
        QSizeF defaultSize=gi->originalSize;
        if(action->text()=="Box Collider"){
            qDebug()<<"Box";
            QVariantMap *boxCollider=new QVariantMap;
            boxCollider->insert("type",QVariant("box"));
            boxCollider->insert("rect",QVariant(QRectF(0,0,defaultSize.width(),defaultSize.height())) );
            gi->colliderShapes.append(boxCollider);
        }else if(action->text()=="Circle Collider"){
            qDebug()<<"Circle";
            QVariantMap *circleCollider=new QVariantMap;
            circleCollider->insert("type",QVariant("circle"));
            circleCollider->insert("rect",QVariant(QRectF(0,0,defaultSize.width(),defaultSize.width())) );
            gi->colliderShapes.append(circleCollider);
        }
        int lastIndex=gi->colliderShapes.count()-1;
        ColliderShapeItem *nShapeItem=new ColliderShapeItem(this,gi,gi->colliderShapes.at(lastIndex));
        shapesGroupLayout->addWidget(nShapeItem);
        colliderShapeInspectors.append(nShapeItem);
        scene->views().at(0)->update();

        scene->sceneChanged(); //Calling sceneChanged() function for save operations and warnings

    });

    //Adding Shapes
    for(int i=0;i<gi->colliderShapes.count();i++){
        ColliderShapeItem *shapeItem=new ColliderShapeItem(this,gi,gi->colliderShapes.at(i));
        shapesGroupLayout->addWidget(shapeItem);
        colliderShapeInspectors.append(shapeItem);
    }

    groupBox->setLayout(physicsMainLayout);
    mainLayout->addWidget(groupBox);

    /*
    connect(editButton,&QPushButton::clicked,[=]{
        GraphicsScene *scn=dynamic_cast<GraphicsScene*>(transformItem->scene);
        scn->editMode=SceneEditModes::ColliderEdit;
        transformItem->leftBottomAnchor->update(transformItem->leftBottomAnchor->boundingRect());
    });
    */
}

void PropertiesItem::resetPhysicsEditButtons(QPushButton *except)
{
    for(int i=0;i<colliderShapeInspectors.count();i++){
        ColliderShapeItem *cItem=colliderShapeInspectors.at(i);
        if(except!=NULL){
            if(except==cItem->editButton){
                qDebug()<<cItem->editButton;
                continue;
            }
        }
        cItem->uncheckEditButton(true);
    }
}

//UPDATE FUNCTIONS
void PropertiesItem::UpdateTransformProp()
{
    if(transformItem->items.count()==1){
        GameItem *gi=dynamic_cast<GameItem*>(transformItem->items.at(0));
        if(gi!=NULL){
            rotation_ui->setText(QString::number(gi->rotate));
            scale_ui->setText(QString::number(gi->scale.width()));
            posX_ui->setText(QString::number(transformItem->scenePos().x()));
            posY_ui->setText(QString::number(-transformItem->scenePos().y()));
            zOrder_ui->setValue(gi->zValue());
        }
    }else{
        QString positionX=areAllPositionsSame(true,false)==true ? QString::number(transformItem->items.at(0)->scenePos().x()) : "--";
        //Crossing y input field to -1. Because y position reversed in qt coordinate system
        QString positionY=areAllPositionsSame(false,true)==true ? QString::number(-transformItem->items.at(0)->scenePos().y()) : "--";
        posX_ui->setText(positionX);
        posY_ui->setText(positionY);
        rotation_ui->setText(QString::number(transformItem->deltaRotation));
        scale_ui->setText(QString::number(transformItem->transform().m11()));
    }
}
// UPDATE TRANSFORMS OF GAME ITEMS
void PropertiesItem::UpdateGameItemPositionX()
{

    //Getting scene as GraphicsScene Object (Our Custom Scene Class)
    GraphicsScene *scn=dynamic_cast<GraphicsScene*>(transformItem->scene);
    //Checking if input value is numeric
    bool isNumeric=false;
    float newPosX=posX_ui->text().toFloat(&isNumeric);
    if(isNumeric!=true){
        UpdateTransformProp();
        return;
    }


    if(scn!=NULL){
        QUndoCommand *cmd;
        if(transformItem->items.count()==1){
            if(transformItem->scenePos().x()==newPosX){
                return;
            }
            cmd=new MoveItemCommand(scn,transformItem->items,transformItem->scenePos(),QPointF(newPosX,transformItem->y()));
            scn->stack->push(cmd);
            transformItem->setPos(QPointF(newPosX,transformItem->y()));
            transformItem->UpdateContainer();
        }else{
            cmd=new SetMultipleItemPosX(scn,transformItem->items,newPosX);
            scn->stack->push(cmd);
        }
    }
}

void PropertiesItem::UpdateGameItemPositionY()
{
    //Getting scene as GraphicsScene Object (Our Custom Scene Class)
    GraphicsScene *scn=dynamic_cast<GraphicsScene*>(transformItem->scene);
    //Checking if input value is numeric
    bool isNumeric=false;
    float newPosY=posY_ui->text().toFloat(&isNumeric);
    if(isNumeric!=true){
        UpdateTransformProp();
        return;
    }
    newPosY=-newPosY;

    if(scn!=NULL){
        QUndoCommand *cmd;
        if(transformItem->items.count()==1){
            if(transformItem->scenePos().y()==newPosY){
                return;
            }
            cmd=new MoveItemCommand(scn,transformItem->items,transformItem->scenePos(),QPointF(transformItem->x(),newPosY));
            scn->stack->push(cmd);
            transformItem->setPos(QPointF(transformItem->x(),newPosY));
            transformItem->UpdateContainer();
        }else{
            cmd=new SetMultipleItemPosY(scn,transformItem->items,newPosY);
            scn->stack->push(cmd);
        }
    }
}

void PropertiesItem::UpdateGameItemScales()
{
    //Getting scene as GraphicsScene Object (Our Custom Scene Class)
    GraphicsScene *scn=dynamic_cast<GraphicsScene*>(transformItem->scene);
    QSizeF startScale(transformItem->transform().m11(),transformItem->transform().m22());
    QSizeF finalScale(scale_ui->text().toFloat(),scale_ui->text().toFloat());
    if(finalScale==QSizeF(0,0)){
        finalScale=QSizeF(0.01,0.01);
    }
    //Getting QGraphicsItem as a gameItem object
    GameItem *gi=NULL;
    if(transformItem->items.count()==1){
        gi=dynamic_cast<GameItem*>(transformItem->items.at(0));
    }
    //If single gameItem selected
    if(gi){
        float gw=gi->scale.width();
        float gh=gi->scale.height();
        finalScale=QSizeF((finalScale.width()*startScale.width())/gw,(finalScale.height()*startScale.height())/gh );
    }
    QUndoCommand *cmd=new ScaleItemCommand(scn,transformItem->items,startScale,finalScale);
    scn->stack->push(cmd);
    transformItem->setNewScale(QPointF(finalScale.width(),finalScale.height()),false);
}

void PropertiesItem::UpdateGameItemRotations()
{
    GraphicsScene *scn=dynamic_cast<GraphicsScene*>(transformItem->scene);
    GameItem *gi=NULL;
    if(transformItem->items.count()==1){
        gi=dynamic_cast<GameItem*>(transformItem->items.at(0));
        if(gi->rotate==rotation_ui->text().toFloat()){
            return;
        }
    }
    float deltaAngle=rotation_ui->text().toFloat();
    if(gi!=NULL){
        deltaAngle=deltaAngle-gi->rotation()-gi->tempRotate;
    }
    deltaAngle-=transformItem->lastRotation;
    transformItem->deltaRotation=0;
    QUndoCommand *cmd=new RotateItemCommand(scn,transformItem->items,deltaAngle);
    scn->stack->push(cmd);
    transformItem->rotateTransform(deltaAngle);
}

void PropertiesItem::UpdateGameItemZOrders()
{
    GraphicsScene *scn=dynamic_cast<GraphicsScene*>(transformItem->scene);
    GameItem *gi=NULL;
    for(int i=0;i<transformItem->items.count();i++){
        gi=dynamic_cast<GameItem*>(transformItem->items.at(i));
        if(gi!=NULL){
            gi->setZValue(zOrder_ui->value());
        }
    }
    transformItem->UpdateContainerZOrder();
}

//CHECKING FUNCTIONS
bool PropertiesItem::areAllPositionsSame(bool checkXPos, bool checkYPos)
{
    QPointF tempPos(transformItem->items.at(0)->scenePos());
    for(int i=0;i<transformItem->items.count();i++){
        QPointF itemPos=transformItem->items.at(i)->scenePos();
        bool condination1=itemPos.x()!=tempPos.x();
        bool condination2=itemPos.y()!=tempPos.y();
        if(checkXPos==true){
            if(condination1)
                return false;
        }

        if(checkYPos==true){
            if(condination2)
                return false;
        }
    }
    return true;
}

bool PropertiesItem::areAllSpritesSame()
{
    GameItem *gi=dynamic_cast<GameItem*>(transformItem->items.at(0));
    QString tempPath=gi->path;
    for(int i=0;i<transformItem->items.count();i++){
        GameItem *gi=dynamic_cast<GameItem*>(transformItem->items.at(i));
        if(tempPath!=gi->path){
            return false;
        }
    }
    return true;
}

bool PropertiesItem::areAllItemsSprite()
{
    for(int i=0;i<transformItem->items.count();i++){
        GameItem *gi=dynamic_cast<GameItem*>(transformItem->items.at(i));
        if(gi->itemType!=GameItemTypes::Sprite){
            return false;
        }
    }
    return true;
}

bool PropertiesItem::areAllItemsObject()
{
    for(int i=0;i<transformItem->items.count();i++){
        GameItem *gi=dynamic_cast<GameItem*>(transformItem->items.at(i));
        if(gi->itemType!=GameItemTypes::Object){
            return false;
        }
    }
    return true;
}

bool PropertiesItem::areAllTagsSame()
{
    GameItem *fgi=dynamic_cast<GameItem*>(transformItem->items.at(0));
    QString sampleTag=fgi->tag;
    for(int i=0;i<transformItem->items.count();i++){
        GameItem *gi=dynamic_cast<GameItem*>(transformItem->items.at(i));
        if(gi->tag!=sampleTag){
            return false;
        }
    }
    return true;
}


