#ifndef ARTICLEEDITEUR_H
#define ARTICLEEDITEUR_H
#include <QApplication>
#include <QFileDialog>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDate>
#include <QDateEdit>
#include <note.h>

class NoteEditeur:public QWidget
{
protected:
    Q_OBJECT
    Note* note;
    QVBoxLayout* layout;
    QLabel* id1;
    QLabel* titre1;
    QLabel* date1;
    QLabel* date2;
    QLineEdit* id;
    QLineEdit* titre;
    QDateEdit* date_c;
    QDateEdit* date_m;
    QLabel* etat;
public:
    explicit NoteEditeur(Note& n, QWidget* parent=0);
    QVBoxLayout* getLayout() {return layout;} //méthode pour récupérer le layout et le modifier en fonction de la sous-classe

};



class ArticleEditeur:public NoteEditeur
{
protected:
    QLabel* text1;
    QTextEdit* text;
public:
    ArticleEditeur(Article& a, QWidget* parent=0);

};


#endif // ARTICLEEDITEUR_H

