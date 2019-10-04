#ifndef Q_SIMPLE_TICKET_GRAPH_H
#define Q_SIMPLE_TICKET_GRAPH_H

#include <QPair>
#include <QQueue>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

class QSimpleTickerGraph : public QWidget
{
    Q_OBJECT

public:
    explicit QSimpleTickerGraph(QWidget *parent = 0);
    void paintEvent(QPaintEvent* pEvent);

    // Data-related parameters
    QString units() const;
    void setUnits(const QString& unit);

    QPair<double, double> range() const;
    void setRange(const QPair<double, double>& p);
    void setRange(double min, double max);

    int pointWidth() const;
    void setPointWidth(int w);

    QList<double> referencePoints() const;
    void setReferencePoints(const QList<double>& points);

    // Style-related parameters
    QBrush backgroundBrush() const;
    void setBackgroundBrush(const QBrush& brush);

    QPen gridPen() const;
    void setGridPen(const QPen& pen);

    double gridPitch() const;
    void setGridPitch(double pitch);

    QPen dataLinePen() const;
    void setDataLinePen(const QPen& pen);

    QColor axisColor() const;
    void setAxisColor(const QColor& color);

    QFont axisFont() const;
    void setAxisFont(const QFont& font);

    QColor labelColor() const;
    void setLabelColor(const QColor& color);

    QFont labelFont() const;
    void setLabelFont(const QFont& font);

    // Adding/clearing data points
    void appendPoint(double point);
    void clear();

private:
    // Data points
    QQueue<double> mData;
    int mPointWidth;
    int mDataCount;
    QString mUnits;

    // Reference points
    QList<double> mReferencePoints;

    // Data range visible in graph
    double mMin;
    double mMax;

    // Style
    QBrush mBackgroundBrush;
    QPen mGridPen;
    double mGridPitch;
    QPen mDataLinePen;
    QColor mAxisColor;
    QFont mAxisFont;
    QColor mLabelColor;
    QFont mLabelFont;
};

#endif // Q_SIMPLE_TICKET_GRAPH_H
