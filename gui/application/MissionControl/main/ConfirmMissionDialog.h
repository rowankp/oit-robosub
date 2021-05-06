#ifndef CONFIRMMISSIONDIALOGBOX_H
#define CONFIRMMISSIONDIALOGBOX_H

#include <QAbstractItemModel>
#include <QDialog>

namespace Ui
{ class ConfirmMissionDialog; }

class ConfirmMissionDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit ConfirmMissionDialog(QWidget *parent = nullptr);
        ~ConfirmMissionDialog();

        void setTasks(QAbstractItemModel *model);
        void setTargetColor(QString color);
        void setTargetShape(QString shape);

    private:
        Ui::ConfirmMissionDialog *_ui;
};

#endif // CONFIRMMISSIONDIALOGBOX_H
