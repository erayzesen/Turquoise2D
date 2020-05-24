#include "projectconsole.h"

ProjectConsole::ProjectConsole()
{

}

void ProjectConsole::Run(Turquoise2DEditor *mainWindow)
{
    if(mainWindow->frameworkName=="Cocos2d-x"){
        QString frameworkLocation;
        frameworkLocation=mainWindow->frameworkLibraryPath+"/tools/cocos2d-console/bin";
        //Setting processes
        QProcess *proc=new QProcess();
        QProcessEnvironment env=QProcessEnvironment::systemEnvironment();;
        env.insert("COCOS_CONSOLE_ROOT",frameworkLocation);
        env.insert("PATH", env.value("PATH")+":$COCOS_CONSOLE_ROOT:$PATH");
        env.insert("COCOS_FRAMEWORKS", FileOperations::getAppRootPath()+"/Frameworks/Cocos2d-x");
        env.insert("PATH", env.value("PATH")+":$COCOS_FRAMEWORKS:$PATH");

        proc->setProcessEnvironment(env);
        proc->setReadChannel(QProcess::StandardOutput);

        proc->setWorkingDirectory(mainWindow->projectFolderPath);
        proc->start(frameworkLocation+"/cocos",QStringList()<<"run"<<"-p"<<"mac");
        ConsoleProgressDialog *consoleDialog=new ConsoleProgressDialog("Running "+mainWindow->projectName+"...",mainWindow);
        consoleDialog->show();
        QTextEdit *txField=consoleDialog->textField;
        //Writing commands
        txField->clear();
        txField->append("Running "+mainWindow->projectName+" with "+QFileInfo(mainWindow->frameworkLibraryPath).fileName()+"!...\n");

        //Getting data from qprocess
        connect(proc,&QProcess::readyReadStandardOutput,[=]{
            txField->append(proc->readAll());
        });
        connect(proc,&QProcess::readyRead,[=]{
            txField->append(proc->readAll());
        });
        connect(proc,&QProcess::readyReadStandardError,[=]{
            txField->append(proc->readAll());
        });
        connect(proc,static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::error),[=](QProcess::ProcessError error){
            consoleDialog->setProgressText("Running failed!...");
            consoleDialog->showDetailsBtn->clicked();
        });

        //Listening "Build succeeded mesage"
        connect(txField,&QTextEdit::cursorPositionChanged,[=]{
            if(txField->toPlainText().indexOf("** BUILD SUCCEEDED **")>0){
                consoleDialog->setProgressText("Build succeded! Running application...");
                QTimer::singleShot(3000,[=]{
                    consoleDialog->close();
                });
            }
        });

    }
}

void ProjectConsole::NewProject(Turquoise2DEditor *mainWindow,QString frameworkName, QString frameworkPath, QString projectName, QString projectPath, bool portrait)
{
    //Setting processes
    QString frameworkLocation=frameworkPath+"/tools/cocos2d-console/bin";
    QProcess *proc=new QProcess();
    QProcessEnvironment env=QProcessEnvironment::systemEnvironment();;
    env.insert("COCOS_CONSOLE_ROOT",frameworkLocation);
    env.insert("PATH", env.value("PATH")+":$COCOS_CONSOLE_ROOT:$PATH");
    env.insert("COCOS_FRAMEWORKS", FileOperations::getAppRootPath()+"/Frameworks/Cocos2d-x");
    env.insert("PATH", env.value("PATH")+":$COCOS_FRAMEWORKS:$PATH");

    proc->setProcessEnvironment(env);
    proc->setReadChannel(QProcess::StandardOutput);

    QStringList parameters=QStringList()<<"new"<<projectName<<"-l"<<"cpp"<<"-d"<<projectPath;
    if(portrait){
        parameters<<"--portrait";
    }
    proc->start(frameworkLocation+"/cocos",parameters);

    ConsoleProgressDialog *consoleDialog=new ConsoleProgressDialog("Creating "+projectName+"...",mainWindow);
    consoleDialog->show();
    QTextEdit *txField=consoleDialog->textField;

    txField->clear();
    txField->append("Creating "+projectName+" project with "+QFileInfo(frameworkPath).fileName()+"!...\n");


    connect(proc,&QProcess::readyReadStandardOutput,[=]{
        txField->append(proc->readAll());
    });
    connect(proc,&QProcess::readyRead,[=]{
        txField->append(proc->readAll());
    });
    connect(proc,&QProcess::readyReadStandardError,[=]{
        txField->append(proc->readAll());
    });
    connect(proc, static_cast<void(QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished),[=](int,QProcess::ExitStatus){
        consoleDialog->setProgressText("Project created successfully.Opening project...");
        QTimer::singleShot(2000,[=]{
            consoleDialog->close();
            mainWindow->OpenProject(projectPath+"/"+projectName);
        });
    });
}

QWidget *ProjectConsole::CreateConsoleWindow(QTextEdit* consoleText,QWidget *parent)
{
    QWidget *consoleWindow=new QWidget(parent);
    QVBoxLayout *windowLayout=new QVBoxLayout;
    consoleWindow->setLayout(windowLayout);
    consoleWindow->setFixedSize(400,300);
    consoleWindow->setWindowTitle("Turquoise2D Console");

    consoleText->setAcceptRichText(true);
    consoleText->ensureCursorVisible();
    consoleText->setReadOnly(true);

    windowLayout->addWidget(consoleText);

    return consoleWindow;
}

