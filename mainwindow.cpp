#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QLineSeries>
#include <QChart>
#include <QChartView>

using namespace QtCharts;

//Main Method to show main window
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Initiates logo and displays creating QPixmap
    QPixmap logo(":/logo/img/CanSat-GRIFON.jpg");
    int w= ui->logoLabel ->width();
    int h= ui->logoLabel ->height();
    ui->logoLabel->setPixmap(logo.scaled(w,h,Qt::KeepAspectRatio));

    //Initiaties values to the defaults;
    tempTotalNumberOfSeconds=0;
    tempColumnSize=0;
    IsStartDataRecordingButtonEnabled=false;
}

//Destructor method
MainWindow::~MainWindow()
{
    delete ui;
}

//Method to create and display a table of received telemetry data
void MainWindow::telemetryDataTable (){

    //Creates a data model for the mapping table from a CSV file
    csvModel = new QStandardItemModel(this);
    csvModel->setColumnCount(17);
    csvModel->setHorizontalHeaderLabels(QStringList() << "TEAM ID" << "MISSION TIME" << "PACKET COUNT" << "ALTITUDE" <<"PRESSURE" <<"TEMPERATURE" <<"VOLTAGE" <<"GPS TIME" <<"GPS LATITUDE" <<"GPS LONGITUDE" <<"GPS ALTITUDE" <<"GPS SATS" <<"PITCH" <<"ROLL" <<"BLADE SPIN RATE" <<"SOFTWARE STATE" <<"BONUS DIRECTION");
    ui->csvTableView->setModel(csvModel);

    //Opens the file by specifying its full the path
    QFile file("F:/test.csv");

    //Gives a warning if the file does not exist
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    }

    else {
        //Removes all elements inside of the column lists
        column1.clear ();
        column2.clear ();
        column3.clear ();
        column4.clear ();
        column5.clear ();
        column6.clear ();

        //Creates a thread to retrieve data from a file
        QTextStream in(&file);
        //Counts every item to determine columns
        int countItem=0;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            // Adds to the model in line with the elements
            QList<QStandardItem *> standardItemsList;
            //Seperates the line by semicolons into columns
            for (QString item : line.split(",")) {

                switch(countItem%17){
                case 1:
                    //Mission Time column
                    column6.append (item);
                    break;
                case 2:
                    //Packet Count column
                    column7.append (item);
                    break;
                case 3:
                    //Altitude Column
                    column1.append (item);
                    break;
                case 4:
                    //Pressure Column
                    column2.append (item);
                    break;
                case 5:
                    //Temperature Column
                    column3.append (item);
                    break;
                case 6:
                    //Voltage Column
                    column4.append (item);
                    break;
                case 15:
                    //Software State Column
                    column5.append (item);
                    break;
                default:
                    break;
                }
                countItem++;
                //Adds items into a list
                standardItemsList.append(new QStandardItem(item));
            }
            countItem = 0;
            //Writes list into a tabel
            csvModel->insertRow(csvModel->rowCount(), standardItemsList);
        }
        file.close();
    }

    //Plots chart if new row is added in the telemetry
    if(column4.size()>tempColumnSize) {
        makePlot();
        tempColumnSize=column4.size();
    }

    stateLabel();
    receivedPacket ();
    lostPacket ();
}

//Method to show elapsed time
void MainWindow::showElapsedTime()
{
    //Elapsed time is not zero if data recording is paused and started again
    if(tempTotalNumberOfSeconds!=0){
        totalNumberOfSeconds=tempTotalNumberOfSeconds+t.elapsed ();
        minutes = (totalNumberOfSeconds / 1000 / 60);
        seconds = (totalNumberOfSeconds / 1000) % 60;
        milliseconds = totalNumberOfSeconds % 100;
    }
    //Elapsed time is zero
    else{
        totalNumberOfSeconds=t.elapsed ();
        minutes = (totalNumberOfSeconds / 1000 / 60);
        seconds = (totalNumberOfSeconds / 1000) % 60;
        milliseconds = totalNumberOfSeconds % 100;
    }
    //Casts integer values into string and displays elapsed time on a label
    QString timeString = QString("%1:%2:%3")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'))
            .arg(milliseconds, 2, 10, QChar('0'));
    ui->missionTimeLabel_2->setText (timeString);
}

//Method to plot realtime chart
void MainWindow::makePlot(){

    //Creates a line serie which appends mission time and altitude data
    QLineSeries *series1 = new QLineSeries();
    for (int i=0; i<column1.size (); i++) {
        series1->append(column6.at(i).toInt (), column1.at(i).toInt ());
    }
    //Creates and plots a chart which adds already created serie
    QChart *chart1 = new QChart();
    chart1->legend()->hide();
    chart1->addSeries(series1);
    chart1->createDefaultAxes();
    chart1->setTitle("Altitude over Time");
    QChartView *chartView1 = new QChartView(chart1);
    chartView1->setRenderHint(QPainter::Antialiasing);
    ui->plot1->setChart (chart1);

    //Creates a line serie which appends mission time and pressure data
    QLineSeries *series2 = new QLineSeries();
    for (int i=0; i<column2.size (); i++) {
       series2->append(column6.at(i).toInt (), column2.at(i).toInt ());
    }
    QChart *chart2 = new QChart();
    chart2->legend()->hide();
    chart2->addSeries(series2);
    chart2->createDefaultAxes();
    chart2->setTitle("Pressure over Time");
    QChartView *chartView2 = new QChartView(chart2);
    chartView2->setRenderHint(QPainter::Antialiasing);
    ui->plot2->setChart (chart2);

    //Creates a line serie which appends mission time and temperature data
    QLineSeries *series3 = new QLineSeries();
    for (int i=0; i<column3.size (); i++) {
          series3->append(column6.at(i).toInt (), column3.at(i).toInt ());
    }
    //Creates and plots a chart which adds already created serie
    QChart *chart3 = new QChart();
    chart3->legend()->hide();
    chart3->addSeries(series3);
    chart3->createDefaultAxes();
    chart3->setTitle("Temperature over Time");
    QChartView *chartView3 = new QChartView(chart3);
    chartView3->setRenderHint(QPainter::Antialiasing);
    ui->plot3->setChart (chart3);

    QLineSeries *series4 = new QLineSeries();
    for (int i=0; i<column4.size (); i++) {
          series4->append(column6.at(i).toInt (), column4.at(i).toInt ());
    }
    //Creates and plots a chart which adds already created serie
    QChart *chart4 = new QChart();
    chart4->legend()->hide();
    chart4->addSeries(series4);
    chart4->createDefaultAxes();
    chart4->setTitle("Voltage over Time");
    QChartView *chartView4 = new QChartView(chart4);
    chartView4->setRenderHint(QPainter::Antialiasing);
    ui->plot4->setChart (chart4);
}

//Method to show telemetry's received packet number
void MainWindow::receivedPacket(){
    if(column4.size ()>0){
        ui -> packetsReceivedLcdNumber ->display(column7.at (column7.size ()-1));
    }
    else{
        ui -> packetsReceivedLcdNumber ->display(0);
    }
}

//Method to show telemetry's lost packet number
void MainWindow::lostPacket(){
    ui -> packetsLostLcdNumber ->display(34);
}

//Method to show CanSat's state label
void MainWindow::stateLabel(){
    QString label=column5.at (column5.size ()-1);
    ui->stateLabel->setText(label);
}

//Listener method of Connect button
void MainWindow::on_connectButton_clicked()
{
    ui->connectionLabel->setText("Connected");
}

//Listener method of Disconnect button
void MainWindow::on_disconnectButton_clicked()
{

    ui->connectionLabel->setText("Disconnected");
}

//Listener method of Start Data Recording button
void MainWindow::on_startDataRecordingButton_clicked()
{
    //Methods only works if Start Data Recording button is enabled
    if(IsStartDataRecordingButtonEnabled==false){
        connect (timer, SIGNAL(timeout()),this,SLOT(telemetryDataTable ()));
        timer->start (1000);

        t.start ();
        connect (timer, SIGNAL(timeout()),this,SLOT(showElapsedTime()));
        timer->start (0);
        IsStartDataRecordingButtonEnabled=true;
    }
}

//Listener method of Pause Data Recording button
void MainWindow::on_pauseDataRecordingButton_clicked()
{
    //Stops the timer and saves elapsed time in a temporary value
    tempTotalNumberOfSeconds=totalNumberOfSeconds;
    if(timer->isActive ()==true){
        timer->stop ();
    }
    //Enables clicking Start Data Recording button
    IsStartDataRecordingButtonEnabled=false;
}

//Listener method of Stop Data Recording button
void MainWindow::on_stopDataRecordingButton_clicked()
{
    //Stops the timer and resets both elapsed time and temporary elapsed time
    tempTotalNumberOfSeconds=0;
    totalNumberOfSeconds=0;
    if(timer->isActive ()==true){
        timer->stop ();
    }
    //Enables clicking Start Data Recording button
    IsStartDataRecordingButtonEnabled=false;
}

