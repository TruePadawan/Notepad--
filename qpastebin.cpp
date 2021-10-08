#include "qpastebin.h"
#include <QFile>
#include <QTextStream>

QPasteBin::QPasteBin(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(parent);
    reply = nullptr;
}

void QPasteBin::setUpPasting(QString code, QString name, PASTE_MODE mode)
{
    API_KEY = getApi();
    _code = code;
    _mode = mode;
    _name = name;
    setUp();
}

void QPasteBin::setUp()
{
    postData.addQueryItem("api_dev_key",API_KEY);
    postData.addQueryItem("api_paste_code",_code);
    postData.addQueryItem("api_option", "paste");
    postData.addQueryItem("api_paste_private",QString::number(_mode));
    postData.addQueryItem("api_paste_name",_name);
    postData.addQueryItem("api_paste_format","cpp");
}

void QPasteBin::paste()
{
    QNetworkRequest req{QUrl("https://pastebin.com/api/api_post.php")};
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    reply = manager->post(req, postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply,&QNetworkReply::finished,this,&QPasteBin::readData);
}

void QPasteBin::readData()
{
    auto buffer = reply->readAll();
    val = QUrl::fromPercentEncoding(buffer);
    emit complete();
}

QString QPasteBin::getApi()
{
    QFile key{":/resources/api.txt"};
    if (!key.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't Access API file";
    }

    QTextStream stream{&key};
    auto api = stream.readLine();
    qDebug() << api;
    return api;
}

QString QPasteBin::getLink()
{
    return val;
}

QPasteBin::~QPasteBin()
{
    delete reply;
    delete manager;
}
