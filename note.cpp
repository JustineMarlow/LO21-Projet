#include "note.h"
#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>
#include <typeinfo>
#include <QDebug>
using namespace std;

Note::~Note(){}
Article::~Article(){}
Tache::~Tache(){}
TacheAvecPriorite::~TacheAvecPriorite(){}
TacheAvecDeadline::~TacheAvecDeadline(){}
Autre::~Autre(){}

void Relation::addCouple(Note& x, Note& y){
    if (nbCouples==nbCouplesMax) {
        //le tableau de couple nécessite un agrandissement
        Note*** newTableau= new Note**[nbCouplesMax+5];
        for(unsigned int i=0; i<nbCouplesMax+5; i++) newTableau[i] = new Note*[i];
        for(unsigned int i=0; i<nbCouples; i++) {
            newTableau[1][i]=tableau[1][i];
            newTableau[2][i]=tableau[2][i];
        }
        Note*** oldTableau=tableau;
        tableau=newTableau;
        nbCouplesMax+=5;
        if (oldTableau) delete[] oldTableau;
    }
    unsigned int rang=nbCouples++;
    tableau[1][rang]=&x;
    tableau[2][rang]=&y;
}

void Relation::removeCouple(Note* x, Note* y){
    unsigned int i=0;
    while (i<nbCouples && (tableau[1][i]!=x || tableau[2][i]!=y))
        i++;
   if(i==nbCouples)
       throw NotesException("Erreur : cette relation n'existe pas");
   else{ //on a bien trouvé le bon couple
        for(unsigned int j=i; j<nbCouples-1; j++){
            tableau[1][j]=tableau[1][j+1];
            tableau[2][j]=tableau[2][j+1];
         }
        nbCouples--;
   }
}

Relation::~Relation(){
    for (unsigned int i=0; i<nbCouples; i++)
        delete[] tableau[i];
      delete[] tableau;
}

NotesManager::~NotesManager(){
    if (filename!="") save();
    for(unsigned int i=0; i<nbNotes; i++) delete notes[i];
    delete[] notes;
}

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
        //il faudrait en plus mettre en place une procédure pour vérifier que, si l'id se répète, c'est parce qu'il s'agit de versions différentes d'une même note
        //pourquoi pas vérifier l'égalité des dates de création et l'existence de toutes les versions

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

//permet de créer un article par l'intermédiaire du NotesManager, appelle addNote
void NotesManager::addArticle(const QString& id, const QString& ti, const QString& te, const QDate date_c, const QDate date_m, const unsigned int v, bool last, const NoteEtat etat)
{
    /*for(unsigned int i=0; i<nbNotes; i++){
        if (notes[i]->getId()==id) throw NotesException("Erreur : identificateur déjà existant");
    }
    */
    Article* a=new Article(id,ti,te,date_c,date_m,v,last,etat);
    addNote(a);
}

//permet de chercher une note via le NotesManager
Note& NotesManager::getNote(const QString& id){
    // si la note existe déjà, on en renvoie une référence
    for(unsigned int i=0; i<nbNotes; i++)
        if (notes[i]->getId()==id && notes[i]->IsLast()) return *notes[i];
}

//méthode pour la comparaison de QString et string
bool latinCompare(const QString& qstr, const std::string& str)
{
  if( qstr.length() != (int)str.size() )
    return false;
  const QChar* qstrData = qstr.data();
  for( int i = 0; i < qstr.length(); ++i ) {
    if( qstrData[i].toLatin1() != str[i] )
      return false;
  }
  return true;
}

//méthode load() et save() récupérées du TD

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
                            qDebug()<<"id="<<identificateur<<"\n";
                        }

                        //titre
                        if(xml.name() == "title") {
                            xml.readNext();
                            titre=xml.text().toString();
                            qDebug()<<"titre="<<titre<<"\n";
                        }

                        //text
                        if(xml.name() == "text") {
                            xml.readNext();
                            text=xml.text().toString();
                            qDebug()<<"text="<<text<<"\n";
                        }

                        //version
                        if(xml.name() == "version") {
                            xml.readNext();
                            QString version_lue=xml.text().toString();
                            bool conversion;
                            version = version_lue.toUInt(&conversion, 10);
                            if (conversion==false) throw NotesException("Erreur dans conversion QString to Int (version)");
                            qDebug()<<"version="<<version<<"\n";
                        }

                        //date de creation
                        if(xml.name() == "creation") {
                            xml.readNext();
                            QString date_creation_lue=xml.text().toString();
                            date_c = QDate::fromString(date_creation_lue,"dd/MM/yyyy");
                            qDebug()<<"date_creation="<<date_c.QDate::toString(QString("dd/MM/yyyy"))<<"\n";
                        }

                        //date de modification
                        if(xml.name() == "modification") {
                            xml.readNext();
                            QString date_modification_lue=xml.text().toString();
                            date_m = QDate::fromString(date_modification_lue,"dd/MM/yyyy");
                            qDebug()<<"date_modification="<<date_m.QDate::toString(QString("dd/MM/yyyy"))<<"\n";
                        }

                        //etat
                        if(xml.name() == "etat") {
                            xml.readNext();
                            QString etat_lu=xml.text().toString();
                            qDebug()<<"etat_lu="<<etat_lu<<"\n";
                            if (latinCompare(etat_lu, "Active")) {etat=active; qDebug()<<"La note est active";}
                            else {if (latinCompare(etat_lu, "Archivee")) {etat=archivee; qDebug()<<"La note est archivee";}
                                  else {if (latinCompare(etat_lu, "Corbeille")) {etat=corbeille; qDebug()<<"La note est dans la corbeille";}
                                        else throw NotesException("Erreur dans lecture etat xml");}}
                        }

                        if(xml.name() == "last") {
                            xml.readNext();
                            QString last_lu=xml.text().toString();
                            qDebug()<<"last_lu="<<last_lu<<"\n";
                            if (latinCompare(last_lu, "oui")) {isLast=true; qDebug()<<"last true";}
                            else {if (latinCompare(last_lu, "non")) {isLast=false; qDebug()<<"last false";}}
                        }

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
        throw NotesException("Erreur lecteur fichier notes, parser xml");
    }
    // Removes any device() or data from the reader * and resets its internal state to the initial state.=
    xml.clear();
    qDebug()<<"fin load\n";
}

void NotesManager::save() const {

    QFile newfile(filename);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw NotesException(QString("Erreur dans la sauvegarde : echec lors de l'ouverture du fichier xml"));
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
