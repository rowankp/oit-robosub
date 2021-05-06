//******************************************************************************
// filename: SubbyButton.h
// authors: Rowan Parker (rowan.parker@oit.edu)
// date: 02/19/2020
// purpose: subclass of QPushButton for Subby joke button

#ifndef SUBBY_H
#define SUBBY_H

#include <QtWidgets/QPushButton>

class SubbyButton : public QPushButton
{
    Q_OBJECT

    public:
        SubbyButton(QWidget *parent = nullptr);

    private:
        QStringList _jokes;

        void initJokes();

    private slots:
        void handlePressed();
        void handleTimeout();

    signals:
        void emitJoke(const QString& joke);
        void hideJoke();
};

#endif // SUBBY_H
