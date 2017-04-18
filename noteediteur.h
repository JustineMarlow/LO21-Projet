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
    QLabel* id;
    QLabel* titre1;
    QLineEdit* titre;
    QLabel* date_c;
    QLabel* date_m;
    QLabel* etat;
    QLabel* version;
    QPushButton* bouton;
public:
    explicit NoteEditeur(Note& n, QWidget* parent=0);
    QVBoxLayout* getLayout() {return layout;} //méthode pour récupérer le layout et le modifier en fonction de la sous-classe
    QPushButton* getButton() {return bouton;} //méthode pour récupérer le bouton et le modifier en fonction de la sous-classe
    QLineEdit* getTitle() {return titre;} //méthode pour récupérer le titre et le modifier en fonction de la sous-classe

signals:

public slots:
    void save(){/*existe mais ne fait rien*/}; //méthode virtuelle : à nécessairement définir dans les sous-classes (car enregistrement dépend de la nature de la note)
private slots: //à usage interne
    void activerBouton(QString str="");
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

