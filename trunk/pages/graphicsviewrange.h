/***************************************************************************
**                                                                        **
**  Polyphone, a soundfont editor                                         **
**  Copyright (C) 2013-2015 Davy Triponney                                **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Davy Triponney                                       **
**  Website/Contact: http://www.polyphone.fr/                             **
**             Date: 01.01.2013                                           **
***************************************************************************/

#ifndef GRAPHICSVIEWRANGE_H
#define GRAPHICSVIEWRANGE_H

#include <QGraphicsView>
#include "pile_sf2.h"
#include "mainwindow.h"
class GraphicsSimpleTextItem;
class GraphicsRectangleItem;
class GraphicsLegendItem;
class GraphicsZoomLine;

class GraphicsViewRange : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphicsViewRange(QWidget *parent = 0);
    ~GraphicsViewRange();

    void init(Pile_sf2 * sf2, MainWindow * mainWindow);
    void display(EltID id);

signals:
    void updateKeyboard();

protected:
    void resizeEvent(QResizeEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event);
    void mouseDoubleClickEvent(QMouseEvent *) {}
    void scrollContentsBy(int dx, int dy);

private:
    void initItems();
    void updateLabels();
    void setCurrentRectangles(QList<GraphicsRectangleItem*> rectanglesToSelect);
    QRectF getCurrentRect();

    Pile_sf2 * _sf2;
    MainWindow * _mainWindow;
    QGraphicsScene * _scene;
    QList<GraphicsRectangleItem *> _rectangles;
    QList<GraphicsRectangleItem *> _currentRectangles;
    QList<GraphicsSimpleTextItem *> _leftLabels, _bottomLabels;
    GraphicsLegendItem * _legendItem;
    GraphicsZoomLine * _zoomLine;
    bool _dontRememberScroll;

    // Drag & zoom
    enum MouseMode
    {
        MOUSE_MODE_NONE,
        MOUSE_MODE_MOVE_RECTANGLE,
        MOUSE_MODE_MOVE_RIGHT,
        MOUSE_MODE_MOVE_LEFT,
        MOUSE_MODE_MOVE_TOP,
        MOUSE_MODE_MOVE_BOTTOM
    };
    Qt::MouseButton _buttonPressed;
    bool _moveOccured;
    MouseMode _mouseMode;
    double _xInit, _yInit;
    double _zoomX, _zoomY, _posX, _posY;
    double _zoomXinit, _zoomYinit, _posXinit, _posYinit;
    QRectF _displayedRect;
    void zoom(QPoint point);
    void drag(QPoint point);
    void zoomDrag();
    void setZoomLine(double x1Norm, double y1Norm, double x2Norm, double y2Norm);
    double normalizeX(int xPixel);
    double normalizeY(int yPixel);

    static const double WIDTH;
    static const double MARGIN;
    static const double OFFSET;
};

#endif // GRAPHICSVIEWRANGE_H
