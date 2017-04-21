#include "note.h"
#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>
#include <typeinfo>
using namespace std;

Note::~Note(){}
Article::~Article(){}
Tache::~Tache(){}
TacheAvecPriorite::~TacheAvecPriorite(){}
TacheAvecDeadline::~TacheAvecDeadline(){}
Autre::~Autre(){}

NotesManager::~NotesManager(){
    //if (filename!="") save();
    for(unsigned int i=0; i<nbNotes; i++) delete notes[i];
    delete[] notes;
}

//permet d'ajouter une note dans le tableau de Notes du NotesManager
void NotesManager::addNote(Note* n)
{
    for(unsigned int i=0; i<nbNotes; i++)
    {
        if (notes[i]->getId()==n->getId()) throw NotesException("Erreur, cet identificateur est deja utilise");
        //il faudrait en plus mettre en place une procédure pour vérifier que, si l'id se répète, c'est parce qu'il s'agit de versions différentes d'une même note
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

void NotesManager::addArticle(const QString& id, const QString& ti, const QString& te)
                              //, const QDate date_c, const QDate date_m, bool last, int v)
{
    for(unsigned int i=0; i<nbNotes; i++){
        if (notes[i]->getId()==id) throw NotesException("Erreur : identificateur déjà existant");
    }
    Article* a=new Article(id,ti,te);
                           //date_c,date_m,last,v);
    addNote(a);
}

Note& NotesManager::getNote(const QString& id){
    // si la note existe déjà, on en renvoie une référence
    for(unsigned int i=0; i<nbNotes; i++)
        //(typeid(notes[i]).name()=="article")
        if (notes[i]->getId()==id) return *notes[i];

    // sinon il est créé
    //Article* a=new Article(id,"","");
    //addNote(a);
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
            // If it's named tache, we'll dig the information from there.
            if(xml.name() == "article") {
                qDebug()<<"new article\n";
                QString identificateur;
                QString titre;
                QString text;
                QXmlStreamAttributes attributes = xml.attributes();
                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named article.
                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "article")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        // We've found identificteur.
                        if(xml.name() == "id") {
                            xml.readNext(); identificateur=xml.text().toString();
                            qDebug()<<"id="<<identificateur<<"\n";
                        }

                        // We've found titre.
                        if(xml.name() == "title") {
                            xml.readNext(); titre=xml.text().toString();
                            qDebug()<<"titre="<<titre<<"\n";
                        }
                        // We've found text
                        if(xml.name() == "text") {
                            xml.readNext();
                            text=xml.text().toString();
                            qDebug()<<"text="<<text<<"\n";
                        }
                    }
                    // ...and next...
                    xml.readNext();
                }
                qDebug()<<"ajout note "<<identificateur<<"\n";
                addArticle(identificateur,titre,text);
            }
        }
    }
    // Error handling.
    if(xml.hasError()) {
        throw NotesException("Erreur lecteur fichier notes, parser xml");
    }
    // Removes any device() or data from the reader * and resets its internal state to the initial state.
    xml.clear();
    qDebug()<<"fin load\n";
}

/*
void NotesManager::save() const {
    QFile newfile(filename);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw NotesException(QString("Erreur dans la sauvegarde : echec lors de l'ouverture du fichier xml"));
    QXmlStreamWriter stream(&newfile);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("Notes");
    for(unsigned int i=0; i<nbNotes; i++){
        //adapter au type de note (article, tache, tacheAvecPriorite, tacheAvecDeadline, Autre)
        stream.writeStartElement("article");
        stream.writeTextElement("id",notes[i]->getId());
        stream.writeTextElement("title",notes[i]->getTitre());
        //stream.writeTextElement("creation",notes[i]->getCreation());
        //stream.writeTextElement("derniere modif",notes[i]->getModification());
        //stream.writeTextElement("etat",notes[i]->getEtat());
        stream.writeEndElement();
    }
    stream.writeEndElement();
    stream.writeEndDocument();
    newfile.close();
}
*/
