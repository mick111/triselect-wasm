#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>

namespace Ui
{
    class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT
    // Create a signal to notify when the player name changes
signals:
    void joueurChanged(const QString &joueur);

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    ~WelcomeDialog();

private:
    Ui::WelcomeDialog *ui;
private slots:
    void on_lineEdit_textChanged(const QString &text);
};

#endif // WELCOMEDIALOG_H
