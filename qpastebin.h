#ifndef QPASTEBIN_H
#define QPASTEBIN_H

#include <QObject>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QUrl>
#include <QUrlQuery>

class QPasteBin : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *manager;
    QNetworkReply *replyForPastedCode;
    QNetworkReply *replyForApiRequest;
    QUrlQuery urlContainingPasteData;

    QString API_KEY;
    QString linkToPastedCode;

    void completePaste();
public:
    explicit QPasteBin(QObject *parent = nullptr);
    void setUpPasteData(QString code, QString pasteName, QString visibility, QString format);
    void buildPasteLink();
    void buildApiKey();
    void requestApiKey();

    void paste();
    QString getLink();
    ~QPasteBin();

signals:
    void complete(QString);
};

#endif // QPASTEBIN_H
