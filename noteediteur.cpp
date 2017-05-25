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
    if (n.getEtat()==active) //on peut modifier la note ou la supprimer s'il s'agit de la dernière version, actualiser la version si ce n'est pas la dernière
        if (n.IsLast()) //derniere version
        {
            QObject::connect(getTitle(), SIGNAL(textChanged(QString)), this, SLOT(activerBouton_save(QString)));
            bouton_save = new QPushButton("Sauver",this);
            bouton_save->setDisabled(true);
            QObject::connect(bouton_save, SIGNAL(clicked()), this, SLOT(save()));
            bouton_delete = new QPushButton("Supprimer",this);
            QObject::connect(bouton_delete, SIGNAL(clicked()), this, SLOT(delete_note()));
        }
        else //ancienne version
        {
            titre->setDisabled(true);
            bouton_actualize = new QPushButton("Retablir comme version actuelle",this);
            bouton_actualize->setDisabled(false);
            QObject::connect(bouton_actualize, SIGNAL(clicked()), this, SLOT(setAsActual()));
        }
    else if (n.getEtat()==archivee) //on peut seulement la restaurer
            {
                titre->setDisabled(true);
                bouton_restore = new QPushButton("Restaurer cette note",this);
                bouton_restore->setDisabled(false);
                QObject::connect(bouton_restore, SIGNAL(clicked()), this, SLOT(restore()));
            }
    else throw NotesException("Cette note est dans la corbeille");

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

//NoteEditeur d'une nouvelle note //on ne peut que la sauvegarder
NoteEditeur::NoteEditeur(QWidget *parent){
    id1 = new QLabel("Identificateur",this);
    id = new QLineEdit(this);
    titre1 = new QLabel("Titre",this);
    titre = new QLineEdit(this);
    bouton_save = new QPushButton("Sauver",this);
    bouton_save->setDisabled(true);
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

void NoteEditeur::delete_note()
{
    NotesManager &manager_notes=NotesManager::getInstance();
    manager_notes.deleteNote(*note);
    QMessageBox::information(this,"Suppression","Suppression de la note");
    bouton_delete->setDisabled(true);
    if (note->getEtat()==active) etat->setText("Etat : Active");
    else if (note->getEtat()==archivee) etat->setText("Etat : Archivee");
            else etat->setText("Etat : a la corbeille");
    blockall();

}

void NoteEditeur::restore()
{
    NotesManager &manager_notes=NotesManager::getInstance();
    manager_notes.restoreNote(*note);
    QMessageBox::information(this,"Restauration","Restauration de la note");
    bouton_restore->setDisabled(true);
    if (note->getEtat()==active) etat->setText("Etat : Active");
    else if (note->getEtat()==archivee) etat->setText("Etat : Archivee");
            else etat->setText("Etat : a la corbeille");
    blockall();
}

//ArticleEditeur d'un article existant
ArticleEditeur::ArticleEditeur(Article& a, QWidget* parent):NoteEditeur(a,parent)
{
    text1 = new QLabel("Texte",this);
    text = new QTextEdit(this);
    getLayout()->addWidget(text1);
    getLayout()->addWidget(text);
    text->setText(a.getTexte());

    if (a.getEtat()==active)
        if (a.IsLast())
        {
            getLayout()->addWidget(getButton_save());
            getLayout()->addWidget(getButton_delete());
            QObject::connect(text, SIGNAL(textChanged()), this, SLOT(activerBouton_save()));
        }
        else
        {
            getLayout()->addWidget(getButton_actualize());
            text->setDisabled(true);
        }
    else if (a.getEtat()==archivee) {text->setDisabled(true);
                                     getLayout()->addWidget(getButton_restore());}

    setLayout(getLayout());
}

//ArticleEditeur d'un nouvel article
ArticleEditeur::ArticleEditeur(QWidget* parent):NoteEditeur(parent)
{
    text1 = new QLabel("Texte",this);
    text = new QTextEdit(this);
    getLayout()->addWidget(text1);
    getLayout()->addWidget(text);
    getLayout()->addWidget(getButton_save());


    QObject::connect(getButton_save(), SIGNAL(clicked()), this, SLOT(create()));
    QObject::connect(getId(), SIGNAL(textChanged(QString)), this, SLOT(activerBouton_save(QString)));

    setLayout(getLayout());
}

void ArticleEditeur::create()
{
    //on créé un nouvel objet de type Article
    NotesManager &m=NotesManager::getInstance();
    m.addArticle(this->getId()->text(), this->getTitle()->text(), this->text->toPlainText(), QDate::currentDate(), QDate::currentDate(), 1, true, active);
    QMessageBox::information(this,"Sauvegarde","Sauvegarde du nouvel article");
    getButton_save()->setDisabled(true);
    blockall();
}


void ArticleEditeur::extensionsave()
{
    Article& a=dynamic_cast<Article&>(getNote());
    //on créé un nouvel objet de type Article, l'id et la date de création ne changent pas, la version++, la date modification = la date current
    NotesManager &m=NotesManager::getInstance();
    m.addArticle(a.getId(), this->getTitle()->text(), text->toPlainText(), a.getCreation(), QDate::currentDate(), a.getVersion()+1, true, active);
    QMessageBox::information(this,"Sauvegarde","Sauvegarde de l'article");
    getButton_save()->setDisabled(true);
    setNote(&m.getNote(a.getId()));
    getDate_m()->setText("Date de derniere modification : "+m.getNote(a.getId()).getModification().QDate::toString(QString("dd/MM/yyyy")));
    getVersion()->setText("Version : "+QString::number(m.getNote(a.getId()).getVersion()));
}

void ArticleEditeur::extensionsetasactual()
{
    Article& a=dynamic_cast<Article&>(getNote());
    NotesManager &m=NotesManager::getInstance();
    unsigned int last_version=m.getNote(a.getId()).getVersion();
    m.getNote(a.getId()).setLast(false);
    m.addArticle(a.getId(),a.getTitre(),a.getTexte(),a.getCreation(),QDate::currentDate(),last_version+1,true,active);
    QMessageBox::information(this,"Mise a jour","Cette version a ete retablie comme version actuelle");
    getButton_actualize()->setDisabled(true);
    getDate_m()->setText("Date de derniere modification : "+m.getNote(a.getId()).getModification().QDate::toString(QString("dd/MM/yyyy")));
    getVersion()->setText("Version : "+QString::number(m.getNote(a.getId()).getVersion()));
    if (m.getNote(a.getId()).IsLast()) getLast()->setText("Derniere version : oui");
    else getLast()->setText("Derniere version : non");
    blockall();
}

void ArticleEditeur::blockall(){
    getId()->setDisabled(true);
    getTitle()->setDisabled(true);
    text->setDisabled(true);
}

void NoteEditeur::activerBouton_save(QString str)
{
    getButton_save()->setEnabled(true);

}

