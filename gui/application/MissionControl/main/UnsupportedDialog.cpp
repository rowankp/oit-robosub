#include "main/UnsupportedDialog.h"
#include "ui_UnsupportedDialog.h"

UnsupportedDialog::UnsupportedDialog(QWidget *parent): QDialog(parent), _ui(new Ui::UnsupportedDialog)
{
    _ui->setupUi(this);

    connect(_ui->pushButtonOk, SIGNAL(released()), this, SLOT(okReleased()));
}

UnsupportedDialog::~UnsupportedDialog()
{
    delete _ui;
}

void UnsupportedDialog::okReleased()
{
    this->close();
}
