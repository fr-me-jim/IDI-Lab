#include <QApplication>
#include "Calculadora.h"
int main(int argc, char **argv)
{
    QApplication app(argc,argv);
    Calculadora form;
    form.show();
    return app.exec();
}
