#ifndef EXPRPATHSETTINGS_H
#define EXPRPATHSETTINGS_H

#include <QDialog>

namespace Ui {
class exprPathSettings;
}

class exprPathSettings : public QDialog
{
    Q_OBJECT
public slots:
    void okButtonPressed();

    void expr1TextChanged();
    void expr2TextChanged();
    void expr3TextChanged();
    void expr4TextChanged();
    void expr5TextChanged();
    void expr6TextChanged();
    void expr7TextChanged();

    void expr1PathChanged();
    void expr2PathChanged();
    void expr3PathChanged();
    void expr4PathChanged();
    void expr5PathChanged();
    void expr6PathChanged();
    void expr7PathChanged();

    void browse1Pressed();
    void browse2Pressed();
    void browse3Pressed();
    void browse4Pressed();
    void browse5Pressed();
    void browse6Pressed();
    void browse7Pressed();

public:
    explicit exprPathSettings(QWidget *parent = 0);
    ~exprPathSettings();
    QString getExprPath(int index);
    QString getExprName(int index);
    int getNumExpr();
    
private:
    Ui::exprPathSettings *ui;
    int numExpr;
};

#endif // EXPRPATHSETTINGS_H
