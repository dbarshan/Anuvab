#include "exprpathsettings.h"
#include "ui_exprpathsettings.h"
#include <iostream>
#include <QFileDialog>
#include <QTreeView>
exprPathSettings::exprPathSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exprPathSettings)
{
    ui->setupUi(this);

    numExpr = 0;

    ui->expr1Label->setDisabled(true);
    ui->expr2Label->setDisabled(true);
    ui->expr3Label->setDisabled(true);
    ui->expr4Label->setDisabled(true);
    ui->expr5Label->setDisabled(true);
    ui->expr6Label->setDisabled(true);
    ui->expr7Label->setDisabled(true);

    connect(ui->okButton,SIGNAL(clicked()),SLOT(okButtonPressed()));
    connect(ui->cancelButton,SIGNAL(clicked()),SLOT(close()));
    connect(ui->expr1Name,SIGNAL(textChanged(QString)),SLOT(expr1TextChanged()));
    connect(ui->expr2Name,SIGNAL(textChanged(QString)),SLOT(expr2TextChanged()));
    connect(ui->expr3Name,SIGNAL(textChanged(QString)),SLOT(expr3TextChanged()));
    connect(ui->expr4Name,SIGNAL(textChanged(QString)),SLOT(expr4TextChanged()));
    connect(ui->expr5Name,SIGNAL(textChanged(QString)),SLOT(expr5TextChanged()));
    connect(ui->expr6Name,SIGNAL(textChanged(QString)),SLOT(expr6TextChanged()));
    connect(ui->expr7Name,SIGNAL(textChanged(QString)),SLOT(expr7TextChanged()));

    connect(ui->expr1Path,SIGNAL(textChanged(QString)),SLOT(expr1PathChanged()));
    connect(ui->expr2Path,SIGNAL(textChanged(QString)),SLOT(expr2PathChanged()));
    connect(ui->expr3Path,SIGNAL(textChanged(QString)),SLOT(expr3PathChanged()));
    connect(ui->expr4Path,SIGNAL(textChanged(QString)),SLOT(expr4PathChanged()));
    connect(ui->expr5Path,SIGNAL(textChanged(QString)),SLOT(expr5PathChanged()));
    connect(ui->expr6Path,SIGNAL(textChanged(QString)),SLOT(expr6PathChanged()));
    connect(ui->expr7Path,SIGNAL(textChanged(QString)),SLOT(expr7PathChanged()));


    connect(ui->browse1,SIGNAL(clicked()),SLOT(browse1Pressed()));
    connect(ui->browse2,SIGNAL(clicked()),SLOT(browse2Pressed()));
    connect(ui->browse3,SIGNAL(clicked()),SLOT(browse3Pressed()));
    connect(ui->browse4,SIGNAL(clicked()),SLOT(browse4Pressed()));
    connect(ui->browse5,SIGNAL(clicked()),SLOT(browse5Pressed()));
    connect(ui->browse6,SIGNAL(clicked()),SLOT(browse6Pressed()));
    connect(ui->browse7,SIGNAL(clicked()),SLOT(browse7Pressed()));
}

exprPathSettings::~exprPathSettings()
{
    delete ui;
}

void exprPathSettings::expr1TextChanged()
{
    QString name = ui->expr1Name->text();
    QString path = ui->expr1Path->text();
    if ( name.isEmpty()||path.isEmpty())
        ui->expr1Label->setDisabled(true);
    else
        ui->expr1Label->setEnabled(true);
}

void exprPathSettings::expr2TextChanged()
{
    QString name = ui->expr2Name->text();
    QString path = ui->expr2Path->text();
    if ( name.isEmpty()||path.isEmpty())
        ui->expr2Label->setDisabled(true);
    else
        ui->expr2Label->setEnabled(true);

}
void exprPathSettings::expr3TextChanged()
{
    QString name = ui->expr3Name->text();
    QString path = ui->expr3Path->text();
    if ( name.isEmpty()||path.isEmpty())
        ui->expr3Label->setDisabled(true);
    else
        ui->expr3Label->setEnabled(true);

}
void exprPathSettings::expr4TextChanged()
{
    QString name = ui->expr4Name->text();
    QString path = ui->expr4Path->text();
    if ( name.isEmpty()||path.isEmpty())
        ui->expr4Label->setDisabled(true);
    else
        ui->expr4Label->setEnabled(true);

}
void exprPathSettings::expr5TextChanged()
{
    QString name = ui->expr5Name->text();
    QString path = ui->expr5Path->text();
    if ( name.isEmpty()||path.isEmpty())
        ui->expr5Label->setDisabled(true);
    else
        ui->expr5Label->setEnabled(true);

}
void exprPathSettings::expr6TextChanged()
{
    QString name = ui->expr6Name->text();
    QString path = ui->expr6Path->text();
    if ( name.isEmpty()||path.isEmpty())
        ui->expr6Label->setDisabled(true);
    else
        ui->expr6Label->setEnabled(true);


}
void exprPathSettings::expr7TextChanged()
{
    QString name = ui->expr7Name->text();
    QString path = ui->expr7Path->text();
    if ( name.isEmpty()||path.isEmpty())
        ui->expr7Label->setDisabled(true);
    else
        ui->expr7Label->setEnabled(true);

}


void exprPathSettings::expr1PathChanged()
{
    QString exprName = ui->expr1Name->text();
    QString exprPath = ui->expr1Path->text();
    if( exprName.isEmpty() || exprPath.isEmpty())
        ui->expr1Label->setDisabled(true);
    else
        ui->expr1Label->setEnabled(true);

}
void exprPathSettings::expr2PathChanged()
{
    QString exprName = ui->expr2Name->text();
    QString exprPath = ui->expr2Path->text();
    if( exprName.isEmpty() || exprPath.isEmpty())
        ui->expr2Label->setDisabled(true);
    else
        ui->expr2Label->setEnabled(true);

}
void exprPathSettings::expr3PathChanged()
{
    QString exprName = ui->expr3Name->text();
    QString exprPath = ui->expr3Path->text();
    if( exprName.isEmpty() || exprPath.isEmpty())
        ui->expr3Label->setDisabled(true);
    else
        ui->expr3Label->setEnabled(true);

}
void exprPathSettings::expr4PathChanged()
{
    QString exprName = ui->expr4Name->text();
    QString exprPath = ui->expr4Path->text();
    if( exprName.isEmpty() || exprPath.isEmpty())
        ui->expr4Label->setDisabled(true);
    else
        ui->expr4Label->setEnabled(true);

}
void exprPathSettings::expr5PathChanged()
{
    QString exprName = ui->expr5Name->text();
    QString exprPath = ui->expr5Path->text();
    if( exprName.isEmpty() || exprPath.isEmpty())
        ui->expr5Label->setDisabled(true);
    else
        ui->expr5Label->setEnabled(true);

}
void exprPathSettings::expr6PathChanged()
{
    QString exprName = ui->expr6Name->text();
    QString exprPath = ui->expr6Path->text();
    if( exprName.isEmpty() || exprPath.isEmpty())
        ui->expr6Label->setDisabled(true);
    else
        ui->expr6Label->setEnabled(true);

}
void exprPathSettings::expr7PathChanged()
{
    QString exprName = ui->expr7Name->text();
    QString exprPath = ui->expr7Path->text();
    if( exprName.isEmpty() || exprPath.isEmpty())
        ui->expr7Label->setDisabled(true);
    else
        ui->expr7Label->setEnabled(true);

}

void exprPathSettings::okButtonPressed()
{

    if( ui->expr1Label->isEnabled())    numExpr++;
    if( ui->expr2Label->isEnabled())    numExpr++;
    if( ui->expr3Label->isEnabled())    numExpr++;
    if( ui->expr4Label->isEnabled())    numExpr++;
    if( ui->expr5Label->isEnabled())    numExpr++;
    if( ui->expr6Label->isEnabled())    numExpr++;
    if( ui->expr7Label->isEnabled())    numExpr++;
    this->close();
}

void exprPathSettings::browse1Pressed()
{
    QFileDialog *fd = new QFileDialog;
    QTreeView *tree = fd->findChild <QTreeView*>();
    tree->setRootIsDecorated(true);
    tree->setItemsExpandable(true);
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    int result = fd->exec();
    if (!result)
        return;
    QString directoryPath = fd->selectedFiles()[0];
    ui->expr1Path->setText(directoryPath);
}

void exprPathSettings::browse2Pressed()
{
    QFileDialog *fd = new QFileDialog;
    QTreeView *tree = fd->findChild <QTreeView*>();
    tree->setRootIsDecorated(true);
    tree->setItemsExpandable(true);
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    int result = fd->exec();
    if (!result)
        return;
    QString directoryPath = fd->selectedFiles()[0];
    ui->expr2Path->setText(directoryPath);
}
void exprPathSettings::browse3Pressed()
{
    QFileDialog *fd = new QFileDialog;
    QTreeView *tree = fd->findChild <QTreeView*>();
    tree->setRootIsDecorated(true);
    tree->setItemsExpandable(true);
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    int result = fd->exec();
    if (!result)
        return;
    QString directoryPath = fd->selectedFiles()[0];
    ui->expr3Path->setText(directoryPath);
}
void exprPathSettings::browse4Pressed()
{
    QFileDialog *fd = new QFileDialog;
    QTreeView *tree = fd->findChild <QTreeView*>();
    tree->setRootIsDecorated(true);
    tree->setItemsExpandable(true);
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    int result = fd->exec();
    if (!result)
        return;
    QString directoryPath = fd->selectedFiles()[0];
    ui->expr4Path->setText(directoryPath);
}
void exprPathSettings::browse5Pressed()
{
    QFileDialog *fd = new QFileDialog;
    QTreeView *tree = fd->findChild <QTreeView*>();
    tree->setRootIsDecorated(true);
    tree->setItemsExpandable(true);
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    int result = fd->exec();
    if (!result)
        return;
    QString directoryPath = fd->selectedFiles()[0];
    ui->expr5Path->setText(directoryPath);
}
void exprPathSettings::browse6Pressed()
{
    QFileDialog *fd = new QFileDialog;
    QTreeView *tree = fd->findChild <QTreeView*>();
    tree->setRootIsDecorated(true);
    tree->setItemsExpandable(true);
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    int result = fd->exec();
    if (!result)
        return;
    QString directoryPath = fd->selectedFiles()[0];
    ui->expr6Path->setText(directoryPath);
}
void exprPathSettings::browse7Pressed()
{
    QFileDialog *fd = new QFileDialog;
    QTreeView *tree = fd->findChild <QTreeView*>();
    tree->setRootIsDecorated(true);
    tree->setItemsExpandable(true);
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    int result = fd->exec();
    if (!result)
        return;
    QString directoryPath = fd->selectedFiles()[0];
    ui->expr7Path->setText(directoryPath);
}

QString exprPathSettings::getExprPath(int index)
{
    switch (index)
    {
    case 1:    return (ui->expr1Path->text());
    case 2:    return (ui->expr2Path->text());
    case 3:    return (ui->expr3Path->text());
    case 4:    return (ui->expr4Path->text());
    case 5:    return (ui->expr5Path->text());
    case 6:    return (ui->expr6Path->text());
    case 7:    return (ui->expr7Path->text());
    default:   return ("");
    }
}

QString exprPathSettings::getExprName(int index)
{
    switch (index)
    {
    case 1:    return (ui->expr1Name->text());
    case 2:    return (ui->expr2Name->text());
    case 3:    return (ui->expr3Name->text());
    case 4:    return (ui->expr4Name->text());
    case 5:    return (ui->expr5Name->text());
    case 6:    return (ui->expr6Name->text());
    case 7:    return (ui->expr7Name->text());
    default:   return ("");
    }
}

int exprPathSettings::getNumExpr()
{
    return numExpr;
}
