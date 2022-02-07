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
    enum PASTE_EXPOSURE {PUBLIC=0,UNLISTED=1,PRIVATE=2}; // ENUM REPRESENTING THE VISIBILITY OF THE PASTE
    QNetworkAccessManager *manager;
    QNetworkReply *replyForPastedCode;
    QNetworkReply *replyForApiRequest;
    QUrlQuery urlContainingPasteData;

    QString API_KEY;
    QString linkToPastedCode;
public:
    explicit QPasteBin(QObject *parent = nullptr);
    bool setUpPasteData(QString code, QString pasteName="Untitled", PASTE_EXPOSURE visibility=PUBLIC);
    void buildPasteLink();
    void buildApiKey();
    void requestApiKey();

    void paste();
    QString getLink();
    ~QPasteBin();

signals:
    void complete();
};

#endif // QPASTEBIN_H
