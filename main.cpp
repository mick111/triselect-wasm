#include "triselect.h"
#include "welcomedialog.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Triselect w;
    w.show();

    static auto dialog = WelcomeDialog(&w);
    dialog.open();
    Triselect::connect(&dialog, &WelcomeDialog::joueurChanged, &w, &Triselect::setJoueur);
    return a.exec();
}
