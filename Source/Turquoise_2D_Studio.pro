#-------------------------------------------------
#
# Project created by QtCreator 2015-09-23T05:29:31
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Turquoise_2D_Studio
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
    turquoise2deditor.cpp \
    fileoperations.cpp \
    contextmenu.cpp \
    Collections/collectionbrowser.cpp \
    Collections/collectionlistview.cpp \
    Collections/collectiontreeview.cpp \
    Collections/customfoldermodel.cpp \
    Canvas/graphicsscene.cpp \
    Canvas/graphicsview.cpp \
    customtitlebar.cpp \
    Collections/proxymodel.cpp \
    Collections/collectionfilesystemmodel.cpp \
    Canvas/gameitem.cpp \
    Hierarchy/hierarchyitem.cpp \
    Hierarchy/hierarchylistview.cpp \
    Canvas/selectiontransformitem.cpp \
    Canvas/transformanchor.cpp \
    Canvas/selectiontransformrect.cpp \
    Canvas/guidesandgrids.cpp \
    mathoperations.cpp \
    commands.cpp \
    Properties/propertiespanel.cpp \
    Properties/propertiesitem.cpp \
    SpriteSheet/spritesheet.cpp \
    qtPlist/plistparser.cpp \
    qtPlist/plistserializer.cpp \
    Collections/collectionfoldersystemmodel.cpp \
    FileSelector/fileselectordialog.cpp \
    Collections/collectionspritesheetmodel.cpp \
    FileSelector/fileselectormodel.cpp \
    FileSelector/spritesheetselectormodel.cpp \
    Canvas/colliderrenderer.cpp \
    Properties/collidershapeitem.cpp \
    Canvas/collideranchor.cpp \
    Canvas/scenepanel.cpp \
    Hierarchy/hierarchypanel.cpp \
    Hierarchy/groupitem.cpp \
    Canvas/objectgizmosrenderer.cpp \
    projectfilesoperations.cpp \
    Canvas/selecteditemscontainer.cpp \
    DialogsAndMenus/newprojectdialog.cpp \
    DialogsAndMenus/filemenu.cpp \
    ClassGenerator/classgenerator.cpp \
    ClassGenerator/classoperations.cpp \
    ProjectConsole/projectconsole.cpp \
    ProjectConsole/consoleprogressdialog.cpp \
    HomeLauncher/homewidget.cpp \
    HomeLauncher/projectlistitem.cpp \
    appsettings.cpp \
    DialogsAndMenus/windowmenu.cpp \
    DialogsAndMenus/projectmenu.cpp

HEADERS  += turquoise2deditor.h \
    fileoperations.h \
    contextmenu.h \
    Collections/collectionbrowser.h \
    Collections/collectionlistview.h \
    Collections/collectiontreeview.h \
    Collections/customfoldermodel.h \
    Canvas/graphicsscene.h \
    Canvas/graphicsview.h \
    customtitlebar.h \
    Collections/proxymodel.h \
    Collections/collectionfilesystemmodel.h \
    Canvas/gameitem.h \
    Canvas/guidesandgrids.h \
    Canvas/transformtypes.h \
    Hierarchy/hierarchyitem.h \
    Hierarchy/hierarchylistview.h \
    Canvas/selectiontransformitem.h \
    Canvas/transformanchor.h \
    Canvas/selectiontransformrect.h \
    mathoperations.h \
    commands.h \
    Properties/propertiespanel.h \
    Properties/propertiesitem.h \
    SpriteSheet/spritesheet.h \
    qtPlist/plistparser.h \
    qtPlist/plistserializer.h \
    Collections/collectionfoldersystemmodel.h \
    FileSelector/fileselectordialog.h \
    Collections/collectionspritesheetmodel.h \
    FileSelector/fileselectormodel.h \
    FileSelector/spritesheetselectormodel.h \
    Canvas/sceneeditmodes.h \
    Canvas/colliderrenderer.h \
    Properties/collidershapeitem.h \
    Canvas/collideranchor.h \
    Canvas/scenepanel.h \
    Hierarchy/hierarchypanel.h \
    Hierarchy/groupitem.h \
    Canvas/objectgizmosrenderer.h \
    projectfilesoperations.h \
    Canvas/selecteditemscontainer.h \
    DialogsAndMenus/newprojectdialog.h \
    DialogsAndMenus/filemenu.h \
    ClassGenerator/classgenerator.h \
    ClassGenerator/classoperations.h \
    ProjectConsole/projectconsole.h \
    ProjectConsole/consoleprogressdialog.h \
    HomeLauncher/homewidget.h \
    HomeLauncher/projectlistitem.h \
    appsettings.h \
    DialogsAndMenus/windowmenu.h \
    DialogsAndMenus/projectmenu.h

FORMS    += turquoise2deditor.ui \
    customtitlebar.ui \
    Collections/collectionbrowser.ui

RESOURCES += \
    qdarkstyle/style.qrc \
    res.qrc \
    mouse_icons.qrc

ICON = appLogo.icns
