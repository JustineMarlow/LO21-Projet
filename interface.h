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
#include <note.h>
#include <noteediteur.h>

class VuePrincipale : public QWidget{
    Q_OBJECT
public:
    VuePrincipale(Article& a);

private slots:
     //void showRelations(); //bouton d'accès à la vue secondaire (gestion & visualisation des relations)

private:
    //Partie gauche

    //Partie centrale
    Article& article;
    ArticleEditeur note;

    //Partie droite
    QLabel* relation_titre;
    QLabel* relation_description;
    QPushButton* relation_details;
    /*QVBoxLayout* rightLayout=new QVBoxLayout;
    rightLayout->addWidget(relation_titre);
    rightLayout-> addWidget(relation_description);*/ //CONSTRUCTEUR

};

#endif // INTERFACE_H
