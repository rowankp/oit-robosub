#ifndef MISSIONINVALIDDIALOG_H
#define MISSIONINVALIDDIALOG_H

#include <QDialog>

namespace Ui
{ class MissionInvalidDialog; }

class MissionInvalidDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit MissionInvalidDialog(QWidget *parent = nullptr);
        ~MissionInvalidDialog();

        void setColor(const QString &value);
        void setShape(const QString &value);
        void setActions(int value);

    private slots:
        void ok_released();

    private:
        Ui::MissionInvalidDialog *_ui;
};

#endif // MISSIONINVALIDDIALOG_H
