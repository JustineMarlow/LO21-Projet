#include "note.h"
#include "noteediteur.h"
#include <QDebug>
#include <typeinfo>

/*============================================================== NoteEditeur ==========================================================================================*/
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

void NoteEditeur::activerBouton_save(QString str)
{
    getButton_save()->setEnabled(true);

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

/*============================================================ ArticleEditeur =========================================================================================*/
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


/*============================================================= TacheEditeur ==========================================================================================*/
//TacheEditeur d'une tache existante
TacheEditeur::TacheEditeur(Tache& t, QWidget* parent):NoteEditeur(t,parent)
{
    text1 = new QLabel("Texte",this);
    text = new QTextEdit(this);
    getLayout()->addWidget(text1);
    getLayout()->addWidget(text);
    text->setText(t.getTexte());
    check_attente = new QRadioButton("En attente",this);
    check_cours = new QRadioButton("En cours",this);
    check_terminee = new QRadioButton("Terminee",this);
    if(t.getStatut()==attente) check_attente->setChecked(true);
    else if(t.getStatut()==cours) { check_cours->setChecked(true); check_attente->setDisabled(true); }
    else { check_terminee->setChecked(true); check_attente->setDisabled(true); check_cours->setDisabled(true);}
    getLayout()->addWidget(check_attente);
    getLayout()->addWidget(check_cours);
    getLayout()->addWidget(check_terminee);

    if (t.getEtat()==active)
        if (t.IsLast())
        {
            QObject::connect(text, SIGNAL(textChanged()), this, SLOT(activerBouton_save()));
            QObject::connect(check_attente, SIGNAL(clicked()), this, SLOT(activerBouton_save()));
            QObject::connect(check_cours, SIGNAL(clicked()), this, SLOT(activerBouton_save()));
            QObject::connect(check_terminee, SIGNAL(clicked()), this, SLOT(activerBouton_save()));
            if (typeid(t)==typeid(Tache))
            {   getLayout()->addWidget(getButton_save());
                getLayout()->addWidget(getButton_delete());
            }


        }
        else
        {
            text->setDisabled(true);
            check_attente->setDisabled(true);
            check_cours->setDisabled(true);
            check_terminee->setDisabled(true);
            if (typeid(t)==typeid(Tache))
                getLayout()->addWidget(getButton_actualize());
        }
    else if (t.getEtat()==archivee) {text->setDisabled(true);
                                     check_attente->setDisabled(true);
                                     check_cours->setDisabled(true);
                                     check_terminee->setDisabled(true);
                                     if (typeid(t)==typeid(Tache)) getLayout()->addWidget(getButton_restore());}

    if (typeid(t)==typeid(Tache)) setLayout(getLayout());
}

//TacheEditeur d'une nouvelle tâche
TacheEditeur::TacheEditeur(QWidget* parent):NoteEditeur(parent)
{
    text1 = new QLabel("Texte",this);
    text = new QTextEdit(this);
    check_attente = new QRadioButton("En attente",this);
    check_cours = new QRadioButton("En cours",this);
    check_terminee = new QRadioButton("Terminee",this);
    getLayout()->addWidget(text1);
    getLayout()->addWidget(text);
    getLayout()->addWidget(check_attente);
    getLayout()->addWidget(check_cours);
    getLayout()->addWidget(check_terminee);
    QObject::connect(getId(), SIGNAL(textChanged(QString)), this, SLOT(activerBouton_save(QString)));
    getLayout()->addWidget(getButton_save());
    QObject::connect(getButton_save(), SIGNAL(clicked()), this, SLOT(create()));
    setLayout(getLayout());
}

void TacheEditeur::create()
{
    qDebug()<<"methode create appelee";
    //on créé un nouvel objet de type Tâche
    NotesManager &m=NotesManager::getInstance();
    TacheStatut st;
    if (check_cours->isChecked()) st=cours;
    else if (check_terminee->isChecked()) st=terminee;
    else st=attente;
    m.addTache(this->getId()->text(),this->getTitle()->text(), this->text->toPlainText(), QDate::currentDate(), QDate::currentDate(), 1, true, active,st);
    QMessageBox::information(this,"Sauvegarde","Sauvegarde de la nouvelle tache");
    getButton_save()->setDisabled(true);
    blockall();
}


void TacheEditeur::extensionsave()
{
    Tache& t=dynamic_cast<Tache&>(getNote());
    //on créé un nouvel objet de type Tâche, l'id et la date de création ne changent pas, la version++, la date modification = la date current
    NotesManager &m=NotesManager::getInstance();
    TacheStatut st;
    if (check_cours->isChecked()) { st=cours; check_attente->setDisabled(true); }
    else if (check_terminee->isChecked()) { st=terminee; check_attente->setDisabled(true); check_cours->setDisabled(true); }
    else st=attente;
    m.addTache(t.getId(), this->getTitle()->text(), text->toPlainText(), t.getCreation(), QDate::currentDate(), t.getVersion()+1, true, active,st);
    QMessageBox::information(this,"Sauvegarde","Sauvegarde de la tache");
    getButton_save()->setDisabled(true);
    setNote(&m.getNote(t.getId()));
    getDate_m()->setText("Date de derniere modification : "+m.getNote(t.getId()).getModification().QDate::toString(QString("dd/MM/yyyy")));
    getVersion()->setText("Version : "+QString::number(m.getNote(t.getId()).getVersion()));

}

void TacheEditeur::extensionsetasactual()
{
    Tache& t=dynamic_cast<Tache&>(getNote());
    NotesManager &m=NotesManager::getInstance();
    unsigned int last_version=m.getNote(t.getId()).getVersion();
    m.getNote(t.getId()).setLast(false);
    m.addTache(t.getId(),t.getTitre(),t.getTexte(),t.getCreation(),QDate::currentDate(),last_version+1,true,active,t.getStatut());
    QMessageBox::information(this,"Mise a jour","Cette version a ete retablie comme version actuelle");
    getButton_actualize()->setDisabled(true);
    getDate_m()->setText("Date de derniere modification : "+m.getNote(t.getId()).getModification().QDate::toString(QString("dd/MM/yyyy")));
    getVersion()->setText("Version : "+QString::number(m.getNote(t.getId()).getVersion()));
    if (m.getNote(t.getId()).IsLast()) getLast()->setText("Derniere version : oui");
    else getLast()->setText("Derniere version : non");
    blockall();
}

void TacheEditeur::blockall(){
    getId()->setDisabled(true);
    getTitle()->setDisabled(true);
    text->setDisabled(true);
    check_attente->setDisabled(true);
    check_cours->setDisabled(true);
    check_terminee->setDisabled(true);
}

/*======================================================= TacheAvecPrioriteEditeur ====================================================================================*/
//TacheAvecPrioriteEditeur d'une tache avec priorite existante
TacheAvecPrioriteEditeur::TacheAvecPrioriteEditeur(TacheAvecPriorite& t, QWidget* parent):TacheEditeur(t,parent)
{
    priorite1 = new QLabel("Priorite",this);
    priorite = new QSpinBox(this);
    priorite->setMinimum(0);
    priorite->setMaximum(10);
    priorite->setValue(t.getPriorite());
    getLayout()->addWidget(priorite1);
    getLayout()->addWidget(priorite);

    if (t.getEtat()==active)
        if (t.IsLast())
        {
            QObject::connect(priorite, SIGNAL(valueChanged(int)), this, SLOT(activerBouton_save()));
            getButton_save()->disconnect();
            QObject::connect(getButton_save(), SIGNAL(clicked()), this, SLOT(save()));
            getLayout()->addWidget(getButton_save());
            getLayout()->addWidget(getButton_delete());

        }
        else
        {
            priorite->setDisabled(true);
            getLayout()->addWidget(getButton_actualize());
        }
    else if (t.getEtat()==archivee) {priorite->setDisabled(true);
                                     getLayout()->addWidget(getButton_restore());}

    setLayout(getLayout());
}

//TacheAvecPrioriteEditeur d'une nouvelle tâche avec priorite existante
TacheAvecPrioriteEditeur::TacheAvecPrioriteEditeur(QWidget* parent):TacheEditeur(parent)
{
    priorite1 = new QLabel("Priorite",this);
    priorite = new QSpinBox(this);
    priorite->setMinimum(0);
    priorite->setMaximum(10);
    getLayout()->addWidget(priorite1);
    getLayout()->addWidget(priorite);
    getLayout()->addWidget(getButton_save());
    getButton_save()->disconnect();
    QObject::connect(getButton_save(), SIGNAL(clicked()), this, SLOT(create()));
    QObject::connect(getId(), SIGNAL(textChanged(QString)), this, SLOT(activerBouton_save(QString)));
    setLayout(getLayout());
}

void TacheAvecPrioriteEditeur::create()
{
    qDebug()<<"methode create avec priorite appelee";
    //on créé un nouvel objet de type TacheAvecPriorite
    NotesManager &m=NotesManager::getInstance();
    TacheStatut st;
    if (getCheck_cours()->isChecked()) st=cours;
    else if (getCheck_terminee()->isChecked()) st=terminee;
    else st=attente;
    m.addTacheAvecPriorite(this->getId()->text(),this->getTitle()->text(), this->getText()->toPlainText(), QDate::currentDate(), QDate::currentDate(), 1, true, active,st,priorite->value());
    QMessageBox::information(this,"Sauvegarde","Sauvegarde de la nouvelle tache avec priorite");
    getButton_save()->setDisabled(true);
    blockall();
}


void TacheAvecPrioriteEditeur::extensionsave()
{
    TacheAvecPriorite& t=dynamic_cast<TacheAvecPriorite&>(getNote());
    //on créé un nouvel objet de type TacheAvecPriorite, l'id et la date de création ne changent pas, la version++, la date modification = la date current
    NotesManager &m=NotesManager::getInstance();
    TacheStatut st;
    if (getCheck_cours()->isChecked()) { st=cours; getCheck_attente()->setDisabled(true); }
    else if (getCheck_terminee()->isChecked()) { st=terminee; getCheck_attente()->setDisabled(true); getCheck_cours()->setDisabled(true); }
    else st=attente;
    m.addTacheAvecPriorite(t.getId(), this->getTitle()->text(), this->getText()->toPlainText(), t.getCreation(), QDate::currentDate(), t.getVersion()+1, true, active,st,priorite->value());
    QMessageBox::information(this,"Sauvegarde","Sauvegarde de la tache avec priorite");
    getButton_save()->setDisabled(true);
    setNote(&m.getNote(t.getId()));
    getDate_m()->setText("Date de derniere modification : "+m.getNote(t.getId()).getModification().QDate::toString(QString("dd/MM/yyyy")));
    getVersion()->setText("Version : "+QString::number(m.getNote(t.getId()).getVersion()));

}

void TacheAvecPrioriteEditeur::extensionsetasactual()
{
    TacheAvecPriorite& t=dynamic_cast<TacheAvecPriorite&>(getNote());
    NotesManager &m=NotesManager::getInstance();
    unsigned int last_version=m.getNote(t.getId()).getVersion();
    m.getNote(t.getId()).setLast(false);
    m.addTacheAvecPriorite(t.getId(),t.getTitre(),t.getTexte(),t.getCreation(),QDate::currentDate(),last_version+1,true,active,t.getStatut(),t.getPriorite());
    QMessageBox::information(this,"Mise a jour","Cette version a ete retablie comme version actuelle");
    getButton_actualize()->setDisabled(true);
    getDate_m()->setText("Date de derniere modification : "+m.getNote(t.getId()).getModification().QDate::toString(QString("dd/MM/yyyy")));
    getVersion()->setText("Version : "+QString::number(m.getNote(t.getId()).getVersion()));
    if (m.getNote(t.getId()).IsLast()) getLast()->setText("Derniere version : oui");
    else getLast()->setText("Derniere version : non");
    blockall();
}

void TacheAvecPrioriteEditeur::blockall(){
    TacheEditeur::blockall();
    priorite->setDisabled(true);
}

/*======================================================= TacheAvecDeadlineEditeur ====================================================================================*/
//TacheAvecDeadlineEditeur d'une tache avec deadline existante
TacheAvecDeadlineEditeur::TacheAvecDeadlineEditeur(TacheAvecDeadline& t, QWidget* parent):TacheEditeur(t,parent)
{
    deadline1 = new QLabel("Deadline",this);
    deadline = new QDateEdit(this);
    deadline->setDate(t.getDeadline());
    getLayout()->addWidget(deadline1);
    getLayout()->addWidget(deadline);

    if (t.getEtat()==active)
        if (t.IsLast())
        {
            QObject::connect(deadline, SIGNAL(dateChanged(QDate)), this, SLOT(activerBouton_save()));
            getButton_save()->disconnect();
            QObject::connect(getButton_save(), SIGNAL(clicked()), this, SLOT(save()));
            getLayout()->addWidget(getButton_save());
            getLayout()->addWidget(getButton_delete());

        }
        else
        {
            deadline->setDisabled(true);
            getLayout()->addWidget(getButton_actualize());
        }
    else if (t.getEtat()==archivee) {deadline->setDisabled(true);
                                     getLayout()->addWidget(getButton_restore());}

    setLayout(getLayout());
}

//TacheAvecDeadlineEditeur d'une nouvelle tâche avec deadline existante
TacheAvecDeadlineEditeur::TacheAvecDeadlineEditeur(QWidget* parent):TacheEditeur(parent)
{
    deadline1 = new QLabel("Deadline",this);
    deadline = new QDateEdit(this);
    getLayout()->addWidget(deadline1);
    getLayout()->addWidget(deadline);
    getLayout()->addWidget(getButton_save());
    getButton_save()->disconnect();
    QObject::connect(getButton_save(), SIGNAL(clicked()), this, SLOT(create()));
    QObject::connect(getId(), SIGNAL(textChanged(QString)), this, SLOT(activerBouton_save(QString)));
    setLayout(getLayout());
}

void TacheAvecDeadlineEditeur::create()
{
    //on créé un nouvel objet de type TacheAvecDeadline
    NotesManager &m=NotesManager::getInstance();
    TacheStatut st;
    if (getCheck_cours()->isChecked()) st=cours;
    else if (getCheck_terminee()->isChecked()) st=terminee;
    else st=attente;
    m.addTacheAvecDeadline(this->getId()->text(),this->getTitle()->text(), this->getText()->toPlainText(), QDate::currentDate(), QDate::currentDate(), 1, true, active,st,deadline->date());
    QMessageBox::information(this,"Sauvegarde","Sauvegarde de la nouvelle tache avec deadline");
    getButton_save()->setDisabled(true);
    blockall();
}


void TacheAvecDeadlineEditeur::extensionsave()
{
    TacheAvecDeadline& t=dynamic_cast<TacheAvecDeadline&>(getNote());
    //on créé un nouvel objet de type TacheAvecPriorite, l'id et la date de création ne changent pas, la version++, la date modification = la date current
    NotesManager &m=NotesManager::getInstance();
    TacheStatut st;
    if (getCheck_cours()->isChecked()) { st=cours; getCheck_attente()->setDisabled(true); }
    else if (getCheck_terminee()->isChecked()) { st=terminee; getCheck_attente()->setDisabled(true); getCheck_cours()->setDisabled(true); }
    else st=attente;
    m.addTacheAvecDeadline(t.getId(), this->getTitle()->text(), this->getText()->toPlainText(), t.getCreation(), QDate::currentDate(), t.getVersion()+1, true, active,st,deadline->date());
    QMessageBox::information(this,"Sauvegarde","Sauvegarde de la tache avec deadline");
    getButton_save()->setDisabled(true);
    setNote(&m.getNote(t.getId()));
    getDate_m()->setText("Date de derniere modification : "+m.getNote(t.getId()).getModification().QDate::toString(QString("dd/MM/yyyy")));
    getVersion()->setText("Version : "+QString::number(m.getNote(t.getId()).getVersion()));

}

void TacheAvecDeadlineEditeur::extensionsetasactual()
{
    TacheAvecDeadline& t=dynamic_cast<TacheAvecDeadline&>(getNote());
    NotesManager &m=NotesManager::getInstance();
    unsigned int last_version=m.getNote(t.getId()).getVersion();
    m.getNote(t.getId()).setLast(false);
    m.addTacheAvecDeadline(t.getId(),t.getTitre(),t.getTexte(),t.getCreation(),QDate::currentDate(),last_version+1,true,active,t.getStatut(),t.getDeadline());
    QMessageBox::information(this,"Mise a jour","Cette version a ete retablie comme version actuelle");
    getButton_actualize()->setDisabled(true);
    getDate_m()->setText("Date de derniere modification : "+m.getNote(t.getId()).getModification().QDate::toString(QString("dd/MM/yyyy")));
    getVersion()->setText("Version : "+QString::number(m.getNote(t.getId()).getVersion()));
    if (m.getNote(t.getId()).IsLast()) getLast()->setText("Derniere version : oui");
    else getLast()->setText("Derniere version : non");
    blockall();
}

void TacheAvecDeadlineEditeur::blockall(){
    TacheEditeur::blockall();
    deadline->setDisabled(true);
}

/*============================================================ FichierEditeur =========================================================================================*/
//FichierEditeur d'un autre existant
FichierEditeur::FichierEditeur(Fichier& f, QWidget* parent):NoteEditeur(f,parent)
{
    type_image = new QRadioButton("Image",this);
    type_audio = new QRadioButton("Enregistrement audio",this);
    type_video = new QRadioButton("Enregistrement video",this);
    if(f.getType()==image) type_image->setChecked(true);
    else if(f.getType()==audio) type_audio->setChecked(true);
    else type_video->setChecked(true);
    //on suppose qu'il n'est pas possible de changer le type d'un fichier
    type_image->setDisabled(true);
    type_video->setDisabled(true);
    type_audio->setDisabled(true);
    getLayout()->addWidget(type_image);
    getLayout()->addWidget(type_audio);
    getLayout()->addWidget(type_video);
    description1 = new QLabel("Description",this);
    description = new QTextEdit(this);
    getLayout()->addWidget(description1);
    getLayout()->addWidget(description);
    description->setText(f.getDescription());
    select = new QPushButton("Selectionner un fichier",this);
    getLayout()->addWidget(select);
    QObject::connect(select, SIGNAL(clicked()), this, SLOT(select_file()));
    filename=f.getFilename();
    filename1 = new QLabel("Nom du fichier : "+filename,this);
    getLayout()->addWidget(filename1);
    label_visu = new QLabel(this);
    visu_image = new QPixmap(filename);
    label_visu->setPixmap(*visu_image);
    getLayout()->addWidget(label_visu);


    if (f.getEtat()==active)
        if (f.IsLast())
        {
            getLayout()->addWidget(getButton_save());
            getLayout()->addWidget(getButton_delete());
            QObject::connect(description, SIGNAL(textChanged()), this, SLOT(activerBouton_save()));
            QObject::connect(select, SIGNAL(clicked()), this, SLOT(activerBouton_save()));
            QObject::connect(type_image, SIGNAL(clicked()), this, SLOT(activerBouton_save()));
            QObject::connect(type_audio, SIGNAL(clicked()), this, SLOT(activerBouton_save()));
            QObject::connect(type_video, SIGNAL(clicked()), this, SLOT(activerBouton_save()));
        }
        else
        {
            getLayout()->addWidget(getButton_actualize());
            blockall();

        }
    else if (f.getEtat()==archivee) {getLayout()->addWidget(getButton_restore());
                                    blockall();
    }
    setLayout(getLayout());
}

//FichierEditeur d'un nouvel fichier
FichierEditeur::FichierEditeur(QWidget* parent):NoteEditeur(parent)
{
    type_image = new QRadioButton("Image",this);
    type_audio = new QRadioButton("Enregistrement audio",this);
    type_video = new QRadioButton("Enregistrement video",this);
    getLayout()->addWidget(type_image);
    getLayout()->addWidget(type_audio);
    getLayout()->addWidget(type_video);
    description1 = new QLabel("Description",this);
    description = new QTextEdit(this);
    getLayout()->addWidget(description1);
    getLayout()->addWidget(description);
    select = new QPushButton("Selectionner un fichier",this);
    getLayout()->addWidget(select);
    QObject::connect(select, SIGNAL(clicked()), this, SLOT(select_file()));
    filename1 = new QLabel("Nom du fichier : ",this);
    getLayout()->addWidget(filename1);
    label_visu = new QLabel(this);
    getLayout()->addWidget(label_visu);
    getLayout()->addWidget(getButton_save());
    QObject::connect(getButton_save(), SIGNAL(clicked()), this, SLOT(create()));
    QObject::connect(getId(), SIGNAL(textChanged(QString)), this, SLOT(activerBouton_save(QString)));
    setLayout(getLayout());
}

void FichierEditeur::create()
{
    //on créé un nouvel objet de type Fichier
    NotesManager &m=NotesManager::getInstance();
    FichierType type;
    if (type_image->isChecked()) type=image;
    else if (type_audio->isChecked()) type=audio;
    else if (type_video->isChecked()) type=video;
    m.addFichier(this->getId()->text(), this->getTitle()->text(), description->toPlainText(), QDate::currentDate(), QDate::currentDate(), 1, true, active,filename,type);
    QMessageBox::information(this,"Sauvegarde","Sauvegarde du nouveau fichier");
    getButton_save()->setDisabled(true);
    blockall();
}


void FichierEditeur::extensionsave()
{
    Fichier& f=dynamic_cast<Fichier&>(getNote());
    //on créé un nouvel objet de type Fichier, l'id et la date de création ne changent pas, la version++, la date modification = la date current
    NotesManager &m=NotesManager::getInstance();
    FichierType type;
    if (type_image->isChecked()) type=image;
    else if (type_audio->isChecked()) type=audio;
    else if (type_video->isChecked()) type=video;
    m.addFichier(f.getId(), this->getTitle()->text(), description->toPlainText(), f.getCreation(), QDate::currentDate(), f.getVersion()+1, true, active,filename,type);
    QMessageBox::information(this,"Sauvegarde","Sauvegarde du fichier");
    getButton_save()->setDisabled(true);
    setNote(&m.getNote(f.getId()));
    getDate_m()->setText("Date de derniere modification : "+m.getNote(f.getId()).getModification().QDate::toString(QString("dd/MM/yyyy")));
    getVersion()->setText("Version : "+QString::number(m.getNote(f.getId()).getVersion()));
}

void FichierEditeur::extensionsetasactual()
{
    Fichier& f=dynamic_cast<Fichier&>(getNote());
    NotesManager &m=NotesManager::getInstance();
    unsigned int last_version=m.getNote(f.getId()).getVersion();
    m.getNote(f.getId()).setLast(false);
    m.addFichier(f.getId(),f.getTitre(),f.getDescription(),f.getCreation(),QDate::currentDate(),last_version+1,true,active,f.getFilename(),f.getType());
    QMessageBox::information(this,"Mise a jour","Cette version a ete retablie comme version actuelle");
    getButton_actualize()->setDisabled(true);
    getDate_m()->setText("Date de derniere modification : "+m.getNote(f.getId()).getModification().QDate::toString(QString("dd/MM/yyyy")));
    getVersion()->setText("Version : "+QString::number(m.getNote(f.getId()).getVersion()));
    if (m.getNote(f.getId()).IsLast()) getLast()->setText("Derniere version : oui");
    else getLast()->setText("Derniere version : non");
    blockall();
}

void FichierEditeur::blockall(){
    getId()->setDisabled(true);
    getTitle()->setDisabled(true);
    description->setDisabled(true);
    type_image->setDisabled(true);
    type_audio->setDisabled(true);
    type_video->setDisabled(true);
    description->setDisabled(true);
    select->setDisabled(true);
}

void FichierEditeur::select_file(){
    if (type_image->isChecked()) {
        filename = QFileDialog::getOpenFileName(this, "Selectionner un fichier image","/home",("Images (*.png *.jpg)"));
        visu_image = new QPixmap(filename);
        label_visu->setPixmap(*visu_image);
    }
    else if (type_video->isChecked()) filename = QFileDialog::getOpenFileName(this, "Selectionner un fichier video","/home",("Vidéo (*.avi *.mp4)"));
    else if (type_audio->isChecked()) filename = QFileDialog::getOpenFileName(this, "Selectionner un fichier audio","/home",("Audio (*.mp3)"));
    filename1->setText("Nom du fichier : "+filename);
    setLayout(getLayout());
}
