#ifndef QPASTEBIN_H
#define QPASTEBIN_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QUrl>
#include <QUrlQuery>

class QPasteBin : public QWidget
{
    Q_OBJECT
private:
    enum PASTE_MODE {PUBLIC=0,UNLISTED=1,PRIVATE=2};
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString API_KEY;
    QString _code;
    QString _name;
    PASTE_MODE _mode;
    QUrlQuery postData;
    QString val;

public:
    explicit QPasteBin(QWidget *parent = nullptr);
    void setUpPasting(QString code, QString name="Untitled", PASTE_MODE mode=PUBLIC);
    void setUp();
    void paste();
    void readData();
    QString getLink();

signals:
    void complete();
};

#endif // QPASTEBIN_H
