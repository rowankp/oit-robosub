#include "main/CancelMissionDialog.h"
#include "ui_CancelMissionDialog.h"

CancelMissionDialog::CancelMissionDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::CancelMissionDialog)
{
    _ui->setupUi(this);
}

CancelMissionDialog::~CancelMissionDialog()
{
    delete _ui;
}
