#include "note.h"
#include "noteediteur.h"


NoteEditeur::NoteEditeur(Note &n, QWidget* parent):QWidget(parent),note(&n)
{
    id1 = new QLabel("Identificateur",this);
    id = new QLineEdit(this);
    titre1 = new QLabel("Titre",this);
    titre = new QLineEdit(this);
    date1 = new QLabel("Date de creation",this);
    date_c = new QDateEdit(this);
    date2 = new QLabel("Date de derniere modification",this);
    date_m = new QDateEdit(this);
    if (n.getEtat()==active) etat = new QLabel("Etat : Active");
    else if (n.getEtat()==archivee) etat = new QLabel("Etat : Archivee");
            else etat = new QLabel("Etat : a la corbeille");
    layout = new QVBoxLayout(this);

    layout-> addWidget(id1);
    layout-> addWidget(id);
    layout-> addWidget(titre1);
    layout-> addWidget(titre);
    layout-> addWidget(date1);
    layout-> addWidget(date_c);
    layout-> addWidget(date2);
    layout-> addWidget(date_m);
    layout-> addWidget(etat);

    id->setReadOnly(true); //empeche la modification de l'id par l'utilisateur
    id->setText(n.getId());
    titre->setText(n.getTitre());
    date_c->setDate(n.getCreation());
    date_m->setDate(n.getModification());
    date_c->setReadOnly(true); //empeche la modification des dates par l'utilisateur -> modif se fera automatiquement
    date_m->setReadOnly(true);
    setLayout(layout);

}



ArticleEditeur::ArticleEditeur(Article& a, QWidget* parent):NoteEditeur(a,parent)
{
    text1 = new QLabel("Texte",this);
    text = new QTextEdit(this);
    getLayout()->addWidget(text1);
    getLayout()->addWidget(text);
    text->setText(a.getTexte());
    setLayout(getLayout());
}

