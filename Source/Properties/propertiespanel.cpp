#include "propertiespanel.h"

PropertiesPanel::PropertiesPanel(QWidget *parent) : QWidget(parent)
{
    mainLayout=new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setMargin(0);

    itemContainerLayout=new QHBoxLayout;
    itemContainerLayout->setMargin(0);
    itemContainer=new QWidget(this);
    itemContainer->setLayout(itemContainerLayout);


    //mainLayout->setMargin(6);
    mainLayout->setContentsMargins(12,0,12,0);
    //this->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setWidget(itemContainer);
    scroll->setFrameShape(QFrame::NoFrame);
    mainLayout->addWidget(scroll);


}

void PropertiesPanel::AddGameProperties(PropertiesItem *item)
{
    itemContainerLayout->addWidget(item);
    lastItem=item;
    //scrollArea->setWidget(this);
}

void PropertiesPanel::ClearGameProperties()
{
    if(lastItem!=NULL){
        itemContainerLayout->removeWidget(lastItem);
        delete lastItem;
        lastItem=NULL;
    }

}

