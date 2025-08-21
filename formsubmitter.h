#ifndef FORMSUBMITTER_H
#define FORMSUBMITTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

class FormSubmitter : public QObject
{
    Q_OBJECT

public:
    explicit FormSubmitter(QObject *parent = nullptr);
    ~FormSubmitter();

    // Method to submit the form with the two entries
    void submitForm(const QString &entry1929742366, const QString &entry1454044128);

signals:
    // Signals to notify about submission status
    void submissionSuccessful();
    void submissionFailed(const QString &errorMessage);

private slots:
    void onReplyFinished();

private:
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_currentReply;
    
    // Google Form URL
    static const QString FORM_URL;
};

#endif // FORMSUBMITTER_H
