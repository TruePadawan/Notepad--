#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QHash>
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
    void setCurrentWidget(CustomTextEdit *_widget);
    ~MainController();

private:
    CustomTextEdit *widget;
    bool isCurrentWidgetDataSaved{true};
    QHash<QString,QString> getNameAndTypeOfFile(CustomTextEdit *_widget);

signals:
    void widgetChanged(QHash<QString,QString>);
    void widgetTextChanged(bool isModified);

};

#endif // MAINCONTROLLER_H
