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
#include<QTreeWidget>
#include<QTreeWidgetItem>
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
    void affichage_central();
    void affichage_droit();

private slots:
     void showRelations(); //bouton d'accès à la vue secondaire (gestion & visualisation des relations)
     void afficageArbo();  //gère la partie droite (masque ou affiche l'arborescence)
     void afficher(QTreeWidgetItem* item,int i);

private:
    QWidget* zoneCentrale;
    QHBoxLayout* layoutPrincipal;

    //Partie gauche
    QGroupBox* gauche;

    //Partie centrale
    QGroupBox* centre;
    Note* note;
    NoteEditeur* noteEdit;

    //Partie droite
    bool arboVisible;
    QTreeWidget* liste_relations;
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
    RelationsManager* manager;
    RelationEditeur relations;
    QPushButton* quitter;

    //colonne gauche
    QHBoxLayout** liste_relations;
    QPushButton** relation_titre;
    QLabel** relation_description;
    //QPushButton** editer;
    //QSignalMapper** mapper_openRelation;
};

class ArbreRelations : public QTreeWidgetItem{
    QString value;
private :
    QString getValue() const {return value;}
    ArbreRelations(const QString& text, const QString& v="Undefined", int type=Type) :
        QTreeWidgetItem(QStringList(text), type), value(v){}
};

#endif // INTERFACE_H
