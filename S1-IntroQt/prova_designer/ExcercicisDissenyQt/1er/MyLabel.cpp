#include "MyLabel.h"
// constructor
MyLabel::MyLabel(QWidget *parent)
: QLabel(parent) {
 // Inicialització d'atributs si cal
}
// implementació slots
void MyLabel::pintaVerd() {
// Implementació de pintaVerd
    setStyleSheet("background-color:rgb(0, 255, 0);");
 }

void MyLabel::pintaVermell() {
// Implementació de pintaVermell
    setStyleSheet("background-color:rgb(255, 0, 0);");
 }
void MyLabel::sumaUn() {
// Implementació de pintaVerd
    QString origin = text();
    int i = origin.toInt();
    i += 1;
    origin = QString::number(i);
    setText(origin);
 }

void MyLabel::sumaQuatre() {
// Implementació de pintaVermell
    QString origin = text();
    int i = origin.toInt();
    i += 4;
    origin = QString::number(i);
    setText(origin);
 }

void MyLabel::passaV() {
// Implementació de pintaVermell
    QString origin = text();
    int i = origin.toInt();
    i += 1;
    origin = QString::number(i);
    setText(origin);
 }
