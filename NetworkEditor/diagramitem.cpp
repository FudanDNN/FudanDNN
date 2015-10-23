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

#include "diagramitem.h"
#include "networkunitbase.h"
#include "arrow.h"
#include "cnnunitbase.h"
#include "networkunitpool.h"
#include "diagramscene.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QStyleOptionGraphicsItem>
#include <QMenu>
#include <QPainter>

DiagramItem::DiagramItem( QMenu *contextMenu, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
    myContextMenu = contextMenu;
	int xpos = WIDTH / 2;
	int ypos = HEIGHT / 2;
    myPolygon << QPointF(-xpos, -ypos) << QPointF(xpos, -ypos)
              << QPointF(xpos, ypos) << QPointF(-xpos, ypos)
              << QPointF(-xpos, -ypos);

    //setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	this->networkUnitPtr = nullptr;
}

void DiagramItem::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void DiagramItem::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void DiagramItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}

QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);
    return pixmap;
}

void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
	if (myContextMenu != 0) myContextMenu->exec(event->screenPos());
	//event->accept();
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemPositionChange) {
		foreach(Arrow *arrow, arrows) {
			arrow->updatePosition();
		}
	}
    if (change == QGraphicsItem::ItemSelectedChange){
        if (value == true){
            dynamic_cast<DiagramScene*> (this->scene())->informItemSelected(this);
        }
        else {
            dynamic_cast<DiagramScene*> (this->scene())->informItemUnSelected(this);
        }
    }
    return value;
}

void DiagramItem :: paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *){
	QPen pen(Qt::black);
	if (option->state & QStyle::State_Selected){
		pen.setStyle(Qt::DotLine);
		pen.setWidth(2);
	}
	painter->setPen(pen);
	painter->setBrush(Qt::white);
	painter->drawPolygon(myPolygon);
    QFont fontPaint("Times New Roman", 11, 10, false);
    fontPaint.setBold(true);
    painter->setFont(fontPaint);
	painter->drawText(-WIDTH/2+5, 0, networkUnitPtr->getTitle());
}

QPainterPath DiagramItem::shape() const{
	QPainterPath path;
	path.addPolygon(myPolygon);
	return path;
}

void DiagramItem::mousePressEvent(QGraphicsSceneMouseEvent * event){
	//qWarning("Caught mouse pressed!");  
	QGraphicsItem::mousePressEvent(event);
	//dynamic_cast<DiagramScene*> (this->scene())->informItemSelected(this);
}