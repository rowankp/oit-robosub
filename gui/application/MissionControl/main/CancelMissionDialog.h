#ifndef CANCELMISSIONDIALOG_H
#define CANCELMISSIONDIALOG_H

#include <QDialog>

namespace Ui
{ class CancelMissionDialog; }

class CancelMissionDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit CancelMissionDialog(QWidget *parent = nullptr);
        ~CancelMissionDialog();

    private:
        Ui::CancelMissionDialog *_ui;
};

#endif // CANCELMISSIONDIALOG_H
