#include "qpastebin.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

void QPasteBin::completePaste()
{
    QNetworkRequest apiRequestDestination{QUrl("https://pastebin.com/api/api_post.php")};
    apiRequestDestination.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    replyForPastedCode = manager->post(apiRequestDestination, urlContainingPasteData.toString(QUrl::FullyEncoded).toUtf8());
    connect(replyForPastedCode,&QNetworkReply::finished,this,&QPasteBin::buildPasteLink);
}

QPasteBin::QPasteBin(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(parent);
    replyForPastedCode = nullptr;
    replyForApiRequest = nullptr;
}

void QPasteBin::setUpPasteData(QString code, QString pasteName, QString visibility, QString format)
{
    urlContainingPasteData.addQueryItem("api_paste_code",code);
    urlContainingPasteData.addQueryItem("api_option", "paste");
    urlContainingPasteData.addQueryItem("api_paste_private",visibility);
    urlContainingPasteData.addQueryItem("api_paste_name",pasteName);

    if (format == "cpp")
        urlContainingPasteData.addQueryItem("api_paste_format","cpp");
    else if (format == "cpp-qt")
        urlContainingPasteData.addQueryItem("api_paste_format","cpp-qt");
}

void QPasteBin::paste()
{
    if (API_KEY.isEmpty())
    {
        requestApiKey();
        return;
    }

    completePaste();
}

void QPasteBin::requestApiKey()
{
    QNetworkRequest apiKeyRequestDestination{QUrl{"https://notepad-proxy.herokuapp.com/api"}};
    replyForApiRequest = manager->get(apiKeyRequestDestination);
    connect(replyForApiRequest, &QNetworkReply::finished,this,&QPasteBin::buildApiKey);
}

void QPasteBin::buildApiKey()
{
    if (replyForApiRequest->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error Retrieving KEY " << replyForApiRequest->errorString();
        return;
    }

    /* Read the data gotten from the API Key request and retrieve the key from the JSON Object */
    QByteArray apiKeyData = replyForApiRequest->readAll();
    QJsonObject keyObject = QJsonDocument::fromJson(apiKeyData).object();

    API_KEY = keyObject.value("key").toString();
    urlContainingPasteData.addQueryItem("api_dev_key",API_KEY);

    qDebug() << API_KEY;

    completePaste();

    replyForApiRequest->deleteLater(); // SCHEDULE THE NETWORK REPLY FOR DELETING
}

void QPasteBin::buildPasteLink()
{
    QByteArray pasteLinkData = replyForPastedCode->readAll();
    linkToPastedCode = QUrl::fromPercentEncoding(pasteLinkData);

    emit complete(linkToPastedCode);
}

QString QPasteBin::getLink()
{
    return linkToPastedCode;
}

QPasteBin::~QPasteBin()
{
    delete replyForPastedCode;
}
