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
    QString fichier = QFileDialog::getOpenFileName();
    NotesManager &m=NotesManager::getInstance();
    m.setFilename(fichier);
    m.load();
    Article& a=dynamic_cast<Article&>(m.getVersionNote("id:A2",1));
    VuePrincipale fenetre(a);

    /*RelationsManager& m=RelationsManager::getInstance();
    m.setFilename(fichier);
    m.load();
    RelationEditeur fenetre;*/

   // manager_relations.createRelation("Reference","La note x fait reference a la note y",true);


    //Article& a=dynamic_cast<Article&>(manager_notes.getNote("id:A1"));
    //ArticleEditeur fenetre(a);


    /*Relation& relation=manager_relations.getRelation("Anteriorite");
    Note& x=manager_notes.getNote("id:A3");
    Note& y=manager_notes.getNote("id:A2");
    relation.addCouple(x,y,"label 4");
    RelationEditeur fenetre2(relation);*/

   // ArticleEditeur fenetre;
    //qDebug()<<"fenetre cree \n";
    //Fichier& f=dynamic_cast<Fichier&>(manager_notes.getNote("id:F2"));
    //FichierEditeur fenetre(f);
    //for(NotesManager::Iterator iterator=NotesManager::getInstance().getIterator(); !iterator.isDone(); iterator.next())
    //{ if (iterator.current().getId()=="id:A2") qDebug()<<"version "<<iterator.current().getVersion()<<" trouvee"<<"\n"; }

    fenetre.show();
    return app.exec();
    }
    catch(NotesException e){qDebug()<<e.getInfo();}



    /*try{
    QApplication app(argc, argv);
    QString fichier_notes = QFileDialog::getOpenFileName();
    NotesManager& manager_notes=NotesManager::getInstance();
    manager_notes.setFilename(fichier_notes);
    manager_notes.load();
    QString fichier_relations = QFileDialog::getOpenFileName();
    RelationsManager& manager_relations=RelationsManager::getInstance();
    manager_relations.setFilename(fichier_relations);
    manager_relations.load();

    Relation & r1=manager_relations.createRelation("New Relation", "nouvelle relation",false);
    Relation & r2=manager_relations.createRelation("New Relation bis", "nouvelle relation bis",false);
    Relation & r=manager_relations.getRelation("Reference");
    RelationEditeur fenetre(r);*/


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


    catch(NotesException e){qDebug()<<e.getInfo();}*/

 }
