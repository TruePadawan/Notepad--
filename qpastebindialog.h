#ifndef QPASTEBINDIALOG_H
#define QPASTEBINDIALOG_H

#include <QDialog>
#include <QHash>

namespace Ui {
class QPastebinDialog;
}

class QPastebinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QPastebinDialog(QWidget *parent = nullptr);
    QHash<QString,QString> getData();
    ~QPastebinDialog();

private:
    Ui::QPastebinDialog *ui;

    void _init();
};

#endif // QPASTEBINDIALOG_H
