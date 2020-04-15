#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QTimer>
#include <QTime>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void telemetryDataTable();

    void showElapsedTime();

    void makePlot();

    void receivedPacket();

    void lostPacket();

    void stateLabel();

    void on_connectButton_clicked();

    void on_disconnectButton_clicked();

    void on_pauseDataRecordingButton_clicked();

    void on_stopDataRecordingButton_clicked();

    void on_startDataRecordingButton_clicked();
private:

    Ui::MainWindow *ui;

    QStandardItemModel *csvModel;

    QTimer *timer=new QTimer(this);
    QTime t;

    qint64 totalNumberOfSeconds;
    qint64 tempTotalNumberOfSeconds;
    qint64 minutes;
    qint64 seconds;
    qint64 milliseconds;

    QList<QString> column1;
    QList<QString> column2;
    QList<QString> column3;
    QList<QString> column4;
    QList<QString> column5;
    QList<QString> column6;
    QList<QString> column7;
    int tempColumnSize;

    bool IsStartDataRecordingButtonEnabled;
};

#endif // MAINWINDOW_H
