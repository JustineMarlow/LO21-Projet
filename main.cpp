#include <QApplication>
#include <QString>
#include <QDebug>
#include "note.h"
#include "relation.h"
#include "noteediteur.h"
#include "interface.h"

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
    VuePrincipale fenetre(a);


    //ArticleEditeur fenetre(a);
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

    Relation & new_relation=manager_relations.createRelation("relation 2","pour tester save",true);
    new_relation.addCouple(manager_notes.getNote("id:A2"),manager_notes.getNote("id:A1"),"new couple for new relation");

    QWidget fenetre;
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
}
