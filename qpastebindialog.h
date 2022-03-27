#ifndef QPASTEBINDIALOG_H
#define QPASTEBINDIALOG_H

#include <QDialog>

namespace Ui {
class QPastebinDialog;
}

class QPastebinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QPastebinDialog(QWidget *parent = nullptr);
    ~QPastebinDialog();

private:
    Ui::QPastebinDialog *ui;
};

#endif // QPASTEBINDIALOG_H
