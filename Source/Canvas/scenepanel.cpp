#include "scenepanel.h"

ScenePanel::ScenePanel(QString _sceneFilePath,PropertiesPanel *_propPanel,Turquoise2DEditor *_mainWindow,QWidget *parent) : QWidget(parent)
{
    propPanel=_propPanel;
    mainWindow=_mainWindow;
    sceneFilePath=_sceneFilePath;
    sceneFileName=QFileInfo(sceneFilePath).fileName();

    QVBoxLayout *mainLayout=new QVBoxLayout(this);
    mainLayout->setMargin(0);

    GraphicsView *nView=new GraphicsView(mainWindow);

    //Adding scene
    scene=new GraphicsScene(sceneFilePath,propPanel,this);
    nView->setScene(scene);
    scene->view=nView;
    mainLayout->addWidget(nView);

    toolsLayout=new QHBoxLayout();
    toolsLayout->setMargin(0);
    toolsLayout->setSpacing(4);
    toolsLayout->setAlignment(Qt::AlignRight);

    drawToolsLayout=new QHBoxLayout();
    drawToolsLayout->setMargin(0);
    drawToolsLayout->setSpacing(4);

    toolsLayout->addLayout(drawToolsLayout);
    toolsLayout->addStretch();

    QHBoxLayout *snapButtonLayout=new QHBoxLayout();
    snapButtonLayout->setMargin(0);
    snapButtonLayout->setSpacing(0);

    //Snap button
    snapButton=new QPushButton(QIcon(QPixmap(":/Icons/uiIcons_mini_snapbutton.png")),"");
    snapButton->setCheckable(true);
    if(scene->isSnapMode==true)snapButton->setChecked(true);
    snapButtonLayout->addWidget(snapButton);

    //Snap Options
    QPushButton *snapOptionsButton=new QPushButton();
    snapOptionsButton->setMaximumWidth(20);
    QMenu *snapMenu=new QMenu(this);
    QActionGroup *snapStyleGroup=new QActionGroup(snapMenu);
    QAction *snapCenterAction=new QAction("with Item Center",this);
    snapCenterAction->setCheckable(true);
    QAction *snapBoundsAction=new QAction("with Item Bounds",this);
    snapBoundsAction->setCheckable(true);
    if(scene->snapStyle==SnapStyle::withCenter)snapCenterAction->setChecked(true);
    if(scene->snapStyle==SnapStyle::withBounds)snapBoundsAction->setChecked(true);
    snapStyleGroup->addAction(snapCenterAction);
    snapStyleGroup->addAction(snapBoundsAction);
    snapMenu->addActions(snapStyleGroup->actions());


    snapOptionsButton->setMenu(snapMenu);
    snapButtonLayout->addWidget(snapOptionsButton);

    //Adding snap tool layout
    toolsLayout->addLayout(snapButtonLayout);

    QPushButton *gridButton=new QPushButton(QIcon(QPixmap(":/Icons/uiIcons_mini_gridbutton.png")),"");
    toolsLayout->addWidget(gridButton);
    mainLayout->addLayout(toolsLayout);

    //Snap button event
    connect(snapButton,&QPushButton::clicked,[=]{
        scene->isSnapMode=snapButton->isChecked();
    });

    //Snap options event
    connect(snapStyleGroup,&QActionGroup::triggered,[=]{
        QString checkedOption=snapStyleGroup->checkedAction()->text();
       if(checkedOption=="with Item Center"){
           scene->snapStyle=SnapStyle::withCenter;
       }else if(checkedOption=="with Item Bounds"){
           scene->snapStyle=SnapStyle::withBounds;
       }
    });


    //Grid button event
    connect(gridButton,&QPushButton::released,[=]{
        // if clicked grid button
        QVariantMap *gridDialog=createGridOptions();
        if(gridDialog!=NULL){
                scene->gridSize=gridDialog->value("size").toSizeF();
                scene->view->update();
                sceneChanged();
        }
    });
}

ScenePanel::~ScenePanel()
{
    delete scene;
}

QVariantMap *ScenePanel::createGridOptions()
{
    QVariantMap *result=new QVariantMap();
    QDialog *dialog=new QDialog(this);
    QVBoxLayout *dialogLayout=new QVBoxLayout();
    dialog->setLayout(dialogLayout);
    dialog->resize(QSize(300,100));
    dialog->setWindowTitle("Grid Config");

    QGridLayout *fieldsLayout=new QGridLayout();
    //width
    fieldsLayout->addWidget(new QLabel("Width:"),0,0);
    QDoubleSpinBox *widthField=new QDoubleSpinBox();
    widthField->setRange(1,99999999);
    widthField->setValue(scene->gridSize.width());
    fieldsLayout->addWidget(widthField,0,1);
    //height
    fieldsLayout->addWidget(new QLabel("Height:"),0,2);
    QDoubleSpinBox *heightField=new QDoubleSpinBox();
    heightField->setRange(1,99999999);
    heightField->setValue(scene->gridSize.height());
    fieldsLayout->addWidget(heightField,0,3);

    dialogLayout->addLayout(fieldsLayout);

    QDialogButtonBox *buttons=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    dialogLayout->addWidget(buttons);

    connect(buttons,&QDialogButtonBox::accepted,[=]{
        dialog->accept();
    });
    connect(buttons,&QDialogButtonBox::rejected,[=]{
        dialog->reject();
    });
    if(dialog->exec()){
        result->insert("size",QVariant(QSizeF(widthField->value(),heightField->value())));
    }else{
        result=NULL;
    }

    return result;

}

void ScenePanel::clearToolsLayout()
{
    while (QLayoutItem *item=drawToolsLayout->takeAt(0)) {
        if(QWidget *widget=item->widget()){
            delete widget;
        }
        delete item;
    }
    //delete objects
    if(eyedropperButton!=NULL)eyedropperButton=NULL;
    if(eyedropperViewer!=NULL)eyedropperViewer=NULL;
    if(eyedropperNameField!=NULL)eyedropperNameField=NULL;
    if(percent100!=NULL)percent100=NULL;
    if(zoomResult!=NULL)zoomResult=NULL;
}

void ScenePanel::addDrawTools()
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::VLine);

    clearToolsLayout();

    //Draw Collision Types
    QLabel * collisionDedectionTitle=new QLabel("Collision Dedection For:");
    QComboBox *collisionTypes=new QComboBox(this);
    collisionTypes->setProperty("mediumSizeComboBox",true);
    collisionTypes->addItem("None",QVariant("none"));
    collisionTypes->addItem("All of Items",QVariant("allOfItems"));
    collisionTypes->addItem("Similar Items",QVariant("similarItems"));
    if(scene->drawCollisionDedection==DrawCollisionDedection::DedectionForAllItems){
        collisionTypes->setCurrentIndex(collisionTypes->findData("allOfItems"));
    }else if(scene->drawCollisionDedection==DrawCollisionDedection::DedectionForSimilarItems){
        collisionTypes->setCurrentIndex(collisionTypes->findData("similarItems"));
    }else{
        collisionTypes->setCurrentIndex(collisionTypes->findData("none"));
    }
    drawToolsLayout->addWidget(collisionDedectionTitle);
    drawToolsLayout->addWidget(collisionTypes);

    drawToolsLayout->addWidget(line);

    //Eyedropper button
    if(eyedropperButton){
       delete eyedropperButton;
    }
    eyedropperButton=new QPushButton(QIcon(QPixmap(":/Icons/toolicon_eyedropper.png")),"");
    eyedropperButton->setCheckable(true);
    eyedropperButton->setToolTip("Eyedropper Tool (Alt)");
    if(scene->editMode==SceneEditModes::EyedropperEdit){
        eyedropperButton->setChecked(true);
    }
    drawToolsLayout->addWidget(eyedropperButton);

    //EyedroppedViewer
    eyedropperViewer=new QLabel;
    eyedropperNameField=new QLabel;
    eyedropperNameField->setEnabled(false);

    eyedropperViewer->setAutoFillBackground(true);
    eyedropperViewer->setMinimumSize(24,24);
    eyedropperViewer->setAlignment(Qt::AlignCenter);
    eyedropperViewer->setBackgroundRole(QPalette::Light);
    eyedropperViewer->setStyleSheet("background:#262626;border: 1px inset #3e3e3e;border-radius:0px;");
    if(scene->eyedroppedItemJson.isEmpty()==false){
        QString sPath=scene->eyedroppedItemJson.value("spritePath").toString();
        QString sName=scene->eyedroppedItemJson.value("name").toString();
        if(sPath==""){
            eyedropperViewer->setPixmap(QPixmap(":/Icons/uiIcons_mini_object.png"));
        }else{
            QPixmap vPixmap(mainWindow->projectFolderPath+mainWindow->resourcesPath+"/"+sPath);
            vPixmap=vPixmap.scaled(QSize(16,16));
            eyedropperViewer->setPixmap(vPixmap);
        }
        eyedropperNameField->setText(sName);
    }else{
        eyedropperViewer->setPixmap(QPixmap(":/Icons/uiIcons_mini_none.png"));
        eyedropperNameField->setText("None");
    }
    drawToolsLayout->addWidget(eyedropperViewer);
    drawToolsLayout->addWidget(eyedropperNameField);


    //Buraya combobox ve collision type seçim özellikleri gelecek!


    connect(eyedropperButton,&QPushButton::toggled,[=](bool checked){
        if(checked==true){
            if(scene->editMode!=SceneEditModes::EyedropperEdit){
                scene->setEditMode(SceneEditModes::EyedropperEdit);
            }
        }else{
            if(scene->editMode!=SceneEditModes::DrawEdit){
                scene->setEditMode(SceneEditModes::DrawEdit);
            }
        }
    });
    connect(collisionTypes,&QComboBox::currentTextChanged,[=](QString selectedStr){
        qDebug()<<selectedStr;
       if(selectedStr=="All of Items"){
           scene->drawCollisionDedection=DrawCollisionDedection::DedectionForAllItems;
       }else if(selectedStr=="Similar Items"){
           scene->drawCollisionDedection=DrawCollisionDedection::DedectionForSimilarItems;
       }else{
           scene->drawCollisionDedection=DrawCollisionDedection::None;
       }
    });
}

void ScenePanel::addZoomTools()
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::VLine);

    clearToolsLayout();

    percent100=new QPushButton("100%");
    percent100->setToolTip("Set zoom level to 100% (Ctrl+1)");
    drawToolsLayout->addWidget(percent100);

    drawToolsLayout->addWidget(line);

    QLabel *zoomTitle=new QLabel;
    zoomTitle->setText("Zoom Level:");
    zoomTitle->setEnabled(false);
    drawToolsLayout->addWidget(zoomTitle);

    zoomResult=new QLabel;
    float percent=qFloor(scene->view->transform().m11()*100);
    qDebug()<<percent;
    zoomResult->setText(QString::number(percent)+"%");
    drawToolsLayout->addWidget(zoomResult);

    connect(percent100,&QPushButton::released,[=]{
        scene->resetZoomLevel();
    });
}

void ScenePanel::sceneChanged()
{
    isSceneChanged=true;
    mainWindow->renameScaneTab(this);
}

