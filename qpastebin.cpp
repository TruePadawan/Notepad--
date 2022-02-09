#include "qpastebin.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

QPasteBin::QPasteBin(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(parent);
    replyForPastedCode = nullptr;
    replyForApiRequest = nullptr;

    requestApiKey();
}

bool QPasteBin::setUpPasteData(QString code, QString pasteName, PASTE_EXPOSURE visibility)
{
    if (API_KEY.isEmpty())
    {
        requestApiKey();
        return false;
    }

    urlContainingPasteData.addQueryItem("api_dev_key",API_KEY);
    urlContainingPasteData.addQueryItem("api_paste_code",code);
    urlContainingPasteData.addQueryItem("api_option", "paste");
    urlContainingPasteData.addQueryItem("api_paste_private",QString::number(visibility));
    urlContainingPasteData.addQueryItem("api_paste_name",pasteName);
    urlContainingPasteData.addQueryItem("api_paste_format","cpp");
    return true;
}

void QPasteBin::paste()
{
    QNetworkRequest apiRequestDestination{QUrl("https://pastebin.com/api/api_post.php")};
    apiRequestDestination.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    replyForPastedCode = manager->post(apiRequestDestination, urlContainingPasteData.toString(QUrl::FullyEncoded).toUtf8());
    connect(replyForPastedCode,&QNetworkReply::finished,this,&QPasteBin::buildPasteLink);
}

void QPasteBin::buildPasteLink()
{
    auto pasteLinkData = replyForPastedCode->readAll();
    linkToPastedCode = QUrl::fromPercentEncoding(pasteLinkData);
    emit complete();
}

void QPasteBin::buildApiKey()
{
    if (replyForApiRequest->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error Retrieving KEY " << replyForApiRequest->errorString();
        return;
    }

    auto apiKeyData = replyForApiRequest->readAll();
    auto keyObject = QJsonDocument::fromJson(apiKeyData).object();
    API_KEY = keyObject.value("key").toString();

    replyForApiRequest->deleteLater(); // SCHEDULE THE NETWORK REPLY FOR DELETION
}

void QPasteBin::requestApiKey()
{
    QNetworkRequest apiKeyRequestDestination{QUrl{"https://notepad-proxy.herokuapp.com/api"}};
    replyForApiRequest = manager->get(apiKeyRequestDestination);
    connect(replyForApiRequest, &QNetworkReply::finished,this,&QPasteBin::buildApiKey);
}

QString QPasteBin::getLink()
{
    return linkToPastedCode;
}

QPasteBin::~QPasteBin()
{
    delete replyForPastedCode;
}
