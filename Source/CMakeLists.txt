cmake_minimum_required(VERSION 3.0)

# Add folder where are supportive functions
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Include Qt basic functions
include(QtCommon)

# Basic information about project

project(Turquoise2D VERSION 0.1)

# Set PROJECT_VERSION_PATCH and PROJECT_VERSION_TWEAK to 0 if not present, needed by add_project_meta
fix_project_version()

set(COMPANY "Turquoise2D")
set(IDENTIFIER "org.erayzesen.Turquoise2D")

set(SOURCE_FILES
./HomeLauncher/projectlistitem.cpp
./HomeLauncher/homewidget.cpp
./form.cpp
./ProjectConsole/projectconsole.cpp
./ProjectConsole/consoleprogressdialog.cpp
./customtitlebar.cpp
./Hierarchy/hierarchylist.cpp
./Hierarchy/groupitem.cpp
./Hierarchy/hierarchyitem.cpp
./Hierarchy/hierarchypanel.cpp
./Hierarchy/hierarchylistview.cpp
./turquoise2deditor.cpp
./DialogsAndMenus/projectmenu.cpp
./DialogsAndMenus/windowmenu.cpp
./DialogsAndMenus/filemenu.cpp
./DialogsAndMenus/newprojectdialog.cpp
./commands.cpp
./contextmenu.cpp
./appsettings.cpp
./fileoperations.cpp
./ClassGenerator/classoperations.cpp
./ClassGenerator/classgenerator.cpp
./Canvas/selectiontransformitem.cpp
./Canvas/collideranchor.cpp
./Canvas/selectionscaleanchor.cpp
./Canvas/guidesandgrids.cpp
./Canvas/graphicsview.cpp
./Canvas/graphicsscene.cpp
./Canvas/colliderrenderer.cpp
./Canvas/scenepanel.cpp
./Canvas/transformanchor.cpp
./Canvas/selectiontransformrect.cpp
./Canvas/objectgizmosrenderer.cpp
./Canvas/selecteditemscontainer.cpp
./Canvas/gameitem.cpp
./Properties/propertiesitem.cpp
./Properties/collidershapeitem.cpp
./Properties/propertiespanel.cpp
./mathoperations.cpp
./qtPlist/plistserializer.cpp
./qtPlist/plistparser.cpp
./Collections/collectionlistview.cpp
./Collections/collectionfoldersystemmodel.cpp
./Collections/collectiontreeview.cpp
./Collections/collectionspritesheetmodel.cpp
./Collections/proxymodel.cpp
./Collections/collectionfilemodel.cpp
./Collections/collectionfilesystemmodel.cpp
./Collections/collectionbrowser.cpp
./Collections/customfoldermodel.cpp
./FileSelector/fileselectormodel.cpp
./FileSelector/spritesheetselectormodel.cpp
./FileSelector/fileselectordialog.cpp
./filetools.cpp
./SpriteSheet/spritesheet.cpp
./SpriteSheet/spritesheetparser.cpp
./projectfilesoperations.cpp
./main.cpp
)

set(RESOURCE_FILES
res.qrc
./qdarkstyle/style.qrc
)

add_project_meta(META_FILES_TO_INCLUDE)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)

add_executable(${PROJECT_NAME} ${OS_BUNDLE} # Expands to WIN32 or MACOS_BUNDLE depending on OS
    ${SOURCE_FILES} ${META_FILES_TO_INCLUDE} ${RESOURCE_FILES}
)

qt5_use_modules(${PROJECT_NAME} Widgets Xml)
