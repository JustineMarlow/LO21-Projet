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
    //PARTIE DU MAIN A NE PAS CHANGER HORMIS LES CHEMINS VERS LES FICHIERS
    QApplication app(argc, argv);
    RelationsManager &manager_relations=RelationsManager::getInstance();
    manager_relations.createRelation("Reference","La note x fait reference a la note y",true);


    NotesManager &manager_notes=NotesManager::getInstance();
    manager_notes.setFilename("/home/camille/Documents/UTC/HU04/LO21/Projet/Projet/Notes.xml");
    //manager_notes.setFilename("/home/marlow/Bureau/Projet/Notes.xml");
    manager_notes.load();

    manager_relations.setFilename("/home/camille/Documents/UTC/HU04/LO21/Projet/Projet/Relations.xml");
    //manager_relations.setFilename("/home/marlow/Bureau/Projet/Relations.xml");
    manager_relations.load();

    //ICI ON PEUT OUVRIR UNE NOTE OU UNE RELATION
    //Article& a=dynamic_cast<Article&>(manager_notes.getNote("id:A1"));
    //ArticleEditeur fenetre(a);


    /*Relation& relation=manager_relations.getRelation("Anteriorite");
    Note& x=manager_notes.getNote("id:A3");
    Note& y=manager_notes.getNote("id:A2");
    relation.addCouple(x,y,"label 4");
    RelationEditeur fenetre(relation);*/

    //Fichier& f=dynamic_cast<Fichier&>(manager_notes.getNote("id:F2"));
    //FichierEditeur fenetre(f);
    //for(NotesManager::Iterator iterator=NotesManager::getInstance().getIterator(); !iterator.isDone(); iterator.next())
    //{ if (iterator.current().getId()=="id:A2") qDebug()<<"version "<<iterator.current().getVersion()<<" trouvee"<<"\n"; }


    Article& a=dynamic_cast<Article&>(manager_notes.getNote("id:A1"));
    VuePrincipale fenetre(&a);
    fenetre.show();
    return app.exec();
    }
    catch(NotesException e){qDebug()<<e.getInfo();}
    catch(InterfaceException e){qDebug()<<e.getInfo();}
}
