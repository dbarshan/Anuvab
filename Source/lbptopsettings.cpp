#include "lbptopsettings.h"
#include "ui_lbptopsettings.h"
#include "global.h"

lbpTopSettings::lbpTopSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::lbpTopSettings)
{
    ui->setupUi(this);
    setWindowTitle("LBP-TOP Parameter Settings");
    ui->PxySpinBox->setRange(1,16);
    ui->PxtSpinBox->setRange(1,16);
    ui->PytSpinBox->setRange(1,16);

    ui->PxySpinBox->setValue(8);
    ui->PxtSpinBox->setValue(8);
    ui->PytSpinBox->setValue(8);

    ui->RxSpinBox->setRange(0,50);
    ui->RySpinBox->setRange(0,50);
    ui->RtSpinBox->setRange(0,50);

    ui->RxSpinBox->setValue(3);
    ui->RySpinBox->setValue(3);
    ui->RtSpinBox->setValue(3);

    connect(ui->cancelButton,SIGNAL(clicked()),SLOT(close()));
    connect(ui->okButton,SIGNAL(clicked()),SLOT(okButtonPressed()));
}

lbpTopSettings::~lbpTopSettings()
{
    delete ui;
}

void lbpTopSettings::okButtonPressed()
{
    Pxy = ui->PxySpinBox->value();
    Pxt = ui->PxtSpinBox->value();
    Pyt = ui->PytSpinBox->value();

    Rx = ui->RxSpinBox->value();
    Ry = ui->RySpinBox->value();
    Rt = ui->RtSpinBox->value();

    this->close();
}
