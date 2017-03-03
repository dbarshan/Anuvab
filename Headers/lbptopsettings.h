#ifndef LBPTOPSETTINGS_H
#define LBPTOPSETTINGS_H

#include <QDialog>

namespace Ui {
class lbpTopSettings;
}

class lbpTopSettings : public QDialog
{
    Q_OBJECT
    
public slots:
    void okButtonPressed();

public:
    explicit lbpTopSettings(QWidget *parent = 0);
    ~lbpTopSettings();
    
private:
    Ui::lbpTopSettings *ui;
};

#endif // LBPTOPSETTINGS_H
