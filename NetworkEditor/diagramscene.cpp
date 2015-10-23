/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "diagramscene.h"
#include "arrow.h"
#include "cnnsubnetcontainer.h"
#include "cnnunitbase.h"
#include "networkunitbase.h"
#include "networkunitpool.h"
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <qmessagebox.h>

DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
	: QGraphicsScene(parent), itemPadding(25), myItemMenu(itemMenu), myMode(MoveItem), 
	line(0), myItemColor(Qt::white), myLineColor(Qt::black)
{
}

void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}

void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

DiagramItem* DiagramScene::addNetworkUnit(NetworkUnitBase::NetworkUnitType type){
	DiagramItem *item;
	item = new DiagramItem(myItemMenu);
	NetworkUnitPool::createNetworkUnit(item, type);
	item->setBrush(myItemColor);
	addItem(item);
	return item;
}
void DiagramScene::removeNetworkUnit(NetworkUnitBase* networkUnit){
	DiagramItem *ditem = networkUnit->getDiagramItem();
	removeNetworkUnit(ditem);
}
void DiagramScene::removeNetworkUnit(DiagramItem* ditem){
	ditem->removeArrows();
	this->removeItem(ditem);
	NetworkUnitPool::unregisterUnit(ditem->getNetworkUnitPtr());
	delete ditem;
}
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    DiagramItem *item;
	CNNSubnetContainer *cnnSubnetContainer;
	CNNSubnetContainer *targetSubnet = nullptr;
	QPoint tmpPos;
    switch (myMode) {
        case InsertItem:
			if (this->unitTypeToInsert== NetworkUnitBase::NULL_UNIT){
				//error called!
				//TODO
				return;
			}
			// check if the item will be put in a subnet
			foreach(QGraphicsItem *item, this->items(mouseEvent->scenePos())){
				if (dynamic_cast<CNNSubnetContainer*>(item) != nullptr){
					targetSubnet = dynamic_cast<CNNSubnetContainer*>(item);
					break;
				}
			}
			if (NetworkUnitBase::isCNNUnit(this->unitTypeToInsert)){
				if (targetSubnet == nullptr) {
					QMessageBox::warning(0, "Forbidden Operation",
						"It's forbidden to install CNN unit without CNN Subnet!", QMessageBox::StandardButton::Ok);
					return;
				}
			}
			else{
				if (targetSubnet != nullptr){
					QMessageBox::warning(0, "Forbidden Operation",
						"It's forbidden to install normal unit into CNN Subnet!", QMessageBox::StandardButton::Ok);
					return;
				}
			}
			//TODO
			item = addNetworkUnit(this->unitTypeToInsert);
            item->setPos(mouseEvent->scenePos());
			if (NetworkUnitBase::isCNNUnit(this->unitTypeToInsert)){
				CNNUnitBase *ptr = dynamic_cast<CNNUnitBase*> (item->getNetworkUnitPtr());
				targetSubnet->getCNNSubnet()->registerUnit(ptr);
                dynamic_cast<CNNUnitHyperParameterControl*>(ptr->getHyperParameterControl())->setCnnId(targetSubnet->getCNNSubnet()->getId());
			}
			this->unitTypeToInsert = NetworkUnitBase::NULL_UNIT;			
            emit itemInserted(item);
            break;
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
			addItem(line);
            break;
		case InsertCNNSubnet:
            /*
			cnnSubnetContainer = new CNNSubnetContainer(0, 0);
			addItem(cnnSubnetContainer);
			cnnSubnetContainer->setPos(mouseEvent->screenPos());
			cnnSubnetContainer->setZValue(-20);
			
			//add input
			item = addNetworkUnit(NetworkUnitBase::NetworkUnitType::CNN_INPUT_UNIT);
			tmpPos = mouseEvent->screenPos();
			tmpPos.setX(tmpPos.x() - cnnSubnetContainer->getWidth() / 2 + 10 + item->WIDTH);
			tmpPos.setY(tmpPos.y() - cnnSubnetContainer->getHeight() / 2);
			item->setPos(tmpPos);
            dynamic_cast<CNNUnitHyperParameterControl*> (item->getNetworkUnitPtr()->getHyperParameterControl())
                ->setCnnId(cnnSubnetContainer->getCNNSubnet()->getId());
			cnnSubnetContainer->getCNNSubnet()->registerUnit(dynamic_cast<CNNUnitBase*> (item->getNetworkUnitPtr()));
			//add ouput
			item = addNetworkUnit(NetworkUnitBase::NetworkUnitType::CNN_OUTPUT_UNIT);
			tmpPos = mouseEvent->screenPos();
			tmpPos.setX(tmpPos.x() + cnnSubnetContainer->getWidth() / 2 - 10 - item->WIDTH);
			tmpPos.setY(tmpPos.y() + cnnSubnetContainer->getHeight() / 2);
			item->setPos(tmpPos);
            dynamic_cast<CNNUnitHyperParameterControl*> (item->getNetworkUnitPtr()->getHyperParameterControl())
                ->setCnnId(cnnSubnetContainer->getCNNSubnet()->getId());
			cnnSubnetContainer->getCNNSubnet()->registerUnit(dynamic_cast<CNNUnitBase*> (item->getNetworkUnitPtr()));
            emit itemInserted(cnnSubnetContainer);
            */
            emit itemInserted(addCNN(mouseEvent->scenePos()));
			break;
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

 void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();
        removeItem(line);
        delete line;
        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
			//check whether the link is availble
			if (!(
				startItem->getNetworkUnitPtr()->isAvailbleInput(endItem->getNetworkUnitPtr())
				&&
				endItem->getNetworkUnitPtr()->isAvailbleOutput(startItem->getNetworkUnitPtr())
				)){
				QMessageBox::warning(0, "Forbidden Operation", "Input node and output node is NOT compatible.",QMessageBox::StandardButton::Ok);
			}
			else {
                addArrow(startItem, endItem);
			}
        }
    }
	//check subnet size change
	foreach(QGraphicsItem *item, items()){
		if (dynamic_cast<CNNSubnetContainer*>(item) != nullptr){
			CNNSubnetContainer *subnetCon = dynamic_cast<CNNSubnetContainer*>(item);
			subnetCon->resizeByItems();
		}
	}
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}


bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

void DiagramScene::resizeSubnets(){
	CNNSubnetContainer *targetSubnet = nullptr;
	foreach(QGraphicsItem *item, this->items()){
		if (dynamic_cast<CNNSubnetContainer*>(item) != nullptr){
			targetSubnet = dynamic_cast<CNNSubnetContainer*>(item); 
			targetSubnet->resizeByItems();
		}
	}
}

void DiagramScene::removeSubnet(int id){
	for (auto iter = NetworkUnitPool::cnnSubnets.begin(); iter != NetworkUnitPool::cnnSubnets.end(); ++iter){
		if ((*iter)->getId() == id) {
			CNNSubnet *subnet = *iter;
			//clean all items in the subnet
			const std::vector<CNNUnitBase*> &unitList = subnet->getUnitsList();
			for (int i = unitList.size() - 1; i >= 0; --i){
				removeNetworkUnit(unitList[i]->getDiagramItem());
			}
			//remove the subnet item
			foreach(QGraphicsItem *gitem, items()){
				if (dynamic_cast<CNNSubnetContainer*> (gitem) != nullptr){
					CNNSubnetContainer* target = dynamic_cast<CNNSubnetContainer*> (gitem);
					if (target->getCNNSubnet() == subnet){
						//found the target 
						removeItem(gitem);
						break;
					}
				}
			}
			NetworkUnitPool::cnnSubnets.erase(iter);
			delete subnet;
			break;
		}
	}
}
CNNSubnetContainer* DiagramScene::addCNN(QPointF pos){
    CNNSubnetContainer *cnnSubnetContainer = new CNNSubnetContainer(0, 0);
    DiagramItem *item;
    QPointF tmpPos;
    addItem(cnnSubnetContainer);
    //cnnSubnetContainer->setPos(mouseEvent->screenPos());
    cnnSubnetContainer->setPos(pos);
    cnnSubnetContainer->setZValue(-20);

    //add input
    item = addNetworkUnit(NetworkUnitBase::NetworkUnitType::CNN_INPUT_UNIT);
    tmpPos = pos;
    tmpPos.setX(tmpPos.x() - cnnSubnetContainer->getWidth() / 2 + 10 + item->WIDTH);
    tmpPos.setY(tmpPos.y() - cnnSubnetContainer->getHeight() / 2);
    item->setPos(tmpPos);
    dynamic_cast<CNNUnitHyperParameterControl*> (item->getNetworkUnitPtr()->getHyperParameterControl())
        ->setCnnId(cnnSubnetContainer->getCNNSubnet()->getId());
    cnnSubnetContainer->getCNNSubnet()->registerUnit(dynamic_cast<CNNUnitBase*> (item->getNetworkUnitPtr()));
    //add ouput
    item = addNetworkUnit(NetworkUnitBase::NetworkUnitType::CNN_OUTPUT_UNIT);
    tmpPos = pos;
    tmpPos.setX(tmpPos.x() + cnnSubnetContainer->getWidth() / 2 - 10 - item->WIDTH);
    tmpPos.setY(tmpPos.y() + cnnSubnetContainer->getHeight() / 2);
    item->setPos(tmpPos);
    dynamic_cast<CNNUnitHyperParameterControl*> (item->getNetworkUnitPtr()->getHyperParameterControl())
        ->setCnnId(cnnSubnetContainer->getCNNSubnet()->getId());
    cnnSubnetContainer->getCNNSubnet()->registerUnit(dynamic_cast<CNNUnitBase*> (item->getNetworkUnitPtr()));
    return cnnSubnetContainer;
}
void DiagramScene::addArrow(QGraphicsItem* item1, QGraphicsItem* item2){
    DiagramItem* startItem = dynamic_cast<DiagramItem*> (item1);
    DiagramItem* endItem = dynamic_cast<DiagramItem*>(item2);
    if (startItem == nullptr || endItem == nullptr){
        exit(233);
    }
    Arrow *arrow = new Arrow(startItem, endItem, myItemMenu);
    arrow->setColor(myLineColor);
    startItem->addArrow(arrow);
    endItem->addArrow(arrow);
    arrow->setZValue(-1000.0);
    addItem(arrow);
    arrow->updatePosition();
    NetworkUnitPool::addLink(startItem->getNetworkUnitPtr(), endItem->getNetworkUnitPtr());
}
void DiagramScene::informItemSelected(QGraphicsItem * item){
	emit this->itemSelected(item);
}
void DiagramScene::informItemUnSelected(QGraphicsItem * item){
    //check if no item is selected
    foreach(QGraphicsItem *iter, this->selectedItems()){
        if (qgraphicsitem_cast<DiagramItem *>(iter) != nullptr) continue;
    }
    emit this->itemUnSelected(item);
}