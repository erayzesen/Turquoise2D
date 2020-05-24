#include "projectlistitem.h"

ProjectListItem::ProjectListItem(Turquoise2DEditor *_mainWindow,QString frameworkName,QString projectName,QString projectPath,QWidget *parent) : QWidget(parent)
{
    mainWindow=_mainWindow;
    QHBoxLayout *mainLayout=new QHBoxLayout;
    mainLayout->setMargin(5);
    this->setLayout(mainLayout);

    QLabel *frameworkIcon=new QLabel;
    frameworkIcon->setPixmap(QPixmap(":/Icons/fwIcons_cocos2dx_128x128.png").scaled(48,48));
    mainLayout->addWidget(frameworkIcon);

    QVBoxLayout *infoLayout=new QVBoxLayout;
    QLabel *nameField=new QLabel(projectName);
    nameField->setStyleSheet("font-size:16px;");
    infoLayout->addWidget(nameField);
    QLabel *pathField=new QLabel(projectPath);
    infoLayout->addWidget(pathField);

    mainLayout->addLayout(infoLayout);

    mainLayout->addStretch();

    QHBoxLayout *buttonsLayout=new QHBoxLayout;
    QPushButton *exploreButton=new QPushButton("Explore");
    buttonsLayout->addWidget(exploreButton);
    QPushButton *openProjectButton=new QPushButton("Open Project");
    buttonsLayout->addWidget(openProjectButton);

    mainLayout->addLayout(buttonsLayout);

    connect(exploreButton,&QPushButton::clicked,[=]{
        QDesktopServices::openUrl(QUrl("file:///"+projectPath));
    });

    connect(openProjectButton,&QPushButton::clicked,[=]{
        mainWindow->OpenProject(projectPath);
    });
}

