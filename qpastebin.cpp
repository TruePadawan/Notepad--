#include "qpastebin.h"

QPasteBin::QPasteBin(QWidget *parent) : QWidget(parent)
{
    manager = new QNetworkAccessManager(parent);
}

void QPasteBin::setUpPasting(QString code, QString name, PASTE_MODE mode)
{
    API_KEY = "3oi69tXZQPRGGP6ovxVjZAjLKki3ncyx";
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
   // postData.addQueryItem("api_paste_expire_date",QString::number(timeExp)+"M");
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

QString QPasteBin::getLink()
{
    return val;
}

QPasteBin::~QPasteBin()
{
    delete manager;
    delete reply;
}