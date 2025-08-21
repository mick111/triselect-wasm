#include "EmscriptenFormSubmitter.h"
#include <QDebug>
#include <QUrl>
#include <string>
#include <QUrlQuery>

#ifdef __EMSCRIPTEN__
#include <emscripten/fetch.h>
#include <emscripten.h>
#include <cstring>
#endif

// Google Form URL (direct, no proxy)
const QString EmscriptenFormSubmitter::FORM_URL = "https://docs.google.com/forms/d/112YW8Dh1F4PeaCMEqtNn_M7QZKovDXNgwgUuSJtyjgk/formResponse";

#ifdef __EMSCRIPTEN__
// Static instance pointer for callbacks
EmscriptenFormSubmitter *EmscriptenFormSubmitter::s_currentInstance = nullptr;
#endif

EmscriptenFormSubmitter::EmscriptenFormSubmitter(QObject *parent)
    : QObject(parent)
{
#ifdef __EMSCRIPTEN__
    s_currentInstance = this;
#endif
}

EmscriptenFormSubmitter::~EmscriptenFormSubmitter()
{
#ifdef __EMSCRIPTEN__
    if (s_currentInstance == this)
    {
        s_currentInstance = nullptr;
    }
#endif
}

void EmscriptenFormSubmitter::submitForm(const QString &entry1929742366, const QString &entry1454044128, SubmissionMethod method)
{
    qDebug() << "Form submission started...";
    qDebug() << "Entry 1929742366:" << entry1929742366;
    qDebug() << "Entry 1454044128:" << entry1454044128;
    qDebug() << "Method:" << static_cast<int>(method);

#ifdef __EMSCRIPTEN__
    switch (method)
    {
    case SubmissionMethod::PopupWindow:
        submitWithPopup(entry1929742366, entry1454044128);
        break;
    case SubmissionMethod::IframeHidden:
        submitWithIframe(entry1929742366, entry1454044128);
        break;
    case SubmissionMethod::FetchAPI:
        submitWithFetch(entry1929742366, entry1454044128);
        break;
    case SubmissionMethod::XMLHttpRequest:
        submitWithXMLHttpRequest(entry1929742366, entry1454044128);
        break;
    }
#else
    // Fallback for non-Emscripten builds
    qDebug() << "Emscripten not available. This is a stub implementation.";
    emit submissionFailed("Emscripten not available in this build");
#endif
}

void EmscriptenFormSubmitter::submitWithPopup(const QString &entry1929742366, const QString &entry1454044128)
{
#ifdef __EMSCRIPTEN__
    std::string entry1Encoded = QUrl::toPercentEncoding(entry1929742366).toStdString();
    std::string entry2Encoded = QUrl::toPercentEncoding(entry1454044128).toStdString();

    std::string jsCode = R"(
        (function() {
            var url = 'https://docs.google.com/forms/d/112YW8Dh1F4PeaCMEqtNn_M7QZKovDXNgwgUuSJtyjgk/formResponse' +
                     '?entry.1929742366=)" +
                         entry1Encoded + R"(' +
                     '&entry.1454044128=)" +
                         entry2Encoded + R"(' +
                     '&usp=pp_url' +
                     '&submit=Submit';
            
            var popup = window.open(url, 'formSubmit', 'width=1,height=1,left=9999,top=9999,toolbar=no,menubar=no,scrollbars=no,resizable=no,location=no,directories=no,status=no');
            
            setTimeout(function() {
                if (popup && !popup.closed) {
                    popup.close();
                }
            }, 2000);
            
            return true;
        })();
    )";

    emscripten_run_script(jsCode.c_str());
    qDebug() << "Form submitted via popup method";
    emit submissionSuccessful();
#endif
}

void EmscriptenFormSubmitter::submitWithIframe(const QString &entry1929742366, const QString &entry1454044128)
{
#ifdef __EMSCRIPTEN__
    std::string entry1Encoded = QUrl::toPercentEncoding(entry1929742366).toStdString();
    std::string entry2Encoded = QUrl::toPercentEncoding(entry1454044128).toStdString();

    std::string jsCode = R"(
        (function() {
            var iframe = document.createElement('iframe');
            iframe.style.display = 'none';
            iframe.style.width = '1px';
            iframe.style.height = '1px';
            iframe.style.border = 'none';
            
            var url = 'https://docs.google.com/forms/d/112YW8Dh1F4PeaCMEqtNn_M7QZKovDXNgwgUuSJtyjgk/formResponse' +
                     '?entry.1929742366=)" +
                         entry1Encoded + R"(' +
                     '&entry.1454044128=)" +
                         entry2Encoded + R"(' +
                     '&usp=pp_url' +
                     '&submit=Submit';
            
            iframe.src = url;
            document.body.appendChild(iframe);
            
            setTimeout(function() {
                if (iframe.parentNode) {
                    document.body.removeChild(iframe);
                }
            }, 3000);
            
            return true;
        })();
    )";

    emscripten_run_script(jsCode.c_str());
    qDebug() << "Form submitted via iframe method";
    emit submissionSuccessful();
#endif
}

void EmscriptenFormSubmitter::submitWithFetch(const QString &entry1929742366, const QString &entry1454044128)
{
#ifdef __EMSCRIPTEN__
    std::string formData = createFormData(entry1929742366, entry1454044128);
    std::string url = FORM_URL.toStdString();

    // Set up fetch attributes
    static emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);

    strcpy(attr.requestMethod, "POST");

    const char *headers[] = {
        "Content-Type", "application/x-www-form-urlencoded",
        0};
    attr.requestHeaders = headers;

    attr.requestData = formData.c_str();
    attr.requestDataSize = formData.length();

    attr.onsuccess = onFetchSuccess;
    attr.onerror = onFetchError;

    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;

    emscripten_fetch(&attr, url.c_str());
    qDebug() << "Form submitted via Fetch API method";
#endif
}

void EmscriptenFormSubmitter::submitWithXMLHttpRequest(const QString &entry1929742366, const QString &entry1454044128)
{
#ifdef __EMSCRIPTEN__
    std::string entry1Encoded = QUrl::toPercentEncoding(entry1929742366).toStdString();
    std::string entry2Encoded = QUrl::toPercentEncoding(entry1454044128).toStdString();

    std::string jsCode = R"(
        (function() {
            var xhr = new XMLHttpRequest();
            var url = 'https://docs.google.com/forms/d/112YW8Dh1F4PeaCMEqtNn_M7QZKovDXNgwgUuSJtyjgk/formResponse';
            var formData = 'entry.1929742366=)" +
                         entry1Encoded + R"(&entry.1454044128=)" + entry2Encoded + R"(&usp=pp_url';
            
            xhr.open('POST', url, true);
            xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
            
            xhr.onreadystatechange = function() {
                if (xhr.readyState === 4) {
                    console.log('XMLHttpRequest completed with status:', xhr.status);
                }
            };
            
            xhr.send(formData);
            return true;
        })();
    )";

    emscripten_run_script(jsCode.c_str());
    qDebug() << "Form submitted via XMLHttpRequest method";
    emit submissionSuccessful();
#endif
}

std::string EmscriptenFormSubmitter::createFormData(const QString &entry1929742366, const QString &entry1454044128)
{
    // URL encode the form data
    QUrl url;
    QUrlQuery query;
    query.addQueryItem("entry.1929742366", entry1929742366);
    query.addQueryItem("entry.1454044128", entry1454044128);
    query.addQueryItem("usp", "pp_url");

    return query.toString(QUrl::FullyEncoded).toStdString();
}

#ifdef __EMSCRIPTEN__
void EmscriptenFormSubmitter::onFetchSuccess(emscripten_fetch_t *fetch)
{
    if (!s_currentInstance)
    {
        emscripten_fetch_close(fetch);
        return;
    }

    qDebug() << "HTTP Status Code:" << fetch->status;
    qDebug() << "Response received, length:" << fetch->numBytes;

    // Check HTTP status code
    if (fetch->status >= 200 && fetch->status < 300)
    {
        qDebug() << "Form submitted successfully!";
        emit s_currentInstance->submissionSuccessful();
    }
    else
    {
        QString errorMessage = QString("HTTP error: %1").arg(fetch->status);
        qDebug() << errorMessage;
        emit s_currentInstance->submissionFailed(errorMessage);
    }

    // Clean up the fetch
    emscripten_fetch_close(fetch);
}

void EmscriptenFormSubmitter::onFetchError(emscripten_fetch_t *fetch)
{
    if (!s_currentInstance)
    {
        emscripten_fetch_close(fetch);
        return;
    }

    QString errorMessage = QString("Network error occurred during form submission. Status: %1").arg(fetch->status);
    qDebug() << errorMessage;

    emit s_currentInstance->submissionFailed(errorMessage);

    // Clean up the fetch
    emscripten_fetch_close(fetch);
}
#endif
