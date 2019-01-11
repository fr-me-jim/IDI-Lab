
#include <QLCDNumber>
class MyLCD: public QLCDNumber
{
Q_OBJECT

public:

MyLCD (QWidget *parent);
int op1 = 0, op2 = 0;
int operacio = 0, result = 0;
bool acum = false, absolut = false;

public slots:

void refresh_op1(int);
void refresh_op2(int);

void acumula(bool);
void val_absolut(bool);
void reset();

void suma();
void resta();
void multiplicacio();
void divisio();
void resultat();

};
