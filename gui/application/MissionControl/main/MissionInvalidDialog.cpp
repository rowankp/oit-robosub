#include "main/MissionInvalidDialog.h"
#include "ui_MissionInvalidDialog.h"

MissionInvalidDialog::MissionInvalidDialog(QWidget *parent): QDialog(parent), _ui(new Ui::MissionInvalidDialog)
{
    _ui->setupUi(this);
    connect(_ui->ok_pushbutton, SIGNAL(released()), this, SLOT(ok_released()));
}

MissionInvalidDialog::~MissionInvalidDialog()
{
    delete _ui;
}

void MissionInvalidDialog::setColor(const QString &color)
{
    _ui->color_label->setText(color);
}

void MissionInvalidDialog::setShape(const QString &shape)
{
    _ui->shape_label->setText(shape);
}

void MissionInvalidDialog::setActions(int value)
{
    _ui->actions_number_label->setText(QString("%1").arg(value));
}

void MissionInvalidDialog::ok_released()
{
    this->close();
}
