
#include <QLabel>

class MyLabel: public QLabel
{ 
   Q_OBJECT   

   public:
     MyLabel (QWidget *parent);
     void setColors();

   public slots:    
     void setRed(int r); 
     void setGreen(int g);
     void setBlue(int b);

   signals:         
     void setTerraColors (int red, int green, int blue);
   private:
    int red, green, blue;
};
