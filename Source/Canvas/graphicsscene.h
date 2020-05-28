#pragma once

#include "QWidget"
#include "QObject"
#include "QGraphicsView"
#include "QGraphicsSceneDragDropEvent"
#include "QUrl"
#include "QMimeData"
#include "QDebug"
#include "QGraphicsPixmapItem"
#include "QPoint"
#include "QFileInfo"
#include "QDir"
#include "QApplication"
#include "Hierarchy/hierarchylistview.h"
#include "Canvas/selectiontransformitem.h"
#include "Canvas/gameitem.h"
#include "Canvas/transformtypes.h"
#include "Canvas/transformanchor.h"
#include "mathoperations.h"
#include "Canvas/scenepanel.h"
#include "sceneeditmodes.h"
#include "Canvas/collideranchor.h"
#include "commands.h"
#include "Canvas/guidesandgrids.h"
#include "QJsonObject"

class HierarchyListView;
class TransformAnchor;
class PropertiesPanel;
class PropertiesItem;
class Turquoise2DEditor;
class ColliderAnchor;
class GameItem;
enum class GameItemTypes;
class GuidesAndGrids;
enum class SnapStyle{withCenter,withBounds};
enum class DrawCollisionDedection{None,DedectionForAllItems,DedectionForSimilarItems};
class ScenePanel;
class HierarchyPanel;
class GraphicsView;
class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(QString _sceneFilePath,PropertiesPanel *_propPanel,ScenePanel *_scenePanel);
    ~GraphicsScene();
    Turquoise2DEditor *mainWindow;
    ScenePanel *scenePanel;
    HierarchyPanel *hierarchyPanel;
    PropertiesPanel *propPanel;
    GraphicsView *view;
    void drawOriginPoint();
    SceneEditModes editMode=SceneEditModes::GameItemsEdit;

    //Scene Properties
    void sceneChanged(); //This function is re-calling original sceneChanged function from scenePanel.
    QString sceneFilePath;
    bool isSnapMode=false;
    GuidesAndGrids *guidesAndGrids;
    QSizeF gridSize=QSizeF(64,64);
    SnapStyle snapStyle=SnapStyle::withCenter;
    DrawCollisionDedection drawCollisionDedection=DrawCollisionDedection::DedectionForSimilarItems;

    //SCENE EDIT MODES
    void setEditMode(SceneEditModes targetMode);
    //Game Items Edit Mode
    void GameItemsEditMode_MousePress(QGraphicsSceneMouseEvent *mouseEvent);
    void GameItemsEditMode_MouseMove(QGraphicsSceneMouseEvent *mouseEvent);
    void GameItemsEditMode_MouseRelease(QGraphicsSceneMouseEvent *mouseEvent);
    //Collider Edit Mode
    void ColliderEditMode_MousePress(QGraphicsSceneMouseEvent *mouseEvent);
    void ColliderEditMode_MouseMove(QGraphicsSceneMouseEvent *mouseEvent);
    void ColliderEditMode_MouseRelease(QGraphicsSceneMouseEvent *mouseEvent);
    //Pan Edit Mode
    void PanEditMode_MousePress(QGraphicsSceneMouseEvent *mouseEvent);
    void PanEditMode_MouseMove(QGraphicsSceneMouseEvent *mouseEvent);
    void PanEditMode_MouseRelease(QGraphicsSceneMouseEvent *mouseEvent);
    //Zoom Edit Mode
    void ZoomEditMode_MousePress(QGraphicsSceneMouseEvent *mouseEvent);
    void ZoomEditMode_MouseMove(QGraphicsSceneMouseEvent *mouseEvent);
    void ZoomEditMode_MouseRelease(QGraphicsSceneMouseEvent *mouseEvent);
    void resetZoomLevel();
    //Draw Edit Mode
    void DrawEditMode_MousePress(QGraphicsSceneMouseEvent *mouseEvent);
    void DrawEditMode_MouseMove(QGraphicsSceneMouseEvent *mouseEvent);
    void DrawEditMode_MouseRelease(QGraphicsSceneMouseEvent *mouseEvent);
    QList<GameItem*> collisionDedectiosnForDrawing(QPointF point,QSizeF size,QJsonObject itemJson=QJsonObject());
    //Eyedropper Edit Mode
    void EyedropperEditMode_MousePress(QGraphicsSceneMouseEvent *mouseEvent);
    void EyedropperEditMode_MouseMove(QGraphicsSceneMouseEvent *mouseEvent);
    void EyedropperEditMode_MouseRelease(QGraphicsSceneMouseEvent *mouseEvent);

    //lastEditMode
    SceneEditModes lastEditMode;

    void clearTransformSelector(bool oneSided=false,bool savePropItem=false);
    void addTransformSelector(QList<QGraphicsItem*> items, PropertiesItem *propItem=NULL);
    void resetPropItem();
    SelectionTransformItem* lastTransformItem=NULL;
    TransformAnchor* selectedAnchor=NULL;
    SelectionTransformItem* lastSelectedTransformItem=NULL;
    QMap<QString, bool> setMouseIcons(QGraphicsSceneMouseEvent *mouseEvent);

    ColliderAnchor *lastSelectedColliderAnchor=NULL;

    //Default edit mode helpers
    QList<QGraphicsItem*> lastSelectedItems=QList<QGraphicsItem*>();
    QPointF *startSelectionPoint=NULL;
    QPointF *startRotatePoint=NULL;
    QPointF *startMovePoint=new QPointF(0,0);
    QSizeF *startScaleSize=new QSizeF(1,1);
    QUndoStack * stack;
    QJsonObject eyedroppedItemJson;
    QSizeF eyedroppedItemSize;
    int externalCounter=0;

    //Snap Filters

    QPointF snapFilter(QPointF point,QSizeF _gridSize,bool alignCenter=false);
    void rePosForSnap(GameItem *item);
    void rePosForSnap(SelectionTransformItem *item);

    //Physics edit mode helpers
    QList<ColliderAnchor*> *colliderAnchors=new QList<ColliderAnchor*>();
    void addColliderAnchors(QVariantMap *collider,GameItem* gameItem,ColliderShapeItem *inspectorItem);
    void clearColliderAnchors();
    void updateColliderAnchors();
    QVariantMap* lastEditedCollider=NULL;

    //Pan edit mode helpers
    bool isPanStarted=false;


    //Generating JSON Format Helpers
    QJsonObject toJsonObject();
    QJsonObject *fromJsonFile(QString filePath);
    QJsonObject *sceneJsonObject;

protected:
    virtual void drawBackground(QPainter * painter, const QRectF & rect);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    //void dragLeaveEvent(QGraphicsSceneDragDropEvent*event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) ;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) ;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) ;


protected slots:
    void isSelectedChanged();
};

