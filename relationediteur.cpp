#include "relation.h"
#include "relationediteur.h"
#include <QDebug>

RelationEditeur::~RelationEditeur(){
    if (relation!=0)
        for (unsigned int i=0; i<relation->getNbCouples(); i++)
        {
            delete box[i];
            delete edit_label[i];
            delete buttons_edit[i];
            delete buttons_remove[i];
        }
    delete [] box;
    delete [] edit_label;
    delete [] buttons_edit;
    delete [] buttons_remove;
}

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
    QMessageBox::information(this,"Sauvegarde","Sauvegarde de la relation");
    bouton_save->setDisabled(true);
}

void RelationEditeur::delete_relation()
{
    RelationsManager &manager_relations=RelationsManager::getInstance();
    manager_relations.deleteRelation(*relation);
    QMessageBox::information(this,"Suppression","Suppression de la relation");
    bouton_delete->setDisabled(true);
    titre->setDisabled(true);
    description->setDisabled(true);
    for (unsigned int i=0; i<relation->getNbCouples(); i++)
    {
        edit_label[i]->setDisabled(true);
        buttons_edit[i]->setDisabled(true);
        buttons_remove[i]->setDisabled(true);
    }

}

void RelationEditeur::activerBouton(QString str)
{
   bouton_save->setEnabled(true);
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
   Note& x=relation->getXCouple(i);
   Note& y=relation->getYCouple(i);
   relation->removeCouple(x,y);
   QMessageBox::information(this,"Suppression","Suppression du couple");
   for (unsigned int k=0; k<relation->getNbCouples()+1; k++)
   {
       edit_label[k]->setDisabled(true);
       buttons_edit[k]->setDisabled(true);
       buttons_remove[k]->setDisabled(true);
   }


   /*
   for (unsigned int k=0; k<relation->getNbCouples(); k++)
   {
       idX[i]= new QLabel(relation->getXCouple(i).getId(),this);
       idY[i]= new QLabel(relation->getYCouple(i).getId(),this);
       idX[k]->setText(relation->getXCouple(k).getId());
       idY[k]->setText(relation->getYCouple(k).getId());
       box[k]->addWidget(idX[k]);
       box[k]->addWidget(idY[k]);
       edit_label[k]->setText(relation->getLabelCouple(i));
       buttons_edit[k]= new QPushButton("Sauver le label", this);
       connect(buttons_edit[k], SIGNAL(clicked()), mapper_label, SLOT(map()));
       mapper_label->setMapping(buttons_edit[k],k);
       buttons_remove[k]= new QPushButton("Supprimer le couple", this);
       connect(buttons_remove[k], SIGNAL(clicked()), mapper_remove, SLOT(map()));
       mapper_remove->setMapping(buttons_remove[k],k);
       box[k]->addWidget(edit_label[k]);
       box[k]->addWidget(buttons_edit[k]);
       box[k]->addWidget(buttons_remove[k]);
       layout->addLayout(box[k]);
   }
   */
}
