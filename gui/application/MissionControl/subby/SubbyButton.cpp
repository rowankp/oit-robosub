//******************************************************************************
// filename: SubbyButton.cpp
// authors: Rowan Parker (rowan.parker@oit.edu)
// date: 02/19/2020

#include "SubbyButton.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QRandomGenerator>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

//******************************************************************************
// purpose: constructor
// paremeters: QWidget *
// return: n/a
SubbyButton::SubbyButton(QWidget * parent) : QPushButton(parent)
{
    QPixmap pixmap("..\\MissionControl\\subby\\subby-icon.png");
    QIcon ButtonIcon(pixmap);
    this->setIcon(ButtonIcon);
    this->setIconSize(pixmap.rect().size());
    this->initJokes();

    connect(this, SIGNAL(released()), this, SLOT(handlePressed()));
}

//******************************************************************************
// purpose: read jokes in from file
// paremeters: none
// return: none
void SubbyButton::initJokes()
{
    QFile file("..\\MissionControl\\subby\\subby-puns.txt");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream input(&file);

        while(!input.atEnd())
        {
            _jokes.push_back(input.readLine());
        }

        file.close();
    }
    else
        qDebug() << "SubbyButton::initJokes: unable to open file.";
}

//******************************************************************************
// purpose: display joke
// paremeters: none
// return: none
void SubbyButton::handlePressed()
{
    quint32 index =  QRandomGenerator::global()->bounded(_jokes.size());
    QTimer::singleShot(3500, this, SLOT(handleTimeout()));
    emit(emitJoke(_jokes.at(index)));
}

//******************************************************************************
// purpose: emit signal to clear joke label
// paremeters: none
// return: none
void SubbyButton::handleTimeout()
{
    emit(hideJoke());
}
