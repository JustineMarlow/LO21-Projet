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

    //Article& a=dynamic_cast<Article&>(manager_notes.getNote("id:A1"));
    //ArticleEditeur fenetre(a);


    Relation& relation=manager_relations.getRelation("Anteriorite");
    RelationEditeur fenetre(relation);

    //Fichier& f=dynamic_cast<Fichier&>(manager_notes.getNote("id:F2"));
    //FichierEditeur fenetre(f);
    //for(NotesManager::Iterator iterator=NotesManager::getInstance().getIterator(); !iterator.isDone(); iterator.next())
    //{ if (iterator.current().getId()=="id:A2") qDebug()<<"version "<<iterator.current().getVersion()<<" trouvee"<<"\n"; }

    fenetre.show();
    return app.exec();
    }
    catch(NotesException e){qDebug()<<e.getInfo();}
}
