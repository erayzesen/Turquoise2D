#include "newprojectdialog.h"

NewProjectDialog::NewProjectDialog(Turquoise2DEditor *parent)
{
    mainWindow=parent;

    contentsWidget = new QListWidget;
    contentsWidget->setIconSize(QSize(32,32));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(150);
    contentsWidget->setSpacing(4);

    pagesWidget = new QStackedWidget;
    cocos2dxContent=Cocos2dxContent();
    pagesWidget->addWidget(cocos2dxContent);

    //Defining Framework Options
    QListWidgetItem *cocos2dxOption=new QListWidgetItem(contentsWidget);
    cocos2dxOption->setIcon(QIcon(":/Icons/fwIcons_cocos2dx_128x128.png"));
    cocos2dxOption->setText("Cocos2d-x");
    cocos2dxOption->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    contentsWidget->setCurrentRow(0);


    QHBoxLayout *horizontalLayout=new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget,1);



    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);


    this->setLayout(mainLayout);
    this->setFixedSize(640,400);
    this->setWindowTitle("Create New Project");

}

QWidget *NewProjectDialog::Cocos2dxContent()
{
    QWidget *result=new QWidget;
    QVBoxLayout *contentLayout=new QVBoxLayout;
    contentLayout->setMargin(4);

    QGridLayout *gridLayout=new QGridLayout;

    //Project Name
    gridLayout->addWidget(new QLabel("Project Name"),0,0);
    QLineEdit *nameField=new QLineEdit();
    nameField->setText("NewCocosProject");
    gridLayout->addWidget(nameField,0,1,1,2);

    //Project Path
    QSettings *settings=new QSettings("Turquoise2D","Turquoise2D Editor");
    QString targetLocation=settings->value("lastCreatedProjectDir",QDir().homePath()).toString();
    settings->setValue("lastCreatedProjectDir",targetLocation);
    gridLayout->addWidget(new QLabel("Project Path"),1,0);
    QLineEdit *projectPathField=new QLineEdit;
    projectPathField->setReadOnly(true);
    projectPathField->setText(targetLocation);
    gridLayout->addWidget(projectPathField,1,1);
    QPushButton *projectPathButton=new QPushButton("Select");
    gridLayout->addWidget(projectPathButton,1,2);

    connect(projectPathButton,&QPushButton::clicked,[=]{
        QString dir=QFileDialog::getExistingDirectory(this, tr("Select Project Path"),targetLocation,QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(dir!=""){
            projectPathField->setText(dir);
            settings->setValue("lastCreatedProjectDir",dir);
        }
    });

    //Framework Version
    gridLayout->addWidget(new QLabel("Framework Version"),2,0);
    QComboBox *frameworkComboBox=new QComboBox;
    QStringList frameworkOptions=ProjectFilesOperations::getFrameworkPaths(0,"Cocos2d-x");
    for(int i=0;i<frameworkOptions.count();i++){
        frameworkComboBox->addItem(QFileInfo(frameworkOptions.at(i)).fileName(),frameworkOptions.at(i));
    }
    gridLayout->addWidget(frameworkComboBox,2,1);

    //Portrait and Landscape Options
    gridLayout->addWidget(new QLabel("Portrait"),3,0);
    QCheckBox *portraitField=new QCheckBox;
    gridLayout->addWidget(portraitField,3,1);

    contentLayout->addLayout(gridLayout);


    QHBoxLayout *buttonsLayout=new QHBoxLayout;
    QDialogButtonBox *buttonBox=new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    buttonBox->button(QDialogButtonBox::Ok)->setText("Create");
    contentLayout->addStretch();
    contentLayout->addWidget(buttonBox);

    connect(buttonBox,&QDialogButtonBox::clicked,[=](QAbstractButton *button){
       if(button==buttonBox->button(QDialogButtonBox::Ok)){
           if(checkForUniqueTargetPath(projectPathField->text()+"/"+nameField->text())==false){
               QString errMessage="\""+nameField->text()+"\" name already exist in the path, please change the name or choose another path.";
               QMessageBox::warning(mainWindow,"Warning!",errMessage);
           }else{
               //Creating project
               ProjectConsole::NewProject(mainWindow,"Cocos2d-x",frameworkComboBox->currentData().toString(),nameField->text(),projectPathField->text(),portraitField->isChecked());
               this->close();
           }
       }else if(button==buttonBox->button(QDialogButtonBox::Cancel)){
           this->close();
       }
    });
    connect(nameField,&QLineEdit::textEdited,[=](QString text){
        if(text.length()<2){
            buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }else{
            buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        }
    });

    result->setLayout(contentLayout);

    return result;

}

bool NewProjectDialog::checkForUniqueTargetPath(QString targetPath)
{
    if(QDir(targetPath).exists()){
        return false;
    }
    return true;
}

