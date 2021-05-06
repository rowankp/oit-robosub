//******************************************************************************
// filename: ExitTextButton.h
// authors: Rowan Parker (rowan.parker@oit.edu)
// date: 04/18/2020
// purpose: subclass of QPushButton to emit a target parameter (color, shape)
//          when pressed.

#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include <QtWidgets/QPushButton>
#include <QtCore/QString>

class EmitTextButton : public QPushButton
{
    Q_OBJECT

    public:
        EmitTextButton(QWidget *parent = nullptr);
        const QString & getTextValue() const;
        void assignTextValue(const QString &text);

    private:
        QString _text;

    private slots:
        void selected();

    signals:
        void emitText(const QString &text);
};

#endif // TEXTBUTTON_H
