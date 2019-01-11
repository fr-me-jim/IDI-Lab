#include "ui_Calculadora.h"
class Calculadora:public QWidget
{
    Q_OBJECT
    
public:
    Calculadora (QWidget *parent=0);
private:
    Ui::Calculadora ui;
};
