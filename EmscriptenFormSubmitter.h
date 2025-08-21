#ifndef EMSCRIPTENFORMSUBMITTER_H
#define EMSCRIPTENFORMSUBMITTER_H

#include <QObject>
#include <QString>

#ifdef __EMSCRIPTEN__
#include <emscripten/fetch.h>
#endif

enum class SubmissionMethod
{
    PopupWindow,
    IframeHidden,
    FetchAPI,
    XMLHttpRequest
};

class EmscriptenFormSubmitter : public QObject
{
    Q_OBJECT

public:
    explicit EmscriptenFormSubmitter(QObject *parent = nullptr);
    ~EmscriptenFormSubmitter();

    // Method to submit the form with the two entries
    void submitForm(const QString &entry1929742366, const QString &entry1454044128, SubmissionMethod method = SubmissionMethod::IframeHidden);

signals:
    // Signals to notify about submission status
    void submissionSuccessful();
    void submissionFailed(const QString &errorMessage);

private:
#ifdef __EMSCRIPTEN__
    // Static callback functions for Emscripten fetch
    static void onFetchSuccess(emscripten_fetch_t *fetch);
    static void onFetchError(emscripten_fetch_t *fetch);

    // Store instance pointer for callbacks
    static EmscriptenFormSubmitter *s_currentInstance;
#endif

    // Google Form URL
    static const QString FORM_URL;

    // Helper method to create form data
    std::string createFormData(const QString &entry1929742366, const QString &entry1454044128);

    // Different submission methods
    void submitWithPopup(const QString &entry1929742366, const QString &entry1454044128);
    void submitWithIframe(const QString &entry1929742366, const QString &entry1454044128);
    void submitWithFetch(const QString &entry1929742366, const QString &entry1454044128);
    void submitWithXMLHttpRequest(const QString &entry1929742366, const QString &entry1454044128);
};

#endif // EMSCRIPTENFORMSUBMITTER_H
