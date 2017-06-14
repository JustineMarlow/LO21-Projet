#include <relation.h>
#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>
#include <typeinfo>

/**
 * \file      relation.cpp
 * \brief     Définit les méthodes relatives aux classes Relation & RelationsManager.

 */

/*============================================================== Relation =============================================================================================*/
//destructeur
Relation::~Relation(){
    delete[] tableau[1];
    delete[] tableau[2];
    delete[] tableau;
    delete[] tableau_label;
}

bool Relation::findCouple(Note& x, Note& y)
{
    if (tableau==0) return false;
    for (unsigned int i=0; i<nbCouples; i++) if (tableau[1][i]==&x && tableau[2][i]==&y) return true;
    return false;
}

//méthode privée : permet d'ajouter un couple
void Relation::addCouple_function(Note& x, Note& y, QString l){
    unsigned int i=0;
    if (!(findCouple(x,y))) //le tableau est vide ou ne contient pas déjà le couple à ajouter
    {
        if (nbCouples==nbCouplesMax) {
            //le tableau de couples et le tableau de labels nécessitent un agrandissement
            Note*** newTableau= new Note**[2];
            QString* newLabel=new QString[nbCouplesMax+5];
            newTableau[1] = new Note*[nbCouplesMax+5];
            newTableau[2] = new Note*[nbCouplesMax+5];
            for(unsigned int i=0; i<nbCouples; i++) {
                newTableau[1][i]=tableau[1][i];
                newTableau[2][i]=tableau[2][i];
                newLabel[i]=tableau_label[i];
            }
            Note*** oldTableau=tableau;
            tableau=newTableau;
            QString* oldLabel=tableau_label;
            tableau_label=newLabel;
            nbCouplesMax+=5;
            if (oldTableau) delete[] oldTableau;
            if(oldLabel) delete[] oldLabel;
        }
        unsigned int rang=nbCouples++;
        tableau[1][rang]=&x;
        tableau[2][rang]=&y;
        tableau_label[rang]=l;
    }
}

//méthode publique qui appelle la méthode privée en fonction du caractère orientée de la relation
void Relation::addCouple(Note &x, Note &y, QString label){
    if (&x==&y) throw NotesException("Une note ne peut pas etre en relation avec elle meme.");
    if (oriente) addCouple_function(x,y,label);
    else { addCouple_function(x,y,label);
           addCouple_function(y,x,label);}
}

//permet de modifier le label d'un couple
void Relation::set_label_couple(Note& x, Note& y, QString l){
    unsigned int i=0;
    while (i<nbCouples && (tableau[1][i]->getId()!=x.getId() || tableau[2][i]->getId()!=y.getId()))
        i++;
    if (i==nbCouples) throw NotesException("Ce couple n'existe pas");
    else tableau_label[i]=l;
}

//méthode privée : permet de retirer un couple
void Relation::removeCouple_function(Note& x, Note& y){
    unsigned int i=0;
    while (i<nbCouples && (tableau[1][i]->getId()!=x.getId() || tableau[2][i]->getId()!=y.getId()))
        i++;
    if (i==nbCouples) throw NotesException("Ce couple n'existe pas");
    else {
        for (unsigned int j=i; j<nbCouples-1; j++){
            tableau[1][j]=tableau[1][j+1];
            tableau[2][j]=tableau[2][j+1];
            tableau_label[j]=tableau_label[j+1];
        }
        nbCouples--;
    }
}

//méthode publique qui appelle la méthode privée en fonction du caractère orientée de la relation
void Relation::removeCouple(Note &x, Note &y){
    if (oriente) removeCouple_function(x,y);
    else { removeCouple_function(x,y);
           removeCouple_function(y,x);}
}

/*========================================================= RelationsManager ==========================================================================================*/
//destructeur
RelationsManager::~RelationsManager(){
    if (filename!="") save();
    for(unsigned int i=0; i<nbRelations; i++) delete relations[i];
    delete[] relations;
}

//singleton
RelationsManager& RelationsManager::getInstance(){
    static RelationsManager instance;
    return instance;
}

//permet d'ajouter une relation au RelationsManager
void RelationsManager::addRelation(Relation& r)
{
    for(unsigned int i=0; i<nbRelations; i++)
        if (relations[i]->getTitre() ==r.getTitre()) throw NotesException("Erreur, ce titre est déjà utilisé");

    if (nbRelations==nbMaxRelations){
        //le tableau de relations nécessite un agrandissement
        Relation** newRelations= new Relation*[nbMaxRelations+5];
        for(unsigned int i=0; i<nbRelations; i++) newRelations[i]=relations[i];
        Relation** oldRelations=relations;
        relations=newRelations;
        nbMaxRelations+=5;
        if (oldRelations) delete[] oldRelations;
    }
    relations[nbRelations]=&r;
    nbRelations++;
}

//permet de créer une nouvelle relation et l'ajouter au RelationsManager
Relation& RelationsManager::createRelation(const QString& titre, const QString& description, bool isOriente)
{
    Relation* r=new Relation(titre,description,isOriente);
    addRelation(*r);
    return *r;
}

//permet de récupérer une relation particulière (via son titre)
Relation& RelationsManager::getRelation(const QString& titre)
{
    for(unsigned int i=0; i<nbRelations; i++)
    {
        if (relations[i]->getTitre()==titre) return *relations[i];
    }
    throw NotesException("Relation inexistante");
}

//permet de supprimer une relation
void RelationsManager::deleteRelation(Relation& r)
{
    if (r.getTitre()=="Reference") throw NotesException("Impossible de supprimer Reference");
    unsigned int i=0;
    while(relations[i]->getTitre()!=r.getTitre())
        i++;
    if(i>nbRelations) throw NotesException("Relation inexistante");
    else delete relations[i];
    for(i; i<nbRelations; i++)
        relations[i]=relations[i+1];
    nbRelations--;
}

//permet de charger un fichier de Relations
void RelationsManager::load() {
    QFile fin(filename);
    // If we can't open it, let's show an error message.
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw NotesException("Erreur ouverture fichier relations");
    }
    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(&fin);
    // We'll parse the XML until we reach end of it.
    while(!xml.atEnd() && !xml.hasError()) {
        // Read next element.
        QXmlStreamReader::TokenType token = xml.readNext();
        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {
            // If it's named taches, we'll go to the next.
            if(xml.name() == "relations") continue;
            if(xml.name() == "relation") {
                QString titre;
                QString description;
                bool isOriente;
                unsigned int nbCouples=0;
                unsigned int nbMaxCouples=10;
                QString* idx=new QString[nbMaxCouples];
                QString* idy=new QString[nbMaxCouples];
                QString* label=new QString[nbMaxCouples];
                QXmlStreamAttributes attributes = xml.attributes();
                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named relations.
                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "relation")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        //titre
                        if(xml.name() == "titre") {
                            xml.readNext();
                            titre=xml.text().toString();}
                        //description
                        if(xml.name() == "description") {
                            xml.readNext();
                            description=xml.text().toString();}
                        if(xml.name() == "oriente") {
                            xml.readNext();
                            QString oriente_lu=xml.text().toString();
                            if (latinCompare(oriente_lu, "oui")) isOriente=true;
                            else {if (latinCompare(oriente_lu, "non")) isOriente=false;}}
                        if(xml.name() == "couple") {
                            if(nbCouples==nbMaxCouples) //les tableaux nécessitent des agrandissements
                            {
                                nbMaxCouples+=10;
                                QString* Newidx=new QString[nbMaxCouples];
                                QString* Newidy=new QString[nbMaxCouples];
                                QString* Newlabel=new QString[nbMaxCouples];
                                for (unsigned int i=0;i<nbCouples;i++)
                                { Newidx[i]=idx[i]; Newidy[i]=idy[i]; Newlabel[i]=label[i];}
                                QString* oldidx=idx;
                                QString* oldidy=idy;
                                QString* oldlabel=label;
                                idx=Newidx;
                                idy=Newidy;
                                label=Newlabel;
                                if (oldidx) delete [] oldidx;
                                if (oldidy) delete [] oldidy;
                                if (oldlabel) delete [] oldlabel;
                            }
                            int rang=nbCouples++;
                            xml.readNext();
                            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "couple")) {
                                if(xml.tokenType() == QXmlStreamReader::StartElement) {
                                    //idx
                                    if(xml.name() == "idX") {
                                        xml.readNext();
                                        idx[rang]=xml.text().toString();}
                                    //idy
                                    if(xml.name() == "idY") {
                                        xml.readNext();
                                        idy[rang]=xml.text().toString();}
                                    //label
                                    if(xml.name() == "label") {
                                        xml.readNext();
                                        label[rang]=xml.text().toString();}
                                 }
                                xml.readNext();
                            }
                        }
                    }
                    // ...and next...
                    xml.readNext();
                }
                Relation& r=createRelation(titre,description,isOriente);
                for (unsigned int i=0; i<nbCouples; i++) r.addCouple(NotesManager::getInstance().getNote(idx[i]), NotesManager::getInstance().getNote(idy[i]), label[i]);
            }
        }
    }
    // Error handling.
    if(xml.hasError()) {
        throw NotesException("Erreur lecteur fichier relations, parser xml");}
    // Removes any device() or data from the reader * and resets its internal state to the initial state.=
    xml.clear();
}

//permet de sauvegarder les relations dans le fichier
void RelationsManager::save() const {
    QFile newfile(filename);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw NotesException(QString("Erreur dans la sauvegarde : echec lors de l'ouverture du fichier xml de relations"));
    QXmlStreamWriter stream(&newfile);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("relations");
    for(unsigned int i=0; i<nbRelations; i++)
    {
        if(relations[i]->getTitre()!="Reference")
        {
            stream.writeStartElement("relation");
            stream.writeTextElement("titre",relations[i]->getTitre());
            stream.writeTextElement("description",relations[i]->getDescription());
            if (relations[i]->IsOriente()==true) stream.writeTextElement("oriente","oui");
            else stream.writeTextElement("oriente","non");
            for (unsigned int c=0; c<relations[i]->getNbCouples();c++)
            {
                stream.writeStartElement("couple");
                stream.writeTextElement("idX",relations[i]->getXCouple(c).getId()); //souci ici
                stream.writeTextElement("idY",relations[i]->getYCouple(c).getId());
                stream.writeTextElement("label",relations[i]->getLabelCouple(c));
                stream.writeEndElement();
            }
            stream.writeEndElement();
        }
    }
    stream.writeEndElement();
    stream.writeEndDocument();
    newfile.close();
}
