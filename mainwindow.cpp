#include <QtCore/QTimer>

#include "qsimpletickergraph.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

const int GRAPH_UPDATE_PERIOD = 100;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mTimer(new QTimer(this)),
    mVoltage(100.0),
    mTemperature(20.0),
    mSpeed(100.0)
{
    mUi->setupUi(this);

    // Configure the graphs. The voltage graph is an example of a graph
    // that has been heavily customized, whereas the temperature graph
    // is hardly customized at all.
    mUi->voltageGraph->setUnits("V");
    mUi->voltageGraph->setRange(-100, 200);
    mUi->voltageGraph->setDataLinePen(QPen(QColor(0,0,0), 2));
    mUi->voltageGraph->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
    mUi->voltageGraph->setGridPitch(50);
    mUi->voltageGraph->setGridPen(QPen(QColor(32, 32, 32), 1, Qt::DotLine));
    mUi->voltageGraph->setAxisColor(QColor(32, 32, 32));
    mUi->voltageGraph->setLabelColor(QColor(32, 32, 32));
    mUi->voltageGraph->setPointWidth(5);

    mUi->voltageGraph->setReferencePoints(QList<double>() << 0);

    mUi->temperatureGraph->setUnits("\u00B0C");
    mUi->temperatureGraph->setRange(10, 30);
    mUi->temperatureGraph->setReferencePoints(QList<double>() << 15);

    mUi->speedGraph->setUnits("km/h");
    mUi->speedGraph->setRange(0, 200);
    mUi->speedGraph->setPointWidth(10);
    mUi->speedGraph->setBackgroundBrush(QBrush(QColor(0, 32, 128)));
    mUi->speedGraph->setDataLinePen(QPen(QColor(32, 0, 0), 5));
    mUi->speedGraph->setGridPitch(35);

    // Periodically update the graphs with new data points
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    mTimer->start(GRAPH_UPDATE_PERIOD);
}

void MainWindow::timeout()
{
    mVoltage += 9.0 - double(qrand() % 1000) / 50.0 + 0.02 * (100.0 - mVoltage);
    mTemperature += 0.5 - double(qrand() % 1000) / 1000.0 + 0.02 * (20.0 - mTemperature);
    mSpeed += 4.9 - double(qrand() % 1000) / 100.0 + 0.02 * (100.0 - mSpeed + 0.3 * mVoltage);

    mUi->voltageGraph->appendPoint(mVoltage);
    mUi->temperatureGraph->appendPoint(mTemperature);
    mUi->speedGraph->appendPoint(mSpeed);
}

