#include "qpastebindialog.h"
#include "ui_qpastebindialog.h"

QPastebinDialog::QPastebinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPastebinDialog)
{
    ui->setupUi(this);
}

QPastebinDialog::~QPastebinDialog()
{
    delete ui;
}
