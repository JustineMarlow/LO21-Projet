#ifndef RELATION_H
#define RELATION_H
#include <QString>
#include <QDate>
#include <note.h>

class Relation {
    QString titre;
    QString description;
    Note*** tableau; //tableau multidirectionnel de couples de Note
    QString* tableau_label;
    unsigned int nbCouples;
    unsigned int nbCouplesMax;
    bool oriente;
    Relation(QString t, QString d, bool o=true):titre(t),description(d),tableau(0),tableau_label(0),nbCouples(0),nbCouplesMax(0),oriente(o){}
    ~Relation();
    friend class RelationsManager;
    //on interdit la recopie et l'affectation : une relation est unique, on ne la duplique pas
    Relation(const Relation& r);
    Relation& operator=(const Relation& r);
    void addCouple_function(Note& x, Note& y, QString label);
    void removeCouple_function(Note& x, Note& y);
    friend class VuePrincipale;
public:
    bool IsOriente() const {return oriente;}
    QString getTitre() const {return titre;}
    QString getDescription() const {return description;}
    unsigned int getNbCouples() const {return nbCouples;}
    const Note& getXCouple(unsigned int i) const {return *tableau[1][i];}
    const Note& getYCouple(unsigned int i) const {return *tableau[2][i];}
    const QString getLabelCouple(unsigned int i) const {return tableau_label[i];}
    void addCouple(Note& x, Note& y, QString label);
    void removeCouple(Note& x, Note& y);
    void set_titre(QString t){titre=t;}
    void set_description(QString d){description=d;}
    void set_Oriente(bool o){oriente=o;}
    void set_label_couple(Note& x, Note& y, QString l);
};

class RelationsManager {
    Relation** relations;
    unsigned int nbRelations;
    unsigned int nbMaxRelations;
    mutable QString filename;
    //implémentation Singleton -> passage des constructeurs, destructeurs, opérateur d'affectation en privé
    RelationsManager():relations(0),nbRelations(0),nbMaxRelations(0),filename(""){}
    ~RelationsManager();
    //recopie et affectation interdites
    RelationsManager (const RelationsManager& m);
    RelationsManager operator=(const RelationsManager& m);
public:
    unsigned int getNbRelations() const {return nbRelations;}
    static RelationsManager& getInstance(); //Singleton
    const Relation& getIRelation(unsigned int i) const {return *relations[i];}
    Relation& createRelation(const QString& titre, const QString& description, bool isOriente);
    void addRelation(Relation* r);
    Relation& getRelation(const QString& titre); //retourne une référence sur la relation
    void load();
    void save() const;
    void setFilename(const QString& f) { filename=f; }

    class Iterator{
        Relation** tab;
        unsigned int courant;
        unsigned int taille;
        friend class RelationsManager;
        Iterator(Relation** t, unsigned int n):tab(t),courant(0),taille(n){}
    public:
        Relation& current() const {return *tab[courant];}
        void next() {if (courant<taille) courant++; else throw NotesException("Iterator de Relation is done");}
        bool isDone() const {return courant==taille;}
        void debut() {courant=0;}
    };
    Iterator getIterator() {return Iterator(relations, nbRelations);}

};

#endif // RELATION_H
