#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QFile>
#include <QString>
#include "tabwidget.h"

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = nullptr);
    TabWidget *newTabWidget(QWidget *parent=nullptr);
    TabWidget *newTabWidget(QFile &file, QWidget *parent=nullptr);
    void save();
    void saveAs();
    void setCurrentTabWidget(TabWidget *widget);

private:
    TabWidget *tabWidget;
    bool isCurrentTabWidgetDataSaved{true};

signals:
    void tabWidgetChanged();
    void tabWidgetTextChanged(bool isModified);

};

#endif // MAINCONTROLLER_H
