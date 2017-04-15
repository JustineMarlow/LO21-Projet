#include "note.h"
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

ArticleEditeur::ArticleEditeur(Article &a, QWidget* parent):QWidget(parent),article(&a)
{
    id1 = new QLabel("Identificateur",this);
    titre1 = new QLabel("Titre",this);
    date1 = new QLabel("Date de création",this);
    date2 = new QLabel("Date de dernière modification",this);
    id = new QLineEdit(this);
    titre = new QLineEdit(this);
    date_c = new QDateEdit(this);
    date_m = new QDateEdit(this);
    text1 = new QLabel(this);
    text = new QTextEdit(this);
    layout = new QVBoxLayout(this);

    layout-> addWidget(id1);
    layout-> addWidget(id);
    layout-> addWidget(titre1);
    layout-> addWidget(titre);
    layout-> addWidget(date1);
    layout-> addWidget(date_c);
    layout-> addWidget(date2);
    layout-> addWidget(date_m);
    layout-> addWidget(text1);
    layout-> addWidget(text);

    id->setReadOnly(true); //empeche la modification de l'id par l'utilisateur
    id->setText(a.getId());
    titre->setText(a.getTitre());
    date_c->setDate(a.getCreation());
    date_m->setDate(a.getModification());
    text->setText(a.getTexte());
    setLayout(layout);
}
