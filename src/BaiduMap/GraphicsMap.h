#ifndef GRAPHICSMAP_H
#define GRAPHICSMAP_H

#include <QObject>
#include <QGraphicsView>
class GraphicsSceneMap;
class QGraphicsTextItem;
class GraphicsMap:public QGraphicsView
{
public:
    GraphicsMap(QWidget *parent = Q_NULLPTR);
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

    // QAbstractScrollArea interface
public:
    void setupViewport(QWidget *viewport);

protected:
    bool viewportEvent(QEvent *event);
    void scrollContentsBy(int dx, int dy);

    // QGraphicsView interface
protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);
private:
    GraphicsSceneMap* m_scene;
    QGraphicsTextItem* m_xyPosition;
};

#endif // GRAPHICSMAP_H
