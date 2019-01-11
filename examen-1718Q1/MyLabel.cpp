
#include "MyLabel.h"
#include <iostream>

using namespace std;

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{ 
    red = green = blue = 0;
}
void MyLabel::setRed(int r)
{
    red = r;
    setColors();
}
void MyLabel::setGreen(int g)
{
    green = g;
    setColors();
}
void MyLabel::setBlue(int b)
{
    blue = b;
    setColors();
}
void MyLabel::setColors()
{
    this -> setStyleSheet(QString("MyLabel {background-color: rgb(%1,%2,%3);}").arg(red).arg(green).arg(blue));
    emit setTerraColors(red,green,blue);
}

