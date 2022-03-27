#include "qpastebindialog.h"
#include "ui_qpastebindialog.h"
#include <QHash>

QPastebinDialog::QPastebinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPastebinDialog)
{
    ui->setupUi(this);
    setModal(true);
    _init();
}

QPastebinDialog::~QPastebinDialog()
{
    delete ui;
}

void QPastebinDialog::_init()
{
    ui->pasteExposureCombobox->addItem("Public");
    ui->pasteExposureCombobox->addItem("Unlisted");

    ui->syntaxHighlightingCombobox->addItem("None");
    ui->syntaxHighlightingCombobox->addItem("CPP");
    ui->syntaxHighlightingCombobox->addItem("CPP-QT");
}

QHash<QString,QString> QPastebinDialog::getData()
{
    QHash<QString,QString> data;

    data["paste_name"] = ui->pasteNameLineEdit->text();
    data["paste_format"] = ui->syntaxHighlightingCombobox->currentText().toLower();
    if (ui->pasteExposureCombobox->currentText() == "Public")
    {
        data["paste_exposure"] = "0";
    }
    else {
        data["paste_exposure"] = "1";
    }

    return data;
}
