#include "welcomedialog.h"
#include "ui_welcomedialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);
    setWindowTitle("Bienvenue sur Triselect");
    setModal(true);
    ui->label->setText("Veuillez entrer votre nom pour commencer.");
    ui->lineEdit->setPlaceholderText("Entrez votre nom");
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &WelcomeDialog::on_lineEdit_textChanged);
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::on_lineEdit_textChanged(const QString &text)
{
    emit joueurChanged(text);
}