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
    if (r.IsOriente()) orientee->setChecked(true);
    else orientee->setChecked(false);
    orientee->setDisabled(true);
    bouton_save = new QPushButton("Sauver",this);
    bouton_save->setDisabled(true);
    bouton_delete = new QPushButton("Supprimer",this);

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
    //QObject::connect(bouton_couple, SIGNAL(clicked()), this, SLOT(couple()));

    layout = new QVBoxLayout(this);
    layout-> addWidget(titre1);
    layout-> addWidget(titre);
    layout-> addWidget(description1);
    layout-> addWidget(description);
    layout-> addWidget(orientee);
    layout-> addWidget(bouton_save);
    layout-> addWidget(bouton_delete);
}

/*
//RelationEditeur d'une nouvelle relation
RelationEditeur::RelationEditeur(QWidget *parent)
{
    titre1 = new QLabel("Titre",this);
    titre = new QLineEdit(this);
    description1 = new QLabel("Description",this);
    description = new QTextEdit(this);
    bouton = new QPushButton("Sauver",this);
    bouton->setDisabled(true);
    QObject::connect(bouton, SIGNAL(clicked()), this, SLOT(save()));
    layout = new QVBoxLayout(this);
    layout-> addWidget(titre1);
    layout-> addWidget(titre);
    layout-> addWidget(description1);
    layout-> addWidget(description);
    layout-> addWidget(orientee);
    layout-> addWidget(bouton);
}

void RelationEditeur::create()
{
    //on créé un nouvel objet de type Relation
    RelationsManager &m=RelationsManager::getInstance();
    m.createRelation(this->titre->text(),this->description->toPlainText(),);
    QMessageBox::information(this,"Sauvegarde","Sauvegarde du nouvel article");
    getButton()->setDisabled(true);
}
*/

void RelationEditeur::save()
{
    relation->set_titre(titre->text());
    relation->set_description(description->toPlainText());
    QMessageBox::information(this,"Sauvegarde","Sauvegarde de la relation");
    bouton_save->setDisabled(true);
}

void RelationEditeur::delete_relation()
{
    RelationsManager &manager_relations=RelationsManager::getInstance();
    manager_relations.deleteRelation(*relation);
    QMessageBox::information(this,"Suppression","Suppression de la relation");
    //rendre tout disabled ?
}

void RelationEditeur::activerBouton(QString str)
{
   bouton_save->setEnabled(true);
}
