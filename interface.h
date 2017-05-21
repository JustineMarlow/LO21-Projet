#ifndef INTERFACE_H
#define INTERFACE_H

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
#include <QMessageBox>
#include<QtGui>
#include<QGroupBox>
#include <note.h>
#include <noteediteur.h>

class VuePrincipale : public QWidget{
    Q_OBJECT
public:
    VuePrincipale(Article& a);

private slots:
     void showRelations(); //bouton d'accès à la vue secondaire (gestion & visualisation des relations)
     void afficageArbo(); //gère la partie droite (masque ou affiche)

private:
    //Partie gauche

    //Partie centrale
    Article& article;
    ArticleEditeur note;


    //Partie droite
    bool arboVisible;
    QGroupBox* droite;
    QLabel* relation_titre;
    QLabel* relation_description;
    QPushButton* relation_details;
    QPushButton* arborescence;

};

class VueSecondaire : public QWidget{
public :
    VueSecondaire();

private :
    //Article& article;
    QPushButton* quitter;

};

#endif // INTERFACE_H
