 
#include <QLabel>
class MyLabel: public QLabel
{
Q_OBJECT

public:
    
 MyLabel (QWidget *parent);
 
public slots:
    
 void pintaVerd ();
 void pintaVermell ();
 void sumaUn();
 void sumaQuatre();
 void passaV();
 
};
