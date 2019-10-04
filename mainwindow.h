#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QTimer;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

protected slots:
    // Periodically updates the graph
    void timeout();

private:
    Ui::MainWindow *mUi;
    QTimer* mTimer;

    double mVoltage;
    double mTemperature;
    double mSpeed;
};

#endif // MAINWINDOW_H
