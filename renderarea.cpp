#include "renderarea.h"

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent),
      penProposedPath(QPen(Qt::blue)), penAxes(QPen(QColor(193,97,0))),
      penCoveredPath(QPen(QColor(60,196,70), 2)), penCurrPos(QPen(Qt::red, 6)),
      penMeasure(QPen(QColor(151,111,26)))
{
    penCurrPos.setCapStyle(Qt::RoundCap);
}

void RenderArea::setItems(QList<PosItem> itemsRcvd)
{
    items = itemsRcvd;

    listToRender.setCurrFileLine(0);
    listToRender.convertList(items);
    listToRender.updateLivePoint();
    update();
}

void RenderArea::setLivePoint(double x, double y, bool mm)
{
    livePoint.setCoords(x, y, mm);
    listToRender.setLivePoint(livePoint);
    update();
}

void RenderArea::setVisCurrLine(int currLine)
{
    if (listToRender.setCurrFileLine(currLine))
        update();
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    if (!items.size())
        return;

    QSize size = this->size();

    listToRender.rescale(size);

    QPainter painter(this);

    painter.setPen(penProposedPath);
    listToRender.writePath(painter, false);

    painter.setPen(penAxes);
    listToRender.drawAxes(painter);

    painter.setPen(penMeasure);
    listToRender.drawMeasurements(painter);

    painter.setPen(penCoveredPath);
    listToRender.writePath(painter, true);

    //if (!livePoint.isNull()) FIX isNull
    {
        painter.setPen(penCurrPos);
        listToRender.drawPoint(painter, livePoint);
    }
}
