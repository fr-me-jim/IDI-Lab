#include "MyLCD.h"
// constructor
MyLCD::MyLCD(QWidget *parent)
: QLCDNumber(parent) {
 // Inicialització d'atributs si cal
}
// implementació slots

void MyLCD::refresh_op1(int value) {
    op1 = value;
}
void MyLCD::refresh_op2(int value) {
    op2 = value;
}

void MyLCD::acumula(bool b) {
// Implementació de pintaVerd
    acum = b;
 }
void MyLCD::val_absolut(bool b) {
// Implementació de pintaVerd
    absolut = b;
 }
void MyLCD::reset() {
// Implementació de pintaVerd
    display(0);
 }


void MyLCD::suma() {
// Implementació de pintaVerd
    operacio = 1;
 }
void MyLCD::resta() {
// Implementació de pintaVerd
    operacio = 2;
 }
void MyLCD::multiplicacio() {
// Implementació de pintaVerd
    operacio = 3;
 }
void MyLCD::divisio() {
// Implementació de pintaVerd
    operacio = 4;
 }
void MyLCD::resultat() {
// Implementació de pintaVerd
    if (!acum) result = 0;
    switch(operacio){
        case 1:
             result += (op1 + op2);
             break;
        case 2:
             result += (op1 - op2);
             break;
        case 3:
             result += (op1 * op2);
             break;
        case 4:
             result += (op1 / op2);
             break;
        default:
             break;

    }
    if (absolut && result<0) result *= -1;
    display(result);
 }
