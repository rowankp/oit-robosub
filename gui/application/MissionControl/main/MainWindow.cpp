//******************************************************************************
// filename: MainWindow.cpp
// authors: Rowan Parker (rowan.parker@oit.edu)
// date: 02/17/2020

#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "help/HelpWindow.h"
#include "about/AboutWindow.h"
#include "main/ConfirmMissionDialog.h"
#include "main/CancelMissionDialog.h"
#include "main/MissionInvalidDialog.h"
#include "main/UnsupportedDialog.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QModelIndex>
#include <QtCore/QTextStream>
#include <QtCore/QProcess>

//******************************************************************************
// purpose: constructor. sets up the application environment.
// parameters: QWidget *
// return: n/a
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), _ui(new Ui::MainWindow)
{
    // setup main window
    _ui->setupUi(this);

    // call additional setup methods
    this->initActionsAvailableModel();
    this->initColorButtons();
    this->initShapeButtons();

    // connect drag-drop lists
    connect(_ui->task_selected_list, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(removeTask(QListWidgetItem *)));
    connect(_ui->task_options_list, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(addTask(QListWidgetItem *)));

    // connect color widgets
    connect(_ui->target_blue_button, SIGNAL(emitText(const QString&)), this, SLOT(setTargetColor(const QString &)));
    connect(_ui->target_black_button, SIGNAL(emitText(const QString&)), this, SLOT(setTargetColor(const QString &)));
    connect(_ui->target_green_button, SIGNAL(emitText(const QString&)), this, SLOT(setTargetColor(const QString &)));
    connect(_ui->target_red_button, SIGNAL(emitText(const QString&)), this, SLOT(setTargetColor(const QString &)));
    connect(_ui->target_white_button, SIGNAL(emitText(const QString&)), this, SLOT(setTargetColor(const QString &)));
    connect(_ui->target_yellow_button, SIGNAL(emitText(const QString&)), this, SLOT(setTargetColor(const QString &)));

    // connect shape widgets
    connect(_ui->target_buoy_button, SIGNAL(emitText(const QString&)), this, SLOT(setTargetShape(const QString &)));
    connect(_ui->target_buoy_button, SIGNAL(released()), this, SLOT(showDimensions()));
    connect(_ui->target_circle_button, SIGNAL(emitText(const QString&)), this, SLOT(setTargetShape(const QString &)));
    connect(_ui->target_circle_button, SIGNAL(released()), this, SLOT(showDimensions()));
    connect(_ui->target_gate_button, SIGNAL(emitText(const QString&)), this, SLOT(setTargetShape(const QString &)));
    connect(_ui->target_gate_button, SIGNAL(released()), this, SLOT(showDimensions()));
    connect(_ui->target_triangle_button, SIGNAL(emitText(const QString&)), this, SLOT(setTargetShape(const QString &)));
    connect(_ui->target_triangle_button, SIGNAL(released()), this, SLOT(showDimensions()));
    connect(_ui->target_rectangle_button, SIGNAL(emitText(const QString&)), this, SLOT(setTargetShape(const QString &)));
    connect(_ui->target_rectangle_button, SIGNAL(released()), this, SLOT(showDimensions()));

    // connect subby button
    connect(_ui->subby_button, SIGNAL(emitJoke(const QString &)), _ui->subby_joke, SLOT(setText(const QString &)));
    connect(_ui->subby_button, SIGNAL(hideJoke()), _ui->subby_joke, SLOT(clear()));

    // connect help page
    connect(_ui->help_menuItem, SIGNAL(aboutToShow()), this, SLOT(help()));

    // connect about page
    connect(_ui->about_menuItem, SIGNAL(aboutToShow()), this, SLOT(about()));

    // connect confirm mission dialog
    connect(_ui->confirm_mission_button, SIGNAL(released()), this, SLOT(confirmMission()));

    // connect cancel mission dialog
    connect(_ui->cancel_mission_button, SIGNAL(released()), this, SLOT(cancelMission()));

    // connect dimensions box to unsupported dialog
    connect(_ui->target_radius_lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(showUnsupported()));
    connect(_ui->target_radius_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showUnsupported()));
    connect(_ui->target_length_lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(showUnsupported()));
    connect(_ui->target_length_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showUnsupported()));
    connect(_ui->target_width_lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(showUnsupported()));
    connect(_ui->target_width_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showUnsupported()));
}

//******************************************************************************
// purpose: destructor. releases allocated memory.
// parameters: none
// return: n/a
MainWindow::~MainWindow()
{
    delete _ui;
}

//******************************************************************************
// purpose: opens up the help page under helpwindow.ui
// parameters: none
// return: none
void MainWindow::help()
{
    HelpWindow help;
    help.setModal(true);
    help.exec();
}

//******************************************************************************
// purpose: opens up the about page under helpwindow.ui
// parameters: none
// return: none
void MainWindow::about()
{
    AboutWindow about;
    about.setModal(true);
    about.exec();
}

//******************************************************************************
// purpose: opens the confirm mission dialog box
// parameters: none
// return: none
void MainWindow::confirmMission()
{
    if (_targetColor.contains("UNDEFINED") ||
        _targetShape.contains("UNDEFINED") ||
        _ui->task_selected_list->count() <= 0)
    {
        MissionInvalidDialog window(this);
        window.setColor(_targetColor);
        window.setShape(_targetShape);
        window.setActions(_ui->task_selected_list->count());
        window.exec();
    }
    else
    {
        ConfirmMissionDialog window(this);
        window.setTasks(_ui->task_selected_list->model());
        window.setTargetColor(_targetColor);
        window.setTargetShape(_targetShape);

        bool confirm = window.exec();

        if (confirm)
        {
            this->exportMission();

            // new dialog, work in progress
            QProcess p;
            QStringList params;

            params << "script.py -arg1 arg1";
            p.start("python", params);
            p.waitForFinished(-1);
            QString p_stdout = p.readAll();
        }
    }
}

//******************************************************************************
// purpose: initalizes each EmitTextButton with a color to emit when pressed.
// parameters: none
// return: none
void MainWindow::initColorButtons()
{
    _ui->target_red_button->assignTextValue("RED");
    _ui->target_blue_button->assignTextValue("BLUE");
    _ui->target_black_button->assignTextValue("BLACK");
    _ui->target_green_button->assignTextValue("GREEN");
    _ui->target_white_button->assignTextValue("WHITE");
    _ui->target_yellow_button->assignTextValue("YELLOW");
}

//******************************************************************************
// purpose: initalizes each EmitTextButton with a shape to emit with pressed.
// parameters: none
// return: none
void MainWindow::initShapeButtons()
{
    _ui->target_buoy_button->assignTextValue("BUOY");
    _ui->target_circle_button->assignTextValue("CIRCLE");
    _ui->target_gate_button->assignTextValue("GATE");
    _ui->target_triangle_button->assignTextValue("TRIANGLE");
    _ui->target_rectangle_button->assignTextValue("RECTANGLE");
}

//******************************************************************************
// purpose: initializes the actions available model with the supported robosub
//          tasks. tasks are represented as QStrings.
// parameters: none
// return: none
void MainWindow::initActionsAvailableModel()
{
    QFile file("..\\MissionControl\\mission\\tasks-supported.txt");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream input(&file);
        int row = 0;

        while(!input.atEnd())
        {
            _ui->task_options_list->addItem(input.readLine());
            _ui->task_options_list->item(row)->setTextAlignment(Qt::AlignCenter);
            ++row;
        }

        file.close();
    }
    else
        qDebug() << "MainWindow::initActionsAvailableModel:: unable to open input file.";
}

//******************************************************************************
// purpose: export mission parameters to text file for AI
// parameters: none
// return: none
void MainWindow::exportMission()
{
    QFile file("..\\MissionControl\\mission\\mission.txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QTextStream output(&file);

        // write target parameters
        output << _targetColor << ",";
        output << _targetShape << "\n";;

        // TODO
        /* write code to handle different dimensions here
        output << "RADIUS: " << _targetRadius << "\n";
        output << "LENGTH: " << _targetLength << "\n";
        output << "WIDTH: " << _targetWidth << "\n";
        */

        // write mission actions
        int row = 0;
        while (_ui->task_selected_list->item(row) != nullptr)
        {
            QString action = _ui->task_selected_list->item(row)->text();
            output << action;

            ++row;

            if (row < _ui->task_selected_list->count())
                output << ",";
        }

        file.close();
    }
    else
        qDebug() << "MainWindow::exportMission:: unable to open output file.";
}

//******************************************************************************
// purpose: clears current mission parameters
// parameters: none
// return: none
void MainWindow::cancelMission()
{
    CancelMissionDialog window(this);
    bool confirm = window.exec();

    if (confirm)
    {
        _targetColor.clear();
        _targetShape.clear();
        _targetWidth.clear();
        _targetLength.clear();
        _targetRadius.clear();
        _ui->task_selected_list->clear();
    }
}

//******************************************************************************
// purpose: change available dimension input based on shape selection
// parameters: none
// return: none
void MainWindow::showDimensions()
{
    // TODO
}

//******************************************************************************
// purpose:
// parameters:
// return:
void MainWindow::showUnsupported()
{
    UnsupportedDialog w(this);
    w.exec();
}

//******************************************************************************
// purpose:
// parameters:
// return:
void MainWindow::addTask(QListWidgetItem *task)
{
    _ui->task_selected_list->addItem(task->text());
}

//******************************************************************************
// purpose: remove an item
// parameters: item index (int)
// return: none
void MainWindow::removeTask(QListWidgetItem *task)
{
    delete _ui->task_selected_list->takeItem(_ui->task_selected_list->row(task));
}

//******************************************************************************
// purpose: set target color
// parameters: color (QString)
// return: none
void MainWindow::setTargetColor(const QString &color)
{ _targetColor = color; }

//******************************************************************************
// purpose: set target shape
// parameters: shape (QString)
// return: none
void MainWindow::setTargetShape(const QString &shape)
{ _targetShape = shape; }

//******************************************************************************
// purpose: set target radius
// parameters: radius (QString)
// return: none
void MainWindow::setTargetRadius(const QString &radius)
{ _targetRadius = radius; }

//******************************************************************************
// purpose: set target length
// parameters: length (QString)
// return: none
void MainWindow::setTargetLength(const QString &length)
{ _targetLength = length; }

//******************************************************************************
// purpose: set target width
// parameters: width (QString)
// return: none
void MainWindow::setTargetWidth(const QString &width)
{ _targetWidth = width; }
