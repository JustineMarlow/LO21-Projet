#include <QApplication>
#include <QString>
#include <QDebug>
#include "note.h"
#include "relation.h"
#include "noteediteur.h"
#include "interface.h"
#include "relationediteur.h"

int main(int argc, char *argv[])
{
    try{
    QApplication app(argc, argv);
    RelationsManager &manager_relations=RelationsManager::getInstance();
    manager_relations.createRelation("Reference","La note x fait reference a la note y",true);

    NotesManager &manager_notes=NotesManager::getInstance();
    manager_notes.setFilename("/home/marlow/Bureau/Projet/Notes.xml");
    manager_notes.load();

    manager_relations.setFilename("/home/marlow/Bureau/Projet/Relations.xml");
    manager_relations.load();

    Article& a=dynamic_cast<Article&>(manager_notes.getNote("id:A1"));
    ArticleEditeur fenetre(a);

    Relation& ref=manager_relations.getRelation("Reference");
    qDebug()<<"nb couples = "<<ref.getNbCouples()<<"\n";
    for (unsigned int i=0; i<ref.getNbCouples(); i++)
        qDebug()<<ref.getXCouple(i).getId()<<" -> "<<ref.getYCouple(i).getId()<<"\n";
    //Fichier& f=dynamic_cast<Fichier&>(manager_notes.getNote("id:F2"));
    //FichierEditeur fenetre;
    //for(NotesManager::Iterator iterator=NotesManager::getInstance().getIterator(); !iterator.isDone(); iterator.next())
    //{ if (iterator.current().getId()=="id:A2") qDebug()<<"version "<<iterator.current().getVersion()<<" trouvee"<<"\n"; }

    fenetre.show();
    return app.exec();
    }
    catch(NotesException e){qDebug()<<e.getInfo();}

/*
    try{
    QApplication app(argc, argv);
    //QString fichier_notes = QFileDialog::getOpenFileName();
    NotesManager &manager_notes=NotesManager::getInstance();
    manager_notes.setFilename("/home/marlow/Bureau/Projet/Notes.xml");
    manager_notes.load();
    //QString fichier_relations = QFileDialog::getOpenFileName();
    RelationsManager &manager_relations=RelationsManager::getInstance();
    manager_relations.setFilename("/home/marlow/Bureau/Projet/Relations.xml");
    manager_relations.load();

    Relation & r1=manager_relations.createRelation("New Relation", "nouvelle relation",false);
    RelationEditeur fenetre(r1);

    //idee pour afficher toutes les relations
    /*
    QVBoxLayout *layout=new QVBoxLayout;
    for (RelationsManager::Iterator iterator=RelationsManager::getInstance().getIterator(); !iterator.isDone(); iterator.next())
    {
        QLabel* relation_titre= new QLabel(iterator.current().getTitre());
        QLabel* relation_description= new QLabel(iterator.current().getDescription());
        layout-> addWidget(relation_titre);
        layout-> addWidget(relation_description);
        for (unsigned int i=0;i<iterator.current().getNbCouples();i++){
            QLabel* relation_couple= new QLabel(iterator.current().getXCouple(i).getId()+"-->"+iterator.current().getYCouple(i).getId()+" (label : "+iterator.current().getLabelCouple(i)+")");
            layout-> addWidget(relation_couple);
        }
    }
    fenetre.setLayout(layout);


    fenetre.show();
    return app.exec();
    }

    catch(NotesException e){qDebug()<<e.getInfo();}
*/
}
