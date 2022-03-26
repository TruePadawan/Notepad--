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

    void undo();
    void redo();

    void copy();
    void cut();
    void paste();
    void selectAll();

    void setCurrentWidget(CustomTextEdit *_widget);
    void updatePreferredSyntaxForTab(const QString syntaxType);

    void setWidgetToNull();
    ~MainController();

private:
    CustomTextEdit *widget;
    bool isCurrentWidgetDataSaved{true};
    QHash<QString,QString> getNameAndPreferredSyntax(CustomTextEdit *_widget = nullptr);

signals:
    void widgetChanged(QHash<QString,QString> fileNameAndPrefferedSyntax);
    void widgetTextChanged(bool isModified);
    void widgetTextSaved(bool isSaved, QString fileName);
    void widgetTextSavedAs(bool isSaved, QString fileName);

};

#endif // MAINCONTROLLER_H
