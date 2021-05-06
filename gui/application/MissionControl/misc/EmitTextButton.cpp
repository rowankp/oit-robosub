//******************************************************************************
// filename: EmitTextButton.cpp
// authors: Rowan Parker (rowan.parker@oit.edu)
// date: 04/18/2020

#include "EmitTextButton.h"

//******************************************************************************
// purpose: constructor.
// parameters: QWidget *
// return: n/a
EmitTextButton::EmitTextButton(QWidget *parent): QPushButton(parent)
{
    connect(this, SIGNAL(released()), this, SLOT(selected()));
}

//******************************************************************************
// purpose: _text accessor.
// parameters: none
// return: const QString & (_text)
const QString & EmitTextButton::getTextValue() const
{
    return _text;
}

//******************************************************************************
// purpose: _ text mutator.
// parameters: const QString & (text value)
// return: none
void EmitTextButton::assignTextValue(const QString &text)
{
    _text = text;
}

//******************************************************************************
// purpose: when button is pressed, emitText signal is emitted
// parameters: none
// return: none
void EmitTextButton::selected()
{
    emit(emitText(_text));
}
