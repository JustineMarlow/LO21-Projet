#ifndef RELATION_H
#define RELATION_H
#include <QString>
#include <QDate>
#include <note.h>

/**
 * \file      relation.h
 * \brief     Définit les classes Relation & RelationsManager
 */

/*============================================================== Relation =============================================================================================*/

/**
 * \class Relation
 * \brief    Représente les relations entre notes.
 */

class Relation {
    QString titre;
    QString description;
    Note*** tableau;
    QString* tableau_label;
    unsigned int nbCouples;
    unsigned int nbCouplesMax;
    bool oriente;
    /**
     * @brief Constructeur
     * @param t Titre
     * @param d Description
     * @param o True si la relation est orientée, false sinon.
     */
    Relation(QString t, QString d, bool o=true):titre(t),description(d),tableau(0),tableau_label(0),nbCouples(0),nbCouplesMax(0),oriente(o){}
    ~Relation();
    friend class RelationsManager;
    Relation(const Relation& r);
    Relation& operator=(const Relation& r);
    /**
     * @brief Ajoute un couple à une relation existante
     * @param x 1ere note du couple
     * @param y 2eme note du couple
     * @param label Nom de la relation à laquelle ajouter le couple
     */
    void addCouple_function(Note& x, Note& y, QString label);
    /**
     * @brief Retire un couple
     * @param x 1ere note du couple
     * @param y 2eme note du couple
     */
    void removeCouple_function(Note& x, Note& y);
    friend class VuePrincipale;
public:
    /**
     * @brief Indique si la relation est orientée
     */
    bool IsOriente() const {return oriente;}
    /**
     * @brief Retourne le titre de la relation
     */
    QString getTitre() const {return titre;}
    /**
     * @brief Retourne la description de la relation
     */
    QString getDescription() const {return description;}
    /**
     * @brief Retourne le nombre de couples de la relation
     */
    unsigned int getNbCouples() const {return nbCouples;}
    /**
     * @brief Retourne la 1ere note du couple d'une relation
     * @param i numéro de la relation
     */
    Note& getXCouple(unsigned int i) const {return *tableau[1][i];}
    /**
     * @brief Retourne la 2eme note du couple d'une relation
     * @param i numéro de la relation
     */
    Note& getYCouple(unsigned int i) const {return *tableau[2][i];}
    /**
     * @brief Retourne le label de la relation
     */
    const QString getLabelCouple(unsigned int i) const {return tableau_label[i];}

    /**
     * @brief Ajoute un couple
     * @param x 1ere note du couple
     * @param y 2eme note du couple
     * @param label Relation à laquelle ajouter le couple
     * @details méthode publique qui appelle la méthode privée addCouple_function() en fonction du caractère orientée de la relation
     */
    void addCouple(Note& x, Note& y, QString label);
    /**
     * @brief Enlève un couple
     * @param x 1ere note du couple
     * @param y 2eme note du couple
     * @details méthode publique qui appelle la méthode privée removeCouple_function() en fonction du caractère orientée de la relation
     */
    void removeCouple(Note& x, Note& y);
    /**
     * @brief Vérifie si un couple existe
     * @param x 1ere note du couple
     * @param y 2eme note du couple
     */
    bool findCouple(Note& x, Note& y);
    /**
     * @brief Modifie le titre de la relation
     * @param t Nouveau titre
     */
    void set_titre(QString t){titre=t;}
    /**
     * @brief Modifie la description d'une relation
     * @param d nouvelle description
     */
    void set_description(QString d){description=d;}
    //void set_Oriente(bool o){oriente=o;}

    /**
     * @brief Modifie le label d'un couple
     * @param x 1ere note du couple
     * @param y 2eme note du couple
     * @param l Nouveau label.
     */
    void set_label_couple(Note& x, Note& y, QString l);

    class Iterator{
        Note*** notes;
        unsigned int courant;
        unsigned int taille;
        friend class Relation;
        Iterator(Note*** tab, unsigned int n):notes(tab),courant(0),taille(n){}
    public:
        Note& current_noteX() const {return *notes[1][courant];}
        Note& current_noteY() const {return *notes[2][courant];}
        void next() {if (courant<taille) courant++; else throw NotesException("Iterator de Relation is done");}
        bool isDone() const {return courant==taille;}
        void debut() {courant=0;}
    };
    /**
     * @brief Permet d'obtenir un iterateur d'une relation (DP Iterator)
     */
    Iterator getIterator() {return Iterator(tableau, nbCouples);}
};

/*========================================================= RelationsManager ==========================================================================================*/
/**
 * \class RelationsManager
 * \brief    Permet la gestion de l'ensemble des relations.
 * \details  La classe stocke l'ensemble des relations, permet leur modification, suppression, ainsi que la création de nouvelles relations.
     * Toute modification/suppression/création de relation doit obligatoirement passer par le RelationsManager.
     */

class RelationsManager {
    Relation** relations;
    unsigned int nbRelations;
    unsigned int nbMaxRelations;
    mutable QString filename;
    /**
     * @brief Constructeur en privé (DP Singleton)
     */
    RelationsManager():relations(0),nbRelations(0),nbMaxRelations(0),filename(""){}
    ~RelationsManager();
    RelationsManager (const RelationsManager& m);
    RelationsManager operator=(const RelationsManager& m);
public:
    /**
     * @brief Retourne le nombre de relations du RelationsManager
     */
    unsigned int getNbRelations() const {return nbRelations;}
    /**
     * @brief Retourne l'unique objet RelationsManager (DP Singleton)
     * @details Crée un RelationsManager s'il n'en existe pas, renvoie son unique instance sinon.
     */
    static RelationsManager& getInstance();
    /**
     * @brief Retourne une relation selon son indice
     * @param i indice de la relation
     */
    Relation& getIRelation(unsigned int i) const {return *relations[i];}
    /**
     * @brief Création d'une nouvelle relation & ajout au RelationsManager
     * @param titre Titre
     * @param description Description
     * @param isOriente True si la relation est orientée, false sinon
     */
    Relation& createRelation(const QString& titre, const QString& description, bool isOriente);
    /**
     * @brief Ajout d'une relation existante au RelationsManager
     * @param r Relation à ajouter
     */
    void addRelation(Relation& r);
    /**
     * @brief Supprime une relation
     * @param r Relation à supprimer
     */
    void deleteRelation(Relation& r);

    /**
     * @brief Permet d'obtenir une relation via son titre
     * @param titre Titre de la relation recherchée
     * @return Une relation
     */
    Relation& getRelation(const QString& titre);
    /**
     * @brief Permet de charger un fichier de relations
     */
    void load();
    /**
     * @brief Permet d'enregistrer les relations dans un fichier
     */
    void save() const;
    /**
     * @brief Modifie le fichier à ouvrir
     * @param f Chemin du fichier à ouvrir
     */
    void setFilename(const QString& f) { filename=f; }

    class Iterator{
        Relation** tab;
        unsigned int courant;
        unsigned int taille;
        friend class RelationsManager;
        Iterator(Relation** t, unsigned int n):tab(t),courant(0),taille(n){}
    public:
        Relation& current() const {return *tab[courant];}
        void next() {if (courant<taille) courant++; else throw NotesException("Iterator de RelationsManager is done");}
        bool isDone() const {return courant==taille;}
        void debut() {courant=0;}
    };
    /**
     * @brief Permet d'obtenir un iterateur d'un RelationsManager (DP Iterator)
     */
    Iterator getIterator() {return Iterator(relations, nbRelations);}

};

#endif // RELATION_H
