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
#include<QDockWidget>
#include<QMainWindow>
#include<QMenuBar>
#include<QMenu>
#include<QShortcut>
#include<QKeySequence>
#include <note.h>
#include <noteediteur.h>
#include<relation.h>
#include<relationediteur.h>


class InterfaceException{
public:
    InterfaceException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};

class VuePrincipale : public QMainWindow{
    Q_OBJECT
public:
    VuePrincipale(Note* n);

private slots:
     void showRelations(); //bouton d'accès à la vue secondaire (gestion & visualisation des relations)
     void afficageArbo(); //gère la partie droite (masque ou affiche)

private:
    //Partie gauche

    //Partie centrale
    Note* note;
    NoteEditeur* noteEdit;

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
    RelationEditeur relations;
    QPushButton* quitter;

};

#endif // INTERFACE_H
