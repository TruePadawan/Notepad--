#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QFile>
#include <QString>
#include "customtextedit.h"

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent=nullptr);
    CustomTextEdit *newWidget(QWidget *parent=nullptr);
    CustomTextEdit *newWidget(QString &filePath, QWidget *parent=nullptr);
    void save();
    void saveAs();
    void setCurrentWidget(CustomTextEdit *widget);
    ~MainController();

private:
    CustomTextEdit *widget;
    bool isCurrentWidgetDataSaved{true};

signals:
    void widgetChanged();
    void widgetTextChanged(bool isModified);

};

#endif // MAINCONTROLLER_H
