#include <QPainter>
#include "qsimpletickergraph.h"

namespace
{
    const double DEFAULT_MIN = 0.0;
    const double DEFAULT_MAX = 100.0;
    const double DEFAULT_GRID_PITCH = 10.0;
    const char DEFAULT_LABEL_FONT_FAMILY[] = "Arial";
    const int DEFAULT_LABEL_FONT_SIZE = 12;
    const char DEFAULT_AXIS_FONT_FAMILY[] = "Arial";
    const int DEFAULT_AXIS_FONT_SIZE = 8;
    const Qt::GlobalColor DEFAULT_LABEL_COLOR = Qt::white;
    const int LABEL_MARGIN = 2;
}

/**
* The QSimpleTickerGraph class implements a basic ticker graph, which may be
* useful for visualizing data such as price or temperature as it changes over time.
*/
QSimpleTickerGraph::QSimpleTickerGraph(QWidget *parent) : QWidget(parent),
    mPointWidth(1),
    mDataCount(0),
    mMin(DEFAULT_MIN),
    mMax(DEFAULT_MAX),
    mBackgroundBrush(QBrush(Qt::black)),
    mGridPen(QColor(0, 128, 64)),
    mGridPitch(DEFAULT_GRID_PITCH),
    mDataLinePen(QColor(0, 255, 0)),
    mAxisColor(DEFAULT_LABEL_COLOR),
    mAxisFont(DEFAULT_AXIS_FONT_FAMILY, DEFAULT_AXIS_FONT_SIZE),
    mLabelColor(DEFAULT_LABEL_COLOR),
    mLabelFont(DEFAULT_LABEL_FONT_FAMILY, DEFAULT_LABEL_FONT_SIZE)
{
}

/**
* Paint event handler
*/
void QSimpleTickerGraph::paintEvent(QPaintEvent*)
{
    const double scale = (mMax > mMin) ? (height() / (mMax - mMin)) : 1.0;
    QPainter painter(this);

    // Draw the background
    painter.fillRect(rect(), mBackgroundBrush);

    // If there are any reference points defined, make sure a grid line goes through the first one
    int gridOffset = 0;
    if (!mReferencePoints.isEmpty())
    {
        gridOffset = height() - scale * (mReferencePoints.first() - mMin);
    }

    // Draw the grid lines
    const int pitchInPixels = scale * mGridPitch;
    if (pitchInPixels > 0)
    {
		painter.setPen(mGridPen);
		for (int x = -(mDataCount * mPointWidth) % pitchInPixels; x < width(); x += pitchInPixels)
			painter.drawLine(QPointF(x, 0), QPoint(x, height()));

		for (int y = -pitchInPixels + gridOffset % pitchInPixels; y < height(); y += pitchInPixels)
			painter.drawLine(QPointF(0, y), QPoint(width(), y));
	}

    // Draw the data lines
    painter.setPen(mDataLinePen);
    const int dataWidth = mPointWidth * (mData.size() - 1);
    for (int i = 1; i < mData.size(); ++ i)
    {
        qreal prev = height() - scale * (mData.at(i - 1) - mMin);
        qreal val = height() - scale * (mData.at(i) - mMin);
        qreal x = width() - dataWidth + mPointWidth * i;
        painter.drawLine(QPointF(x - mPointWidth, prev), QPointF(x, val));
    }

    // Draw the min and max values in the left margin
    painter.setPen(mAxisColor);
    painter.setFont(mAxisFont);
    QString minLabel = QString("%1 %2").arg(mMin).arg(mUnits);
    QString maxLabel = QString("%1 %2").arg(mMax).arg(mUnits);
    painter.drawText(rect().adjusted(LABEL_MARGIN, LABEL_MARGIN, -LABEL_MARGIN, -LABEL_MARGIN), Qt::AlignTop | Qt::AlignLeft, maxLabel);
    painter.drawText(rect().adjusted(LABEL_MARGIN, LABEL_MARGIN, -LABEL_MARGIN, -LABEL_MARGIN), Qt::AlignBottom | Qt::AlignLeft, minLabel);

    // Draw the reference labels, if any
    const int axisLabelHeight = QFontMetrics(mAxisFont).height();
    foreach(double point, mReferencePoints)
    {
        double y = height() - scale * (point - mMin);

        QString label = QString("%1 %2").arg(point).arg(mUnits);
        painter.drawText(QRect(LABEL_MARGIN, y - axisLabelHeight/2, width() - LABEL_MARGIN, axisLabelHeight), Qt::AlignVCenter | Qt::AlignLeft, label);
    }

    // Draw the current value as text
    if (!mData.isEmpty())
    {
        QString current = QString::fromUtf8("%1 %2")
                          .arg(mData.last(), 3, 'f', 3)
                          .arg(mUnits);
        painter.setPen(mLabelColor);
        painter.setFont(mLabelFont);
        painter.drawText(rect().adjusted(LABEL_MARGIN, LABEL_MARGIN, -LABEL_MARGIN, -LABEL_MARGIN), Qt::AlignTop | Qt::AlignRight, current);
    }
}

/**
* The units to be displayed along with the current value
*/
QString QSimpleTickerGraph::units() const
{
    return mUnits;
}

/**
* Specifies the units to be displayed along with the current value.
* For example, if the graph is to display voltage:
*     graph->setUnits("V")
*/
void QSimpleTickerGraph::setUnits(const QString& units)
{
    if (units != mUnits)
    {
        mUnits = units;
        if (!mData.isEmpty())
            update();
    }
}

/**
* The range of the data that the graph is meant to display.
* The default range is [0, 100]
*/
QPair<double, double> QSimpleTickerGraph::range() const
{
    return QPair<double, double>(mMin, mMax);
}

/**
* Sets the range of data that this graph is meant to display.
* This is used to scale the graph appropriately. For instance,
* if one executes
*    graph->setRange(0, 100)
* and then executes
*    graph->addPoint(100)
* then the added point will appear at top of the visibile graph.
* The default range is [0, 100] if no other range is specified.
*/
void QSimpleTickerGraph::setRange(double min, double max)
{
    if ((min != mMin || max != mMax) && (mMax > mMin))
    {
        mMin = min;
        mMax = max;
        if (!mData.isEmpty())
            update();
    }
}

/**
* Sets the range of data that this graph is meant to display.
*/
void QSimpleTickerGraph::setRange(const QPair<double, double>& range)
{
    setRange(range.first, range.second);
}

/**
* The horizontal distance in pixels between consecutive data points.
*/
int QSimpleTickerGraph::pointWidth() const
{
    return mPointWidth;
}

/**
* Sets the horizontal distance in pixels between consecutive data points.
*/
void QSimpleTickerGraph::setPointWidth(int w)
{
    if (w != mPointWidth)
    {
        mPointWidth = w;
        if (!mData.isEmpty())
            update();
    }
}

/**
* A list of levels, aside from the min and max, that should be labelled on the graph.
*/
QList<double> QSimpleTickerGraph::referencePoints() const
{
    return mReferencePoints;
}

/**
* Sets the list of levels, aside from the min and max, that should be labelled on the graph.
*/
void QSimpleTickerGraph::setReferencePoints(const QList<double>& points)
{
    if (points != mReferencePoints)
    {
        mReferencePoints = points;
        update();
    }
}

/**
* The brush used to fill the background of the graph.
*/
QBrush QSimpleTickerGraph::backgroundBrush() const
{
    return mBackgroundBrush;
}

/**
* Sets the brush used to fill the background of the graph.
* The default brush is a solid fill black brush.
*/
void QSimpleTickerGraph::setBackgroundBrush(const QBrush& brush)
{
    if (brush != mBackgroundBrush)
    {
        mBackgroundBrush = brush;
        update();
    }
}

/**
* The pen used to draw the grid lines.
*/
QPen QSimpleTickerGraph::gridPen() const
{
    return mGridPen;
}

/**
* Sets the pen used to draw the grid lines.
* The default pen is a single pixel wide and green.
*/
void QSimpleTickerGraph::setGridPen(const QPen& pen)
{
    if (pen != mGridPen)
    {
        mGridPen = pen;
        update();
    }
}

/**
* The spacing between adjacent horizontal or vertical grid lines,
* in the same units as the data values. The default is 10.
*/
double QSimpleTickerGraph::gridPitch() const
{
    return mGridPitch;
}

/**
* Sets the spacing between adjacent horizontal or vertical grid lines,
* in the same units as the data values. The default is 10.
*/
void QSimpleTickerGraph::setGridPitch(double pitch)
{
    if (pitch != mGridPitch)
    {
        mGridPitch = pitch;
        update();
    }
}

/**
* The pen used for drawing grid lines on the graph.
*/
QPen QSimpleTickerGraph::dataLinePen() const
{
    return mDataLinePen;
}

/**
* Sets the pen used for drawing the data lines on the graph.
* The default pen is a single pixel wide and green.
*/
void QSimpleTickerGraph::setDataLinePen(const QPen& pen)
{
    if (pen != mDataLinePen)
    {
        mDataLinePen = pen;
        if (!mData.isEmpty())
            update();
    }
}

/**
* The color for the value label.
*/
QColor QSimpleTickerGraph::labelColor() const
{
    return mLabelColor;
}

/**
* Sets the color for the value label. The default color is white.
*/
void QSimpleTickerGraph::setLabelColor(const QColor& color)
{
    if (color != mLabelColor)
    {
        mLabelColor = color;
        update();
    }
}

/**
* The font used for drawing the current value label.
*/
QFont QSimpleTickerGraph::labelFont() const
{
    return mLabelFont;
}

/**
* Sets the font used for drawing the current value label at
* the top left corner of the graph.
* The default font is 12pt Arial.
*/
void QSimpleTickerGraph::setLabelFont(const QFont& font)
{
    if (font != mLabelFont)
    {
        mLabelFont = font;
        if (!mData.isEmpty())
            update();
    }
}

/**
* The color used to draw the min and max value labels on the vertical axis.
*/
QColor QSimpleTickerGraph::axisColor() const
{
    return mAxisColor;
}

/**
* Sets the color used for the min and max value labels on the vertical
* axis. The default color used is white.
*/
void QSimpleTickerGraph::setAxisColor(const QColor& color)
{
    if (color != mAxisColor)
    {
        mAxisColor = color;
        update();
    }
}

/**
* The font used for the min and max value labels on the vertical axis.
*/
QFont QSimpleTickerGraph::axisFont() const
{
    return mAxisFont;
}

/**
* Sets the font used for the min and max value labels on the vertical axis.
* The default font is 8pt Arial.
*/
void QSimpleTickerGraph::setAxisFont(const QFont& font)
{
    if (font != mAxisFont)
    {
        mAxisFont = font;
        update();
    }
}

/**
* Appends a new data point to the graph, and redraws the graph with
* the new point added to the right edge.
*/
void QSimpleTickerGraph::appendPoint(double point)
{
    mData.enqueue(point);
    const int dataWidth = mPointWidth * (mData.size() - 1);
    if (dataWidth >= (width() + mPointWidth))
        mData.dequeue();
    ++ mDataCount;

    update();
}

/**
* Clears all of the data from the graph and redraws the graph with no data
*/
void QSimpleTickerGraph::clear()
{
    mData.clear();
    mDataCount = 0;
    update();
}

