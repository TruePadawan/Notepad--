#include "qpastebin.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

QPasteBin::QPasteBin(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(parent);
    pasteData = nullptr;
    apiData = nullptr;

    requestApi();
}

void QPasteBin::setUpPasting(QString code, QString name, PASTE_MODE mode)
{
    if (API_KEY.isEmpty())
    {
        requestApi();
    }
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
    pasteData = manager->post(req, postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(pasteData,&QNetworkReply::finished,this,&QPasteBin::readPasteData);
}

void QPasteBin::readPasteData()
{
    auto buffer = pasteData->readAll();
    val = QUrl::fromPercentEncoding(buffer);
    emit complete();
}

void QPasteBin::readApiData()
{
    qDebug() << "READING API DATA...........";
    if (apiData->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error Retrieving KEY " << apiData->errorString();
    }else
    {
        auto apiDataBuffer = apiData->readAll();
        auto keyObject = QJsonDocument::fromJson(apiDataBuffer).object();
        API_KEY = keyObject.value("key").toString();
        qDebug() << "API DATA " << API_KEY;
    }
    qDebug() << "FINISHED READING API DATA...........";
    apiData = nullptr;
}

void QPasteBin::requestApi()
{
    QNetworkRequest API_REQ{QUrl{"https://notepad-proxy.herokuapp.com/api"}};
    apiData = manager->get(API_REQ);
    connect(apiData, &QNetworkReply::finished,this,&QPasteBin::readApiData);

}

QString QPasteBin::getLink()
{
    return val;
}

QPasteBin::~QPasteBin()
{
    delete manager;
    delete pasteData;
    delete apiData;
}
