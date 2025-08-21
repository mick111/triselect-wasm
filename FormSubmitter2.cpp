#include "FormSubmitter.h"
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QDebug>

// Google Form URL from your HTML form
const QString FormSubmitter::FORM_URL = "https://docs.google.com/forms/d/112YW8Dh1F4PeaCMEqtNn_M7QZKovDXNgwgUuSJtyjgk/formResponse";

FormSubmitter::FormSubmitter(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_currentReply(nullptr)
{
}

FormSubmitter::~FormSubmitter()
{
    if (m_currentReply) {
        m_currentReply->abort();
        m_currentReply->deleteLater();
    }
}

void FormSubmitter::submitForm(const QString &entry1929742366, const QString &entry1454044128)
{
    // Cancel any ongoing request
    if (m_currentReply) {
        m_currentReply->abort();
        m_currentReply->deleteLater();
        m_currentReply = nullptr;
    }

    // Prepare the form data
    QUrlQuery postData;
    postData.addQueryItem("entry.1929742366", entry1929742366);
    postData.addQueryItem("entry.1454044128", entry1454044128);
    postData.addQueryItem("usp", "pp_url");

    // Create the request
    QNetworkRequest request;
    request.setUrl(QUrl(FORM_URL));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    // Set user agent to mimic a browser
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");
    
    // Optional: Set referer header
    request.setRawHeader("Referer", "https://docs.google.com/forms/");

    // Submit the form
    QByteArray data = postData.toString(QUrl::FullyEncoded).toUtf8();
    m_currentReply = m_networkManager->post(request, data);

    // Connect the reply to our slot
    connect(m_currentReply, &QNetworkReply::finished, this, &FormSubmitter::onReplyFinished);

    qDebug() << "Form submission started...";
    qDebug() << "Entry 1929742366:" << entry1929742366;
    qDebug() << "Entry 1454044128:" << entry1454044128;
}

void FormSubmitter::onReplyFinished()
{
    if (!m_currentReply) {
        return;
    }

    // Check for network errors
    if (m_currentReply->error() != QNetworkReply::NoError) {
        QString errorMessage = QString("Network error: %1").arg(m_currentReply->errorString());
        qDebug() << errorMessage;
        emit submissionFailed(errorMessage);
    } else {
        // Check HTTP status code
        int statusCode = m_currentReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "HTTP Status Code:" << statusCode;

        if (statusCode >= 200 && statusCode < 300) {
            qDebug() << "Form submitted successfully!";
            emit submissionSuccessful();
        } else {
            QString errorMessage = QString("HTTP error: %1").arg(statusCode);
            qDebug() << errorMessage;
            emit submissionFailed(errorMessage);
        }
    }

    // Clean up
    m_currentReply->deleteLater();
    m_currentReply = nullptr;
}
