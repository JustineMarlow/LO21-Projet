#include "note.h"
#include "relation.h"
#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>
#include <typeinfo>
#include <QDebug>

/*======================================================= Note et classes dérivées ====================================================================================*/
Note::~Note(){}
Article::~Article(){}
Tache::~Tache(){}
TacheAvecPriorite::~TacheAvecPriorite(){}
TacheAvecDeadline::~TacheAvecDeadline(){}
Autre::~Autre(){}

/*============================================================= NotesManager ==========================================================================================*/
//destructeur
NotesManager::~NotesManager(){
    viderCorbeille();
    if (filename!="") save();
    for(unsigned int i=0; i<nbNotes; i++) delete notes[i];
    delete[] notes;
}

//singleton
NotesManager& NotesManager::getInstance(){
    static NotesManager instance;
    return instance;
}

//permet d'ajouter une note dans le tableau de Notes du NotesManager
void NotesManager::addNote(Note* n)
{
    for(unsigned int i=0; i<nbNotes; i++)
    {
        if (notes[i]->getId()==n->getId()) {
            //on vérifie qu'il s'agit bien d'une nouvelle version
            if (notes[i]->getVersion()>=n->getVersion()) //il ne s'agit pas d'une nouvelle version
                throw NotesException("Erreur, cet identificateur est deja utilise");
        }
    }

    if (nbNotes==nbMaxNotes){
        //le tableau de note nécessite un agrandissement
        Note** newNotes= new Note*[nbMaxNotes+5];
        for(unsigned int i=0; i<nbNotes; i++) newNotes[i]=notes[i];
        Note** oldNotes=notes;
        notes=newNotes;
        nbMaxNotes+=5;
        if (oldNotes) delete[] oldNotes;
    }
    notes[nbNotes++]=n;
}

//permet de renvoyer la dernière version d'une note via le NotesManager
Note& NotesManager::getNote(const QString& id)
{
    // si la note existe déjà, on en renvoie une référence
    for(unsigned int i=0; i<nbNotes; i++)
        if (notes[i]->getId()==id && notes[i]->IsLast()) return *notes[i];
    throw NotesException("Note inexistante");
}

//permet de renvoyer une version particulière d'une note via le NotesManager
Note& NotesManager::getVersionNote(const QString& id, unsigned int v)
{
    // si la version de la note existe, on en renvoie une référence
    for(unsigned int i=0; i<nbNotes; i++)
        if (notes[i]->getId()==id && notes[i]->getVersion()==v) return *notes[i];
    throw NotesException("Note ou version inexistante");
}

//permet de créer un article et l'ajouter au NotesManager
void NotesManager::addArticle(const QString& id, const QString& ti, const QString& te, const QDate date_c, const QDate date_m, const unsigned int v, bool last, const NoteEtat etat)
{
    /*for(unsigned int i=0; i<nbNotes; i++){
        if (notes[i]->getId()==id) throw NotesException("Erreur : identificateur déjà existant");
    }
    */
    Article* a=new Article(id,ti,te,date_c,date_m,v,last,etat);
    addNote(a);
}

//permet de supprimer une relation
void NotesManager::deleteNote(Note& n)
{
    //on regarde d'abord si la note est en relation via Reference
    NotesManager &manager_notes=NotesManager::getInstance();
    RelationsManager &manager_relations=RelationsManager::getInstance();
    Relation& reference=manager_relations.getRelation("Reference");
    unsigned int i=0;
    while(i<reference.getNbCouples() && reference.getXCouple(i).getId()!=n.getId() && reference.getYCouple(i).getId()!=n.getId())
        i++;
    if(i<reference.getNbCouples()) //la note apparait dans la relation reference -> on ne peut pas la supprimer -> on l'archive
       { n.setEtat(archivee);
         for(unsigned int v=1; v<n.getVersion(); v++) manager_notes.getVersionNote(n.getId(),v).setEtat(archivee);}
    else { //la note n'apparait pas dans la relation reference -> on peut la supprimer -> mise à la corbeille et suppression des couples où n est impliquée
        n.setEtat(corbeille);
        for(unsigned int v=1; v<n.getVersion(); v++) manager_notes.getVersionNote(n.getId(),v).setEtat(corbeille);
        for(unsigned int i=0; i<manager_relations.getNbRelations(); i++)
            for(unsigned int j=0; j<manager_relations.getIRelation(i).getNbCouples();j++)
                if (manager_relations.getIRelation(i).getXCouple(j).getId()==n.getId() || manager_relations.getIRelation(i).getYCouple(j).getId()==n.getId())
                    manager_relations.getIRelation(i).removeCouple(manager_relations.getIRelation(i).getXCouple(j),manager_relations.getIRelation(i).getYCouple(j));
    }
}

void NotesManager::restoreNote(Note& n)
{
    NotesManager &manager_notes=NotesManager::getInstance();
    for(unsigned int v=1; v<=n.getVersion(); v++) manager_notes.getVersionNote(n.getId(),v).setEtat(active);
}

void NotesManager::viderCorbeille()
{
    for(unsigned int i=0;i<nbNotes;i++)
        if(notes[i]->getEtat()==corbeille)
        { delete notes[i];
          for(unsigned int j=i;j<nbNotes;j++)
              notes[j]=notes[j+1];
          nbNotes--;
        }
}

//permet de charger un fichier de Notes
void NotesManager::load() {
    QFile fin(filename);
    // If we can't open it, let's show an error message.
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw NotesException("Erreur ouverture fichier notes");
    }
    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(&fin);
    //qDebug()<<"debut fichier\n";
    // We'll parse the XML until we reach end of it.
    while(!xml.atEnd() && !xml.hasError()) {
        // Read next element.
        QXmlStreamReader::TokenType token = xml.readNext();
        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {
            // If it's named taches, we'll go to the next.
            if(xml.name() == "notes") continue;
            if(xml.name() == "article") {
                qDebug()<<"new article\n";
                QString identificateur;
                QString titre;
                QString text;
                unsigned int version;
                QDate date_c;
                QDate date_m;
                NoteEtat etat;
                bool isLast;
                QXmlStreamAttributes attributes = xml.attributes();
                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named article.
                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "article")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        //identificateur
                        if(xml.name() == "id") {
                            xml.readNext();
                            identificateur=xml.text().toString();
                            qDebug()<<"id="<<identificateur<<"\n";}
                        //titre
                        if(xml.name() == "title") {
                            xml.readNext();
                            titre=xml.text().toString();
                            qDebug()<<"titre="<<titre<<"\n";}
                        //text
                        if(xml.name() == "text") {
                            xml.readNext();
                            text=xml.text().toString();
                            qDebug()<<"text="<<text<<"\n";}
                        //version
                        if(xml.name() == "version") {
                            xml.readNext();
                            QString version_lue=xml.text().toString();
                            bool conversion;
                            version = version_lue.toUInt(&conversion, 10);
                            if (conversion==false) throw NotesException("Erreur dans conversion QString to Int (version)");
                            qDebug()<<"version="<<version<<"\n";}
                        //date de creation
                        if(xml.name() == "creation") {
                            xml.readNext();
                            QString date_creation_lue=xml.text().toString();
                            date_c = QDate::fromString(date_creation_lue,"dd/MM/yyyy");
                            qDebug()<<"date_creation="<<date_c.QDate::toString(QString("dd/MM/yyyy"))<<"\n";}
                        //date de modification
                        if(xml.name() == "modification") {
                            xml.readNext();
                            QString date_modification_lue=xml.text().toString();
                            date_m = QDate::fromString(date_modification_lue,"dd/MM/yyyy");
                            qDebug()<<"date_modification="<<date_m.QDate::toString(QString("dd/MM/yyyy"))<<"\n";}
                        //etat
                        if(xml.name() == "etat") {
                            xml.readNext();
                            QString etat_lu=xml.text().toString();
                            qDebug()<<"etat_lu="<<etat_lu<<"\n";
                            if (latinCompare(etat_lu, "Active")) {etat=active; qDebug()<<"La note est active";}
                            else {if (latinCompare(etat_lu, "Archivee")) {etat=archivee; qDebug()<<"La note est archivee";}
                                  else {if (latinCompare(etat_lu, "Corbeille")) {etat=corbeille; qDebug()<<"La note est dans la corbeille";}
                                        else throw NotesException("Erreur dans lecture etat xml");}}}
                        if(xml.name() == "last") {
                            xml.readNext();
                            QString last_lu=xml.text().toString();
                            qDebug()<<"last_lu="<<last_lu<<"\n";
                            if (latinCompare(last_lu, "oui")) {isLast=true; qDebug()<<"last true";}
                            else {if (latinCompare(last_lu, "non")) {isLast=false; qDebug()<<"last false";}}}
                    }
                    // ...and next...
                    xml.readNext();
                }
                qDebug()<<"ajout article "<<identificateur<<"\n";
                addArticle(identificateur,titre,text,date_c,date_m,version,isLast,etat);
            }
        }
    }
    // Error handling.
    if(xml.hasError()) {
        throw NotesException("Erreur lecteur fichier notes, parser xml");}
    // Removes any device() or data from the reader * and resets its internal state to the initial state.=
    xml.clear();
    qDebug()<<"fin load\n";
}

//permet de sauvegarder les notes dans le fichier
void NotesManager::save() const {

    QFile newfile(filename);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw NotesException(QString("Erreur dans la sauvegarde : echec lors de l'ouverture du fichier xml de notes"));
    QXmlStreamWriter stream(&newfile);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("notes");
    for(unsigned int i=0; i<nbNotes; i++){
        if (typeid(*notes[i])==typeid(Article))
        //adapter au type de note (article, tache, tacheAvecPriorite, tacheAvecDeadline, Autre)
        {   Article& a=dynamic_cast<Article&>(*notes[i]);
            stream.writeStartElement("article");
            stream.writeTextElement("id",a.getId());
            stream.writeTextElement("title",a.getTitre());
            stream.writeTextElement("text",a.getTexte());
            stream.writeTextElement("creation",a.getCreation().QDate::toString(QString("dd/MM/yyyy")));
            stream.writeTextElement("modification",a.getModification().QDate::toString(QString("dd/MM/yyyy")));
            if (a.IsLast()==true) stream.writeTextElement("last","oui");
            else stream.writeTextElement("last","non");
            if (a.getEtat()==active) stream.writeTextElement("etat","Active");
            else if (a.getEtat()==archivee) stream.writeTextElement("etat","Archivee");
            else stream.writeTextElement("etat","Corbeille");
            stream.writeTextElement("version",QString::number(a.getVersion()));
            stream.writeEndElement();
            qDebug()<<i<<" : note mise a jour \n";
        }
    }
    stream.writeEndElement();
    stream.writeEndDocument();
    newfile.close();
}
