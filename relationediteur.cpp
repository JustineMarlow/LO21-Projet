#include "relation.h"
#include "relationediteur.h"
#include <QDebug>

//RelationEdieur d'une relation existante
RelationEditeur::RelationEditeur(Relation& r, QWidget* parent):QWidget(parent),relation(&r)
{
    titre1 = new QLabel("Titre",this);
    titre = new QLineEdit(this);
    description1 = new QLabel("Description",this);
    description = new QTextEdit(this);
    orientee = new QCheckBox("Relation orientee", this);
    titre->setText(r.getTitre());
    description->setText(r.getDescription());
    if (relation->IsOriente()) orientee->setChecked(true);
    else orientee->setChecked(false);
    orientee->setDisabled(true);
    bouton_save = new QPushButton("Sauver la relation",this);
    bouton_save->setDisabled(true);
    bouton_delete = new QPushButton("Supprimer la relation",this);
    couple1 = new QLabel("Edition des couples :",this);

    if (r.getTitre()=="Reference")
    {
        //la relation n'est ni modifiable ni supprimable
        titre->setDisabled(true);
        description->setDisabled(true);
        bouton_delete->setDisabled(true);
    }

    QObject::connect(bouton_save, SIGNAL(clicked()), this, SLOT(save()));
    QObject::connect(titre, SIGNAL(textChanged(QString)), this, SLOT(activerBouton(QString)));
    QObject::connect(description, SIGNAL(textChanged()), this, SLOT(activerBouton()));
    QObject::connect(bouton_delete, SIGNAL(clicked()), this, SLOT(delete_relation()));

    layout = new QVBoxLayout(this);
    layout-> addWidget(titre1);
    layout-> addWidget(titre);
    layout-> addWidget(description1);
    layout-> addWidget(description);
    layout-> addWidget(orientee);
    layout-> addWidget(bouton_save);
    layout-> addWidget(bouton_delete);
    layout-> addWidget(couple1);

    box = new QHBoxLayout*[relation->getNbCouples()];
    idX = new QLabel*[relation->getNbCouples()];
    idY = new QLabel*[relation->getNbCouples()];
    edit_label = new QLineEdit*[relation->getNbCouples()];
    buttons_edit = new QPushButton*[relation->getNbCouples()];
    buttons_remove = new QPushButton*[relation->getNbCouples()];
    mapper_label = new QSignalMapper(this);
    connect(mapper_label, SIGNAL(mapped(int)), this, SLOT(set_label(int)));
    mapper_remove = new QSignalMapper(this);
    connect(mapper_remove, SIGNAL(mapped(int)), this, SLOT(remove_couple(int)));

    for (int i=0; i<relation->getNbCouples(); i++)
    {
        box[i]= new QHBoxLayout();
        idX[i]= new QLabel(relation->getXCouple(i).getId(),this);
        idY[i]= new QLabel(relation->getYCouple(i).getId(),this);
        box[i]->addWidget(idX[i]);
        box[i]->addWidget(idY[i]);
        if(relation->getTitre()!="Reference")
        {
            //on peut modifier les labels et supprimer les couples
            edit_label[i] = new QLineEdit(this);
            edit_label[i]->setText(relation->getLabelCouple(i));
            buttons_edit[i]= new QPushButton("Sauver le label", this);
            connect(buttons_edit[i], SIGNAL(clicked()), mapper_label, SLOT(map()));
            mapper_label->setMapping(buttons_edit[i],i);
            qDebug()<<"arrivee ici";
            buttons_remove[i]= new QPushButton("Supprimer le couple", this);
            connect(buttons_remove[i], SIGNAL(clicked()), mapper_remove, SLOT(map()));
            mapper_remove->setMapping(buttons_remove[i],i);
            box[i]->addWidget(edit_label[i]);
            box[i]->addWidget(buttons_edit[i]);
            box[i]->addWidget(buttons_remove[i]);
        }
        layout->addLayout(box[i]);
    }

    couple2 = new QLabel("Ajout d'un nouveau couple :",this);
    layout-> addWidget(couple2);

    new_couple = new QHBoxLayout(this);
    liste_x = new QListView();
    model_x = new QStringListModel();
    QStringList list_x;
    liste_y = new QListView();
    model_y = new QStringListModel();
    QStringList list_y;
    for(NotesManager::Iterator iterator=NotesManager::getInstance().getIterator(); !iterator.isDone(); iterator.next())
        if (iterator.current().IsLast()) { list_x << iterator.current().getId(); list_y << iterator.current().getId(); }
    model_x->setStringList(list_x);
    liste_x->setModel(model_x);
    model_y->setStringList(list_y);
    liste_y->setModel(model_y);
    new_label = new QLineEdit(this);
    add_couple = new QPushButton("Ajouter le couple", this);
    QObject::connect(add_couple, SIGNAL(clicked()), this, SLOT(add_new_couple()));
    new_couple->addWidget(liste_x);
    new_couple->addWidget(liste_y);
    new_couple->addWidget(new_label);
    new_couple->addWidget(add_couple);
    layout->addLayout(new_couple);
}

//RelationEditeur d'une nouvelle relation
RelationEditeur::RelationEditeur(QWidget *parent):relation(0)
{
    titre1 = new QLabel("Titre",this);
    titre = new QLineEdit(this);
    description1 = new QLabel("Description",this);
    description = new QTextEdit(this);
    orientee = new QCheckBox("Relation orientee", this);
    bouton_save = new QPushButton("Sauver",this);
    bouton_save->setDisabled(true);

    QObject::connect(titre, SIGNAL(textChanged(QString)), this, SLOT(activerBouton(QString)));
    QObject::connect(description, SIGNAL(textChanged()), this, SLOT(activerBouton()));
    QObject::connect(bouton_save, SIGNAL(clicked()), this, SLOT(create()));

    layout = new QVBoxLayout(this);
    layout-> addWidget(titre1);
    layout-> addWidget(titre);
    layout-> addWidget(description1);
    layout-> addWidget(description);
    layout-> addWidget(orientee);
    layout-> addWidget(bouton_save);
}

void RelationEditeur::create()
{
    //on créé un nouvel objet de type Relation
    RelationsManager &m=RelationsManager::getInstance();
    m.createRelation(this->titre->text(),this->description->toPlainText(),orientee->isChecked());
    QMessageBox::information(this,"Sauvegarde","Sauvegarde de la nouvelle relation");
    bouton_save->setDisabled(true);
    titre->setDisabled(true);
    description->setDisabled(true);
    orientee->setDisabled(true);
}

void RelationEditeur::save()
{
    relation->set_titre(titre->text());
    relation->set_description(description->toPlainText());
    bouton_save->setDisabled(true);
    QMessageBox::information(this,"Sauvegarde","Sauvegarde de la relation");
}

void RelationEditeur::delete_relation()
{
    //on bloque toute nouvelle action
    bouton_delete->setDisabled(true);
    titre->setDisabled(true);
    description->setDisabled(true);
    for (unsigned int i=0; i<relation->getNbCouples(); i++)
    {
        edit_label[i]->setDisabled(true);
        buttons_edit[i]->setDisabled(true);
        buttons_remove[i]->setDisabled(true);
    }
    liste_x->setDisabled(true);
    liste_y->setDisabled(true);
    new_label->setDisabled(true);
    add_couple->setDisabled(true);
    RelationsManager &manager_relations=RelationsManager::getInstance();
    manager_relations.deleteRelation(*relation);
    QMessageBox::information(this,"Suppression","Suppression de la relation");

}

void RelationEditeur::activerBouton(QString str)
{
   bouton_save->setEnabled(true);
}

void RelationEditeur::add_new_couple()
{
   //on bloque toute nouvelle action
   for (unsigned int k=0; k<relation->getNbCouples(); k++)
   {
        edit_label[k]->setDisabled(true);
        buttons_edit[k]->setDisabled(true);
        buttons_remove[k]->setDisabled(true);
   }
   liste_x->setDisabled(true);
   liste_y->setDisabled(true);
   new_label->setDisabled(true);
   add_couple->setDisabled(true);
   bouton_delete->setDisabled(true);
   titre->setDisabled(true);
   description->setDisabled(true);

   //on ajoute le nouveau couple
   QModelIndexList listeSelection_x = liste_x->selectionModel()->selectedIndexes();
   QString id_x = model_x->data(listeSelection_x[0],Qt::DisplayRole).toString();
   QModelIndexList listeSelection_y = liste_y->selectionModel()->selectedIndexes();
   QString id_y = model_y->data(listeSelection_y[0],Qt::DisplayRole).toString();
   NotesManager &manager_notes=NotesManager::getInstance();
   Note& x = manager_notes.getNote(id_x);
   Note& y = manager_notes.getNote(id_y);
   relation->addCouple(x,y,new_label->text());
   QMessageBox::information(this,"Ajout","Ajout du nouveau couple");

}

void RelationEditeur::set_label(int i)
{
   Note& x=relation->getXCouple(i);
   Note& y=relation->getYCouple(i);
   relation->set_label_couple(x,y,edit_label[i]->text());
   QMessageBox::information(this,"Edition","Edition label");
}

void RelationEditeur::remove_couple(int i)
{
   //on bloque toute nouvelle action
   bouton_delete->setDisabled(true);
   titre->setDisabled(true);
   description->setDisabled(true);
   for (unsigned int k=0; k<relation->getNbCouples(); k++)
   {
       edit_label[k]->setDisabled(true);
       buttons_edit[k]->setDisabled(true);
       buttons_remove[k]->setDisabled(true);
   }
   liste_x->setDisabled(true);
   liste_y->setDisabled(true);
   new_label->setDisabled(true);
   add_couple->setDisabled(true);
   //on supprime le couple
   Note& x=relation->getXCouple(i);
   Note& y=relation->getYCouple(i);
   relation->removeCouple(x,y);
   QMessageBox::information(this,"Suppression","Suppression du couple");
}
