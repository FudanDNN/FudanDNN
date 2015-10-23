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

#include "cnnsubnetcontainer.h"
#include "cnnsubnet.h"
#include "diagramscene.h"
#include "networkunitpool.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QStyleOptionGraphicsItem>
#include <QMenu>
#include <QPainter>

//! [0]
CNNSubnetContainer::CNNSubnetContainer(QMenu *contextMenu, QGraphicsItem *parent)
	: QGraphicsPolygonItem(parent), ignoreChange(false)
{
	myContextMenu = contextMenu;
	int xpos = width / 2;
	int ypos = height / 2;
	myPolygon << QPointF(-xpos, -ypos) << QPointF(xpos, -ypos)
		<< QPointF(xpos, ypos) << QPointF(-xpos, ypos)
		<< QPointF(-xpos, -ypos);
	setPolygon(myPolygon);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	this->cnnSubnet = NetworkUnitPool::createSubnet();
}

QPixmap CNNSubnetContainer::image() const
{
	QPixmap pixmap(250, 250);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setPen(QPen(Qt::black, 8));
	painter.translate(125, 125);
	painter.drawPolyline(myPolygon);
	return pixmap;
}

void CNNSubnetContainer::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	scene()->clearSelection();
	setSelected(true);
	if (myContextMenu != 0) myContextMenu->exec(event->screenPos());
	//event->accept();
}

QVariant CNNSubnetContainer::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (ignoreChange) return value;
	static QPointF orignalPos;
	if (change == GraphicsItemChange::ItemPositionHasChanged){
		QPointF dPos = pos()-orignalPos;
		const std::vector<CNNUnitBase*> & unitsList = cnnSubnet->getUnitsList();
		for (auto iter = unitsList.cbegin(); iter != unitsList.cend(); ++iter){
			(*iter)->getDiagramItem()->moveBy(dPos.x(), dPos.y());
		}
	}
	if (change == GraphicsItemChange::ItemPositionChange){
		orignalPos = pos();
	}
	return value;
}

void CNNSubnetContainer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *){
	QPen pen(Qt::blue);
	if (option->state & QStyle::State_Selected){
		pen.setStyle(Qt::DotLine);
		pen.setWidth(2);
	}
	painter->setPen(pen);
	painter->setBrush(Qt::transparent);
	painter->drawPolygon(myPolygon);
}

QPainterPath CNNSubnetContainer::shape() const{
	QPainterPath path;
	path.addPolygon(myPolygon);
	return path;
}

void CNNSubnetContainer::resizeByItems(){
	if (ignoreChange) return;
	ignoreChange = true;
	prepareGeometryChange();
	const std::vector<CNNUnitBase*> &units = getCNNSubnet()->getUnitsList();
	QPointF leftUp, rightDown;
	leftUp.setX(INFINITY);
	leftUp.setY(INFINITY);
	rightDown.setX(0);
	rightDown.setY(0);
	for (auto iter = units.cbegin(); iter != units.cend(); ++iter){
		auto ditem = (*iter)->getDiagramItem();
		QPointF tmppos = ditem->pos();
		tmppos.setX(tmppos.x() - DiagramItem::WIDTH);
		NetworkUnitBase::NetworkUnitType unitType = ditem->getNetworkUnitPtr()->getTypeId();
		if (unitType != NetworkUnitBase::CNN_INPUT_UNIT && unitType != NetworkUnitBase::CNN_OUTPUT_UNIT){
			tmppos.setY(tmppos.y() - DiagramItem::HEIGHT);
		}
		if (tmppos.x() < leftUp.x()){
			leftUp.setX(tmppos.x());
		}
		if (tmppos.y() < leftUp.y()){
			leftUp.setY(tmppos.y());
		}
		tmppos = ditem->pos();
		tmppos.setX(tmppos.x() + DiagramItem::WIDTH);
		if (unitType != NetworkUnitBase::CNN_INPUT_UNIT && unitType != NetworkUnitBase::CNN_OUTPUT_UNIT){
			tmppos.setY(tmppos.y() + DiagramItem::HEIGHT);
		}
		if (tmppos.x() > rightDown.x()){
			rightDown.setX(tmppos.x());
		}
		if (tmppos.y() > rightDown.y()){
			rightDown.setY(tmppos.y());
		}
	}
	QPointF center((leftUp.x() + rightDown.x()) / 2, (leftUp.y() + rightDown.y()) / 2);
	this->setPos(center);
	width = rightDown.x() - leftUp.x();
	height = rightDown.y() - leftUp.y();
	int xpos = width / 2;
	int ypos = height / 2;
	myPolygon.clear();
	myPolygon << QPointF(-xpos, -ypos) << QPointF(xpos, -ypos)
		<< QPointF(xpos, ypos) << QPointF(-xpos, ypos)
		<< QPointF(-xpos, -ypos);
	setPolygon(myPolygon);
	//reset input and output node
	CNNUnitBase *inputNode = units[0];
	inputNode->getDiagramItem()->setX(leftUp.x() + DiagramItem::WIDTH );
	inputNode->getDiagramItem()->setY(leftUp.y());
	CNNUnitBase *outputNode = units[1];
	outputNode->getDiagramItem()->setX(rightDown.x() - DiagramItem::WIDTH );
	outputNode->getDiagramItem()->setY(rightDown.y());
	ignoreChange = false;
	this->update();

}
void CNNSubnetContainer::mousePressEvent(QGraphicsSceneMouseEvent * event){
    qWarning("Caught mouse pressed!");
    QGraphicsItem::mousePressEvent(event);
    dynamic_cast<DiagramScene*> (this->scene())->informItemSelected(this);
}
