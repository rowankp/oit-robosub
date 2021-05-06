//******************************************************************************
// filename: HelpWindow.cpp
// authors: Jay Sucharitakul (jay.sucharitakul@oit.edu)
// last edited: 05/03/2020
#include "HelpWindow.h"
#include "ui_helpwindow.h"
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QPalette>

//******************************************************************************
// purpose: constructor
// paremeters: QWidget *
// return: none
HelpWindow::HelpWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpwindow)
{
    ui->setupUi(this);

    // connect buttons to gui
    connect(ui->drag_drop, SIGNAL(released()), this, SLOT(handleDragDrop()));
    connect(ui->parameters, SIGNAL(released()), this, SLOT(handleParameters()));
}

//******************************************************************************
// purpose: deconstructor
// paremeters: none
// return: none
HelpWindow::~HelpWindow()
{
    delete ui;
}

//******************************************************************************
// purpose: button handler for drag and drop
// paremeters: none
// return: none
void HelpWindow::handleDragDrop()
{
    QFile file("..\\MissionControl\\mission\\mission.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString content = stream.readAll();
    file.close();
    // change the text
    ui->textEdit->clear();
    ui->textEdit->append(content);
}

//******************************************************************************
// purpose: button handler for parameters
// paremeters: none
// return: none
void HelpWindow::handleParameters()
{
    QFile file("..\\MissionControl\\mission\\tasks-supported.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString content = stream.readAll();
    file.close();
    // change the text
    ui->textEdit->clear();
    ui->textEdit->append(content);
}
