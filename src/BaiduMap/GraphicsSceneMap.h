#ifndef GRAPHICSSCENEMAP_H
#define GRAPHICSSCENEMAP_H

#include <QGraphicsScene>
class QGraphicsItem;
class GraphicsSceneMap:public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsSceneMap(QObject *parent = Q_NULLPTR);

    // QGraphicsScene interface
    int level() const;
    void setLevel(int level);
    QRectF getPointRect();
protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);
private slots:
    void slotSceneRectChange(const QRectF& rect);
private:

    QList<QPointF> m_Points;
    QList<QGraphicsItem*> m_tempItems;
    QGraphicsLineItem* m_line;
    QList<QGraphicsItem*> m_modifyItems;
    QGraphicsItemGroup* m_lastModifyGroup;
    int m_level;
    QList<QGraphicsItem*> m_listPloygons;
    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // GRAPHICSSCENEMAP_H
