//******************************************************************************
// filename: AboutWindow.cpp
// authors: Jay Sucharitakul (jay.sucharitakul@oit.edu)
// last edited: 05/03/2020


#include "AboutWindow.h"
#include "ui_aboutwindow.h"
#include <QGraphicsScene>
#include <QImageReader>
#include <QMessageBox>
#include <QDir>
#include <QScrollArea>

//******************************************************************************
// purpose: constructor
// paremeters: QWidget *
// return: none
AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    ui->people->setText("The purpose of this project is to create "
                        "both a computer vision library for future students at the Oregon Institute "
                        "of Technology and use this library to control an autonomous robotic submarine "
                        "that will be submitted in a competition. The computer vision library will give "
                        "users access to our existing code-base of image recognition methods, allowing "
                        "the user to create their own computer vision projects with ease. "
                        "For the autonomous submarine, it must be able to perform various "
                        "tasks through communication between hardware and software and without human "
                        "interference. The tasks it must perform include locating a 6’ by 5’ metal gate "
                        "of a certain color which will be announced before the competition, move through "
                        "that gate after locating it, circle the right side of the gate, and move back "
                        "to its original position, marked by a marker. If the submarine is able to complete "
                        "the aforementioned tasks, additional tasks may include recognizing patterns and "
                        "firing a torpedo at a moving target.");

    ui->people->setAlignment(Qt::AlignCenter);
    QPixmap pm("..\\MissionControl\\about\\team.png"); // <- path to image file
    ui->picture->setPixmap(pm);
    ui->picture->setScaledContents(true);
}

//******************************************************************************
// purpose: deconstructor
// paremeters: none
// return: none
AboutWindow::~AboutWindow()
{
    delete ui;
}
