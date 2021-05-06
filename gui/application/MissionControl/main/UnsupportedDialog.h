#ifndef UNSUPPORTEDDIALOG_H
#define UNSUPPORTEDDIALOG_H

#include <QDialog>

namespace Ui
{ class UnsupportedDialog; }

class UnsupportedDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit UnsupportedDialog(QWidget *parent = nullptr);
        ~UnsupportedDialog();

    private slots:
        void okReleased();

    private:
        Ui::UnsupportedDialog *_ui;
};

#endif // UNSUPPORTEDDIALOG_H
