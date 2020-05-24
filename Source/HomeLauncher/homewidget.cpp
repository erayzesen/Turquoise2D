#include "homewidget.h"

HomeWidget::HomeWidget(Turquoise2DEditor *_mainWindow,QWidget *parent) : QWidget(parent)
{
    mainWindow=_mainWindow;
    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->setMargin(3);

    QHBoxLayout *headerLayout=new QHBoxLayout;
    headerLayout->setAlignment(Qt::AlignTop);
    headerLayout->setMargin(0);

    QWidget *logoArea=new QWidget();
    logoArea->setProperty("home",true);
    QVBoxLayout *logoLayout=new QVBoxLayout;
    logoLayout->setSpacing(0);
    logoLayout->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    logoArea->setStyleSheet("QWidget{background-image: url(:/Icons/home_logo_291x71.png);background-position: top left;} QLabel{background:none}");
    QLabel *vInfo=new QLabel("ver 0.01");
    vInfo->setMargin(0);
    vInfo->setStyleSheet("background-image:none;");
    vInfo->setAlignment(Qt::AlignRight);
    logoLayout->addWidget(new QLabel("v0.1a"));
    logoArea->setFixedSize(291,71);
    logoArea->setMaximumSize(291,71);
    logoArea->setLayout(logoLayout);
    headerLayout->addWidget(logoArea);


    QHBoxLayout *buttonsLayout=new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    QPushButton *newProjectBtn=new QPushButton("New Project");
    newProjectBtn->setObjectName("home");
    QPushButton *openProjectBtn=new QPushButton("Open Project");
    openProjectBtn->setObjectName("home");
    buttonsLayout->addWidget(openProjectBtn);
    buttonsLayout->addWidget(newProjectBtn);

    headerLayout->addLayout(buttonsLayout);

    mainLayout->addLayout(headerLayout);

    QHBoxLayout *recentTitleLayout=new QHBoxLayout;
    QLabel *recentIcon=new QLabel;
    recentIcon->setPixmap(QPixmap(":/Icons/uiIcons_mini_project.png"));
    recentTitleLayout->addWidget(recentIcon);
    QLabel *recentTitle=new QLabel("Recent Projects:");
    recentTitle->setStyleSheet("font-size:14px");
    recentTitleLayout->addWidget(recentTitle);
    recentTitleLayout->addStretch();
    mainLayout->addLayout(recentTitleLayout);

    recentProjectList=new QListWidget;
    recentProjectList->setObjectName("recentList");
    mainLayout->addWidget(recentProjectList);
    //recentProjectList->setSpacing(2);

    this->setLayout(mainLayout);
    this->setObjectName("home");


    connect(newProjectBtn,&QPushButton::clicked,[=]{
        mainWindow->fileMenu->newProject->triggered();
    });
    connect(openProjectBtn,&QPushButton::clicked,[=]{
        mainWindow->fileMenu->openProject->triggered();
    });

    updateRecentList();
}

void HomeWidget::updateRecentList()
{
    recentProjectList->clear();
    QList<QVariantMap> recentProjects=AppSettings::getRecentProjects();
    for(int i=0;i<recentProjects.count();i++){
        QString projectPath=recentProjects.at(i)["projectPath"].toString();
        QString frameworkName=recentProjects.at(i)["frameworkName"].toString();
        QString projectName=QFileInfo(ProjectFilesOperations::getXcodeFilePath(projectPath)).baseName();
        ProjectListItem *itemWidget=new ProjectListItem(mainWindow,frameworkName,projectName,projectPath);
        QListWidgetItem *item=new QListWidgetItem();
        item->setSizeHint(QSize(150, 70));
        QColor color=i%2==1 ? QColor("#2f2f2f"):QColor("#2c2c2c");
        item->setBackground(QBrush(QColor(color)));
        recentProjectList->addItem(item);
        recentProjectList->setItemWidget(item,itemWidget);
    }
}


