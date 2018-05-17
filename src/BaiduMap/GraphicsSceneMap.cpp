#include "GraphicsSceneMap.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

GraphicsSceneMap::GraphicsSceneMap(QObject *parent)
    :QGraphicsScene(parent),m_line(0),m_lastModifyGroup(0),m_level(1)
{

    setBackgroundBrush(QBrush(QPixmap(":/res/timg.jpg")));
    connect(this,SIGNAL(sceneRectChanged(const QRectF&)) ,SLOT(slotSceneRectChange(const QRectF&)));
    //setForegroundBrush(QColor(255, 255, 255, 50));

    // a grid foreground
    //setForegroundBrush(QBrush(Qt::lightGray, Qt::CrossPattern));
}


void GraphicsSceneMap::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{


    const QMimeData* mimeData = event->mimeData();
    if(mimeData->hasImage())
    {
        qDebug()<<"hasImage";
        //QImage image = qvariant_cast<QImage>(mimeData->imageData());



    }
    else if(mimeData->hasText())
    {
        qDebug()<<"hasText"<<mimeData->text();

    }
    event->acceptProposedAction();
    //QGraphicsScene::dragEnterEvent(event);


}

void GraphicsSceneMap::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dragMoveEvent(event);

}

void GraphicsSceneMap::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dragLeaveEvent(event);
}

void GraphicsSceneMap::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dropEvent(event);
    qDebug()<<"dropEvent........";


    const QMimeData* mimeData = event->mimeData();
    if(mimeData->hasText())
    {
        qDebug()<<"hasText"<<mimeData->text();

        QString filePath = QUrl(mimeData->text()).toLocalFile();
        QPixmap pix(filePath);
        QGraphicsPixmapItem * pixmapItem = this->addPixmap(pix);
        pixmapItem->setFlag(QGraphicsItem::ItemIsMovable);
        pixmapItem->setFlag(QGraphicsItem::ItemIsSelectable);
        pixmapItem->setPos(event->pos());
    }
}

void GraphicsSceneMap::focusInEvent(QFocusEvent *event)
{
    QGraphicsScene::focusInEvent(event);
}

void GraphicsSceneMap::focusOutEvent(QFocusEvent *event)
{
    QGraphicsScene::focusOutEvent(event);
}

void GraphicsSceneMap::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
//    qDebug()<<"last pos"<<event->lastPos();
//    qDebug()<<"lastScenePos"<<event->lastScenePos();
//    qDebug()<<"lastScreenPos"<<event->lastScreenPos();
//    qDebug()<<"buttonDownPos"<<event->buttonDownPos(Qt::LeftButton);
//    qDebug()<<"buttonDownScenePos"<<event->buttonDownScenePos(Qt::LeftButton);
//    qDebug()<<"buttonDownScreenPos"<<event->buttonDownScreenPos(Qt::LeftButton);

    if(event->modifiers().testFlag(Qt::ControlModifier))
    {
        QPointF newPos = event->lastScenePos();
        if(event->button() == Qt::LeftButton)
        {
            QRectF pointRect = getPointRect();
            pointRect.moveCenter(newPos);

            if(m_Points.size())
            {
                QLineF lineF(m_Points.last(),newPos );
                QGraphicsLineItem* lineitem = this->addLine(lineF,QPen(Qt::green));
                m_tempItems.push_back(lineitem);
            }
            m_Points.push_back(newPos);
            QGraphicsEllipseItem* item =  this->addEllipse(pointRect,QPen(Qt::green),QBrush(Qt::transparent));
            m_tempItems.push_back(item);
        }

    }
    else if(event->modifiers().testFlag(Qt::NoModifier))
    {
        if(event->button() == Qt::RightButton)
        {
           if(m_Points.size()>2)
                {
                    QLineF lineF(m_Points.last(),m_Points.first());
                    QGraphicsLineItem* lineitem = this->addLine(lineF,QPen(Qt::green));
                    m_tempItems.push_back(lineitem);
                    QPolygonF polygonF(m_Points.toVector());
                    QGraphicsPolygonItem* polygonItem =  this->addPolygon(polygonF,QPen(Qt::green),QBrush(QColor(0,0,255,50)));
                    polygonItem->setFlag(QGraphicsItem::ItemIsMovable);
                    polygonItem->setFlag(QGraphicsItem::ItemIsSelectable);
                    polygonItem->setFocus(Qt::MouseFocusReason);
                    m_listPloygons.push_back(polygonItem);
                    //qDebug()<<"......polygonItem scenePos:"<<polygonItem->scenePos();
                    foreach(QGraphicsItem* item,m_tempItems)
                    this->removeItem(item);
                    m_tempItems.clear();
                    m_Points.clear();
                }
        }
        else if(event->button() == Qt::LeftButton)
        {

            if(m_lastModifyGroup)
            {
                QPointF newPos = event->lastScenePos();
                QList<QGraphicsItem*> items =  this->items(newPos);
                if(items.size()&&m_modifyItems.contains(items.first()))
                {
                    QGraphicsEllipseItem* pointItem = (QGraphicsEllipseItem*)(items.first());
                    m_lastModifyGroup->removeFromGroup(pointItem);
                    pointItem->setBrush(Qt::red);
                }
                else
                {
                    foreach(QGraphicsItem* item,m_modifyItems)
                    {
                        this->removeItem(item);
                    }
                    m_modifyItems.clear();
                    this->destroyItemGroup(m_lastModifyGroup);
                    m_lastModifyGroup = 0;
                }



            }
        }

    }
}

void GraphicsSceneMap::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    this->views().first();
    //QPainter painter;
    //QRectF rect(0,0,60,60);
    //rect.moveCenter(event->lastScenePos());
    //painter.fillRect(rect,QBrush(Qt::red));
    if(event->modifiers().testFlag(Qt::ControlModifier))
    {
        if(m_Points.size())
        {

            QPointF lastPos = m_Points.last();
            QPointF newPos = event->lastScenePos();
            QLineF lineF(lastPos,newPos);
            if(!m_line)
            {
                m_line = this->addLine(lineF,QPen(Qt::yellow));
            }
            else
            {
                m_line->setLine(lineF);
            }
            m_line->setVisible(true);
        }

    }
    else if(event->modifiers().testFlag(Qt::NoModifier))
    {
        if(m_line)    m_line->setVisible(false);

        if(m_lastModifyGroup)
        {
            QList<QGraphicsItem *> items  = this->selectedItems();
            bool bAdjust = false;
            foreach (QGraphicsItem* item, items) {
                if(m_modifyItems.contains(item))
                {
                    bAdjust = true;
                    QVector<QPointF> ptList;
                    QGraphicsPolygonItem* polyGenItem = (QGraphicsPolygonItem*)(m_lastModifyGroup->parentItem());
                    foreach(QGraphicsItem* itemPoint,m_modifyItems)
                    {
                        QGraphicsEllipseItem* elipItem = (QGraphicsEllipseItem*)itemPoint;
                        QPointF pt = elipItem->rect().center();
                        pt = elipItem->mapToScene(pt);
                        ptList.push_back(pt);
                    }

                    QPolygonF polygonF =  polyGenItem->mapFromScene(ptList);
                    polyGenItem->setPolygon(polygonF);
                    break;
                }
            }

            //??
            if(!bAdjust)
            {
                QGraphicsPolygonItem* polyGenItem = (QGraphicsPolygonItem*)(m_lastModifyGroup->parentItem());

            }




        }


    }

}

void GraphicsSceneMap::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);

}

void GraphicsSceneMap::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseDoubleClickEvent(event);

    QPointF newPos = event->lastScenePos();
    if(event->button() == Qt::LeftButton)
    {
        QList<QGraphicsItem *> items =  this->items(newPos,Qt::IntersectsItemShape);
        if(items.size()&&items.first()->type() ==QGraphicsPolygonItem::Type)
        {
            QGraphicsPolygonItem* polygenItem = (QGraphicsPolygonItem*)(items.first());
            qDebug()<<"ori"<<polygenItem->polygon();
            QPolygonF polygenF =  polygenItem->mapToScene(polygenItem->polygon());
            qDebug()<<"map"<<polygenF;
            QGraphicsItemGroup* groupItem = new QGraphicsItemGroup(polygenItem);
             m_lastModifyGroup = groupItem;
            foreach(QPointF pt,polygenF)
            {
                QRectF rectF = getPointRect();
                rectF.moveCenter(pt);
                QGraphicsEllipseItem* pointItem = this->addEllipse(rectF,QPen(Qt::red),QBrush(QColor(0,255,255,60)));
                pointItem->setFlag(QGraphicsItem::ItemIsMovable);
                pointItem->setFlag(QGraphicsItem::ItemIsSelectable);
                pointItem->setFocus(Qt::MouseFocusReason);
                m_modifyItems.push_back(pointItem);
                groupItem->addToGroup(pointItem);
            }
        }

    }

}

void GraphicsSceneMap::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    QGraphicsScene::wheelEvent(event);
}

void GraphicsSceneMap::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter,rect);
}

void GraphicsSceneMap::drawForeground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawForeground(painter,rect);




    //qDebug()<<rect;
//    painter->save();
//    QVector<QLineF> lines;

//    const double xpadding =100.0f;
//    const double ypadding =100.0f;
//    for(double dx = 3.0f;dx<rect.width();dx+=xpadding)
//    {
//       double ddx =  rect.left()+dx;
//       lines<<QLineF(ddx,rect.top(),ddx,rect.bottom());
//    }
//    for(double dy =3.0f;dy<rect.height();dy+=ypadding)
//    {
//       double ddy =  rect.top()+dy;
//       lines<<QLineF(rect.left(),ddy,rect.right(),ddy);
//    }

//    painter->setPen(Qt::gray);

//    painter->drawLines(lines);
//    painter->restore();


}

void GraphicsSceneMap::slotSceneRectChange(const QRectF &rect)
{
    qDebug()<<"slotSceneRectChange:"<<rect;
}

int GraphicsSceneMap::level() const
{
    return m_level;
}

void GraphicsSceneMap::setLevel(int level)
{
    m_level = level;
    QRect rectScene = QRect(0,0,255*(1<<level),255*(1<<level));
    qDebug()<<"level"<<level;
    setSceneRect(rectScene);

}

QRectF GraphicsSceneMap::getPointRect()
{
    int scaled = (1<<m_level);
    QRectF rectF(0,0,20*scaled,20*scaled);
    return rectF;
}




void GraphicsSceneMap::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        if(m_lastModifyGroup && m_modifyItems.size()>3)
        {
            QGraphicsPolygonItem* polyGenItem = (QGraphicsPolygonItem*)(m_lastModifyGroup->parentItem());
            QVector<QPointF> ptList;
            auto itr = m_modifyItems.begin();
            for(;itr != m_modifyItems.end();)
            {
                if((*itr)->isSelected())
                {
                    this->removeItem((*itr));
                    itr = m_modifyItems.erase(itr);
                }
                else
                {
                    QGraphicsEllipseItem* elipItem = (QGraphicsEllipseItem*)(*itr);
                    QPointF pt = elipItem->rect().center();
                    pt = elipItem->mapToScene(pt);
                    ptList.push_back(pt);
                    itr++;
                }
            }

            QPolygonF polygonF =  polyGenItem->mapFromScene(ptList);
            polyGenItem->setPolygon(polygonF);


        }

        QList<QGraphicsItem*> items = this->selectedItems();
        foreach(QGraphicsItem* item,items)
        {
            if(item->isSelected())
            {
                int index = m_listPloygons.indexOf(item);
                if(index != -1)
                {
                    this->removeItem(m_listPloygons.at(index));
                    m_listPloygons.removeAt(index);

                }
            }
        }



    }

}
