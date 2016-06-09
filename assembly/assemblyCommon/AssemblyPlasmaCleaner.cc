#include <vector>

#include <QFormLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QLabel>
#include <QApplication>
#include <QProgressBar>
#include <sstream>
#include <iostream>
#include <nqlogger.h>

#include "AssemblyPlasmaCleaner.h"

//relay card
#include "../../devices/Conrad/ConradCommunication.h"
#include "../../devices/Conrad/ConradController.h"


using namespace std;

AssemblyPlasmaCleaner::AssemblyPlasmaCleaner(QWidget *parent)
    : QWidget(parent)
{

 
  
//   bool comm = motor->IsCommunication();
//   cout <<"comms on?"<<  comm<<"   "   <<endl;
 


    //    for (int t = 0; t < 20; t++){

      //      cout << " pressure A " << motor->GetPressureA()  << endl; 
    //}



      
    QGridLayout *l = new QGridLayout(this);
    setLayout(l);

    QGridLayout *g0 = new QGridLayout(this);
    l->addLayout(g0,0,0);
    

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(220, 220, 220));
    
    QPushButton *btn1 = new QPushButton(this);
    btn1->setText("Upload Firmware");
    g0->addWidget(btn1,0,0);

    CleanerCommander * cmdr1 = new CleanerCommander(this, "Scan (speed,distance)", 100.0,100.0);
    g0->addWidget(cmdr1,0,1);

    //ql = new QLabel("", this);
    //g0->addWidget(ql,0,1);
    
    //    QPixmap pixmap(100,100);
    //pixmap.fill(QColor("transparent"));
    
    //QPainter painter(&pixmap);
    //painter.setBrush(QBrush(Qt::red));
    //painter.drawEllipse(0, 0, 30, 30);
    
    //ql->setPixmap(pixmap);
    //ql->setAlignment(Qt::AlignCenter);
    //ql->setText("READY");
    //ql->setStyleSheet("QLabel { background-color : orange; color : black; }");
     

    QProgressBar *progressBar = new QProgressBar();
    progressBar->setMinimum(0);
    progressBar->setMaximum(99);
    progressBar->setValue(20);
    progressBar->setTextVisible(true);
    //progressBar->show();
    g0->addWidget(progressBar,0,2);
}

CleanerCommander::CleanerCommander(QWidget *parent, std::string string, double x ,double y)
: QWidget(parent), local_x(x), local_y(y)
{
     QFormLayout *l = new QFormLayout(this);
     setLayout(l);

    NQLog("CleanerCommander") << ": in mode";
    motor = new ArduinoMotor("/dev/ttyACM1");

    std::ostringstream strs;
    strs.clear();
    strs << x;
    strs << ",";
    strs << y;

    str = strs.str();

    std::cout << "[CleanerCommander]."   << str <<std::endl;

    QString qstr = QString::fromStdString(str);
    QString qname = QString::fromStdString(string);
    
    this->local_x = x;
    this->local_y = y;
    
    button1 = new QPushButton(qname, this);

    lineEdit1 = new QLineEdit();
    lineEdit1->setText(qstr);
    l->addRow(button1,lineEdit1);



    connect(button1, SIGNAL(clicked()),
          this, SLOT(sendCommand()));
    
    
}


void CleanerCommander::sendCommand(){
std::cout << "[CleanerCommander::sendCommand]."   << str <<std::endl;

//    stringstream convert;
//   convert << steps;
//   const char* cmd = convert.str().c_str();
    motor->SendCommand("100");

}





    
  







