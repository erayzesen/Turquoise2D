#ifndef PROPERTIESPANEL_H
#define PROPERTIESPANEL_H

#include <QWidget>
#include <Properties/propertiesitem.h>
#include <QBoxLayout>
#include <QGroupBox>

class PropertiesItem;
class PropertiesPanel : public QWidget
{
    Q_OBJECT
public:
    explicit PropertiesPanel(QWidget *parent = 0);
    void AddGameProperties(PropertiesItem *item);
    void ClearGameProperties();
    QHBoxLayout* mainLayout;
    QWidget* itemContainer;
    QHBoxLayout* itemContainerLayout;
    QGroupBox *groupBox;
    PropertiesItem* lastItem=NULL;
    QScrollArea *scroll;


signals:

public slots:
};

#endif // PROPERTIESPANEL_H
