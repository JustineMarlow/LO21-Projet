#include "note.h"
#include "noteediteur.h"
#include <QDebug>

//NoteEditeur d'une note existante
NoteEditeur::NoteEditeur(Note &n, QWidget* parent):QWidget(parent),note(&n)
{
    id1 = new QLabel("Identificateur",this);
    id = new QLineEdit(this);
    id->setText(n.getId());
    id->setDisabled(true);
    titre1 = new QLabel("Titre",this);
    titre = new QLineEdit(this);
    titre->setText(n.getTitre());
    date_c = new QLabel("Date de creation : "+n.getCreation().QDate::toString(QString("dd/MM/yyyy")),this);
    date_m = new QLabel("Date de derniere modification : "+n.getModification().QDate::toString(QString("dd/MM/yyyy")),this);
    if (n.getEtat()==active) etat = new QLabel("Etat : Active");
    else if (n.getEtat()==archivee) etat = new QLabel("Etat : Archivee");
            else etat = new QLabel("Etat : a la corbeille");
    version = new QLabel("Version : "+QString::number(n.getVersion()),this);
    if (n.IsLast()) last = new QLabel("Derniere version : oui");
    else last = new QLabel("Derniere version : non");
    if (n.IsLast())
    {
        bouton = new QPushButton("Sauver",this);
        bouton->setDisabled(true);
        QObject::connect(bouton, SIGNAL(clicked()), this, SLOT(save()));
    }
    else
    {
        titre->setDisabled(true);
        bouton = new QPushButton("Rétablir comme version actuelle",this);
        bouton->setDisabled(false);
        QObject::connect(bouton, SIGNAL(clicked()), this, SLOT(setAsActual()));
    }
    layout = new QVBoxLayout(this);
    layout-> addWidget(id1);
    layout-> addWidget(id);
    layout-> addWidget(titre1);
    layout-> addWidget(titre);
    layout-> addWidget(date_c);
    layout-> addWidget(date_m);
    layout-> addWidget(etat);
    layout-> addWidget(last);
    layout-> addWidget(version);
}

//NoteEditeur d'une nouvelle note
NoteEditeur::NoteEditeur(QWidget *parent){
    id1 = new QLabel("Identificateur",this);
    id = new QLineEdit(this);
    titre1 = new QLabel("Titre",this);
    titre = new QLineEdit(this);
    bouton = new QPushButton("Sauver",this);
    bouton->setDisabled(true);
    layout = new QVBoxLayout(this);

    layout-> addWidget(id1);
    layout-> addWidget(id);
    layout-> addWidget(titre1);
    layout-> addWidget(titre);
}

void NoteEditeur::save()
{
    getNote().setLast(false);
    extensionsave();
}

void NoteEditeur::setAsActual()
{
    extensionsetasactual();
}

//ArticleEditeur d'un article existant
ArticleEditeur::ArticleEditeur(Article& a, QWidget* parent):NoteEditeur(a,parent)
{
    text1 = new QLabel("Texte",this);
    text = new QTextEdit(this);
    getLayout()->addWidget(text1);
    getLayout()->addWidget(text);
    text->setText(a.getTexte());
    getLayout()->addWidget(getButton());

    if (a.IsLast())
    {
        QObject::connect(getTitle(), SIGNAL(textChanged(QString)), this, SLOT(activerBouton(QString)));
        QObject::connect(text, SIGNAL(textChanged()), this, SLOT(activerBouton()));
    }
    else
    {
        text->setDisabled(true);
    }
    setLayout(getLayout());
}

//ArticleEditeur d'un nouvel article
ArticleEditeur::ArticleEditeur(QWidget* parent):NoteEditeur(parent)
{
    text1 = new QLabel("Texte",this);
    text = new QTextEdit(this);
    getLayout()->addWidget(text1);
    getLayout()->addWidget(text);
    getLayout()->addWidget(getButton());


    QObject::connect(getButton(), SIGNAL(clicked()), this, SLOT(create()));
    QObject::connect(getId(), SIGNAL(textChanged(QString)), this, SLOT(activerBouton(QString)));

    setLayout(getLayout());
}

void ArticleEditeur::create()
{
    //on créé un nouvel objet de type Article
    NotesManager &m=NotesManager::getInstance();
    m.addArticle(this->getId()->text(), this->getTitle()->text(), this->text->toPlainText(), QDate::currentDate(), QDate::currentDate(), 1, true, active);
    QMessageBox::information(this,"Sauvegarde","Sauvegarde du nouvel article");
    getButton()->setDisabled(true);
}


void ArticleEditeur::extensionsave()
{
    Article& a=dynamic_cast<Article&>(getNote());
    //on créé un nouvel objet de type Article, l'id et la date de création ne changent pas, la version++, la date modification = la date current
    NotesManager &m=NotesManager::getInstance();
    m.addArticle(a.getId(), this->getTitle()->text(), text->toPlainText(), a.getCreation(), QDate::currentDate(), a.getVersion()+1, true, active);
    QMessageBox::information(this,"Sauvegarde","Sauvegarde de l'article");
    getButton()->setDisabled(true);
}

void ArticleEditeur::extensionsetasactual()
{
    Article& a=dynamic_cast<Article&>(getNote());
    //on appelle la méthode setAsActualArticle du NotesManager
    NotesManager &m=NotesManager::getInstance();
    m.addArticle(a.getId(),a.getTitre(),a.getTexte(),a.getCreation(),QDate::currentDate(),m.getNote(a.getId()).getVersion()+1,true,active);
    m.getNote(a.getId()).setLast(false);

    QMessageBox::information(this,"Mise a jour","Cette version a ete retablie comme version actuelle");
    getButton()->setDisabled(true);
}

void NoteEditeur::activerBouton(QString str)
{
    getButton()->setEnabled(true);

}

