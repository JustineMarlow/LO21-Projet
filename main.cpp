#include <QApplication>
#include <QString>
#include <QDebug>
#include "note.h"
#include "relation.h"
#include "noteediteur.h"

int main(int argc, char *argv[])
{
    /*
    try{
    QApplication app(argc, argv);
    QString fichier = QFileDialog::getOpenFileName();
    NotesManager &m=NotesManager::getInstance();
    m.setFilename(fichier);
    m.load();
    Article& a=dynamic_cast<Article&>(m.getNote("id:A2"));
    ArticleEditeur fenetre(a);
    //for(NotesManager::Iterator iterator=NotesManager::getInstance().getIterator(); !iterator.isDone(); iterator.next())
    //{ if (iterator.current().getId()=="id:A2") qDebug()<<"version "<<iterator.current().getVersion()<<" trouvee"<<"\n"; }

    fenetre.show();
    return app.exec();
    }
    catch(NotesException e){qDebug()<<e.getInfo();}
    */


    try{
    QApplication app(argc, argv);
    QString fichier_notes = QFileDialog::getOpenFileName();
    NotesManager &manager_notes=NotesManager::getInstance();
    manager_notes.setFilename(fichier_notes);
    manager_notes.load();
    QString fichier_relations = QFileDialog::getOpenFileName();
    RelationsManager &manager_relations=RelationsManager::getInstance();
    manager_relations.setFilename(fichier_relations);
    manager_relations.load();
    Relation& reference=manager_relations.getRelation("Reference");
    qDebug()<<"ref sur reference recuperee \n";

    QWidget fenetre;
    QLabel* relation_titre= new QLabel(reference.getTitre());
    QLabel* relation_description= new QLabel(reference.getDescription());
    QVBoxLayout *layout=new QVBoxLayout;
    layout-> addWidget(relation_titre);
    layout-> addWidget(relation_description);
    for (unsigned int i=0;i<reference.getNbCouples();i++){
        QLabel* relation_couple= new QLabel(reference.getXCouple(i).getId()+"-->"+reference.getYCouple(i).getId()+" (label : "+reference.getLabelCouple(i)+")");
        layout-> addWidget(relation_couple);
    }
    fenetre.setLayout(layout);
    fenetre.show();
    return app.exec();
    }//fin try

    catch(NotesException e){qDebug()<<e.getInfo();}
}
