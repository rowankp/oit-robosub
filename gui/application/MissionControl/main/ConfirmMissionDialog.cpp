#include "ConfirmMissionDialog.h"
#include "ui_ConfirmMissionDialog.h"

ConfirmMissionDialog::ConfirmMissionDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::ConfirmMissionDialog)
{
    _ui->setupUi(this);
}

ConfirmMissionDialog::~ConfirmMissionDialog()
{
    delete _ui;
}

void ConfirmMissionDialog::setTasks(QAbstractItemModel *model)
{
    _ui->listView->setModel(model);
}

void ConfirmMissionDialog::setTargetColor(QString color)
{
    _ui->labelTargetColorInput->setText(color);
}

void ConfirmMissionDialog::setTargetShape(QString shape)
{
    _ui->labelTargetShapeInput->setText(shape);
}
