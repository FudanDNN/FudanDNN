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

#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include "diagramitem.h"
#include "cnnsubnetcontainer.h"
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE
#include "networkunitbase.h"
//! [0]
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, MoveItem, InsertCNNSubnet };

    explicit DiagramScene(QMenu *itemMenu, QObject *parent = 0);
    QColor itemColor() const { return myItemColor; }
    QColor lineColor() const { return myLineColor; }
    void setLineColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setFont(const QFont &font);
	void setUnitTypeToInsert(NetworkUnitBase::NetworkUnitType t){ this->unitTypeToInsert = t; }
	/* 生成一个Network unit以及与之绑定的DiagramItem，返回DiagramItem对象*/
	DiagramItem* addNetworkUnit(NetworkUnitBase::NetworkUnitType type);
	/* 删除一个network unit*/
	void removeNetworkUnit(NetworkUnitBase *);
	/* 删除一个network unit*/
	void removeNetworkUnit(DiagramItem*);
	/* 删除一个subnet*/
	void removeSubnet(int id);
	int getItemPadding() const { return this->itemPadding; }
	/*请求修改所有CNN子网络的虚框大小*/
	void resizeSubnets();
	/* 由item告知自己被selected了*/
	void informItemSelected(QGraphicsItem *item);
    /* 由item告知自己被unselected了*/
    void informItemUnSelected(QGraphicsItem *item);
    /* 用于读取文件时重建网络 */
    void addArrow(QGraphicsItem* item1, QGraphicsItem* item2);
    CNNSubnetContainer* addCNN(QPointF pos);
public slots:
    void setMode(Mode mode);
	void setItemPadding(int padding){ this->itemPadding = padding; emit itemPaddingChanged(padding); }
signals:
	void itemInserted(QGraphicsItem *item);
    void itemSelected(QGraphicsItem *item);
    void itemUnSelected(QGraphicsItem *item);
	void itemPaddingChanged(int padding);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;

private:
    bool isItemChange(int type);
	NetworkUnitBase::NetworkUnitType unitTypeToInsert;
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QColor myItemColor;
    QColor myLineColor;
	int itemPadding;
};
//! [0]

#endif // DIAGRAMSCENE_H
