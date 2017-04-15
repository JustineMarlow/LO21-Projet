#include "noteediteur.h"


NoteEditeur::NoteEditeur(Note &n, QWidget* parent):QWidget(parent),note(&n)
{
    id1 = new QLabel("Identificateur",this);
    titre1 = new QLabel("Titre",this);
    date1 = new QLabel("Date de création",this);
    date2 = new QLabel("Date de dernière modification",this);
    id = new QLineEdit(this);
    titre = new QLineEdit(this);
    date_c = new QDateEdit(this);
    date_m = new QDateEdit(this);
    layout = new QVBoxLayout(this);

    layout-> addWidget(id1);
    layout-> addWidget(id);
    layout-> addWidget(titre1);
    layout-> addWidget(titre);
    layout-> addWidget(date1);
    layout-> addWidget(date_c);
    layout-> addWidget(date2);
    layout-> addWidget(date_m);

    id->setReadOnly(true); //empeche la modification de l'id par l'utilisateur
    id->setText(n.getId());
    titre->setText(n.getTitre());
    date_c->setDate(n.getCreation());
    date_m->setDate(n.getModification());
    setLayout(layout);

}
