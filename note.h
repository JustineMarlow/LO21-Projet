#ifndef NOTE
#define NOTE
#include <QString>
#include <QDate>

/**
 * \file      note.h
 * \brief     Définit les classes Note & ses descendant (articles, tâches, tâches avec deadline/priorité, fichier) et le NotesManager
 *
 */

/**
 * \class NotesException
 * \brief      Gestion des exceptions déclenchées par les classes Notes & descendants & NotesManager.
 */
class NotesException{
public:
    /**
     * \brief      Constructeur
     * \param message Chaîne de caractère définissant l'exception
     */
    NotesException(const QString& message):info(message){}
    /**
     * \brief      Retourne le message d'information
     */
    QString getInfo() const { return info; }
private:
    QString info;
};

/*======================================================= Note et classes dérivées ====================================================================================*/
enum NoteEtat {active, archivee, corbeille};


/**
 * \class Note
 * \brief      Classe abstraite représentant une note.
 */
class Note
{
protected:
    QString identificateur;
    QString titre;
    QDate date_creation;
    QDate date_Lastmodification;
    bool isLastVersion;
    unsigned int version;
    NoteEtat etat;

    /**
     * \brief     Constructeur d'une note
     * \param id L'identificateur de la note
     * \param t Le titre de la note
     * \param date_c La date de création
     * \param date_m La date de dernière modification
     * \param last Indique si c'est la dernière version de la note ou pas
     * \param v Version de la note
     * \param e Etat de la note (active, archivée ou corbeille).
     */
    Note(QString id, QString t, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, unsigned int v=1, NoteEtat e=active)
        :identificateur(id),
         titre(t),
         date_creation(date_c),
         date_Lastmodification(date_m),
         isLastVersion(last),
         version(v),
         etat(e){}
    virtual ~Note();
    friend class NotesManager;

public:
    /**
     * @brief getId
     * @return Retourne l'id de la note
     */
    QString getId() const {return identificateur;}
    /**
     * @brief getTitre
     * @return Retourne le titre de la note
     */
    QString getTitre() const {return titre;}
    /**
     * @brief getCreation
     * @return Retourne la date de création de la note
     */
    QDate getCreation() const {return date_creation;}
    /**
     * @brief getModification
     * @return Retourne la date de dernière modification de la note.
     */
    QDate getModification() const {return date_Lastmodification;}
    /**
     * @brief getEtat
     * @return Retourne l'état de la note (active, archivée ou corbeille)
     */
    NoteEtat getEtat() const {return etat;}
    /**
     * @brief IsLast
     * @return True si c'est la dernière version, false sinon.
     */
    bool IsLast() const {return isLastVersion;}
    /**
     * @brief getVersion
     * @return Retourne le numéro de la version
     */
    unsigned int getVersion() const {return version;}
    /**
     * @brief permet de définir une version comme dernière ou pas.
     * @param b Vrai (si on veut définir la version comme dernière version) ou faux sinon.
     */
    void setLast(const bool b) {isLastVersion=b;}
    /**
     * @brief Permet de modifier l'état de la note
     * @param e Nouvel état de la note (active, archivée ou corbeille).
     */
    void setEtat(NoteEtat e){etat=e;}

};

/**
 * \class Article
 * \brief      Représente un article.
*/
    class Article:public Note
    {
    protected:
        QString texte;
        /**
         * \brief     Constructeur d'un article
         * \param id L'identificateur de la note
         * \param t Le titre de la note
         * \param date_c La date de création
         * \param date_m La date de dernière modification
         * \param last Indique si c'est la dernière version de la note ou pas
         * \param v Version de la note
         * \param e Etat de la note (active, archivée ou corbeille).
         * \param text Le contenu de l'article.
         */
        Article(QString id, QString t, QString text, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), unsigned int v=1, bool last=true, NoteEtat e=active)
            :Note(id,t,date_c,date_m,last,v,e),
             texte(text)
             {}
        ~Article();
        friend class NotesManager;
        friend class VuePrincipale;
    public:
        /**
         * @brief getTexte
         * @return Retourne le contenu de l'article
         */
        QString getTexte() const {return texte;}
    };

    enum TacheStatut {attente,cours,terminee};

    /**
     * \class Tache
     * \brief      Représente une tâche, sans deadline ni priorité.
     */
    class Tache:public Note
    {
    protected:
        QString texte;
        TacheStatut statut;

        /**
         * \brief     Constructeur d'une tâche sans deadline ni priorité
         * \param id L'identificateur de la note
         * \param t Le titre de la note
         * \param date_c La date de création
         * \param date_m La date de dernière modification
         * \param last Indique si c'est la dernière version de la note ou pas
         * \param v Version de la note
         * \param e Etat de la note (active, archivée ou corbeille).
         * \param text Contenu de la tâche
         * \param st Le statut (attente, cours, terminee)
         */
        Tache(QString id, QString t, QString text, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active, TacheStatut st=attente)
            :Note(id,t,date_c,date_m,last,v,e),
             texte(text),
             statut(st)
             {}
        ~Tache();
        friend class NotesManager;
    public:
        /**
         * @brief getTexte
         * @return Retourne le contenu de la tâche.
         */
        QString getTexte() const {return texte;}
        /**
         * @brief getStatut
         * @return Retourne le statut de la tâche.
         */
        TacheStatut getStatut() const {return statut;}
    };

    /**
     * \class TacheAvecPriorite
     * \brief      Représente une tâche avec priorité
     */

        class TacheAvecPriorite:public Tache
        {
        protected:
            unsigned int priorite;
            /**
             * \brief     Constructeur d'une tâche avec priorité
             * \param id L'identificateur de la note
             * \param t Le titre de la note
             * \param date_c La date de création
             * \param date_m La date de dernière modification
             * \param last Indique si c'est la dernière version de la note ou pas
             * \param v Version de la note
             * \param e Etat de la note (active, archivée ou corbeille).
             * \param text Contenu de la tâche
             * \param st Le statut (attente, cours, terminee)
             * \param p La priorité
             */
            TacheAvecPriorite(QString id, QString t, QString text, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active, TacheStatut st=attente,int p=0)
                :Tache(id,t,text,date_c,date_m,last,v,e,st),
                 priorite(p)
                 {}
            ~TacheAvecPriorite();
            friend class NotesManager;
        public:
            /**
             * @brief getPriorite
             * @return La priorité
             */
            int getPriorite() const {return priorite;}
        };

        /**
         * \class TacheAvecDeadline
         * \brief      Représente une tâche avec deadline.
         */
        class TacheAvecDeadline:public Tache
        {
        protected:
            QDate deadline;
            /**
             * \brief     Constructeur d'une tâche avec deadline
             * \param id L'identificateur de la note
             * \param t Le titre de la note
             * \param date_c La date de création
             * \param date_m La date de dernière modification
             * \param last Indique si c'est la dernière version de la note ou pas
             * \param v Version de la note
             * \param e Etat de la note (active, archivée ou corbeille).
             * \param text Contenu de la tâche
             * \param st Le statut (attente, cours, terminee)
             * \param dead La deadline
             */
            TacheAvecDeadline(QString id, QString t, QString text, QDate dead, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active, TacheStatut st=attente)
                :Tache(id,t,text,date_c,date_m,last,v,e,st),
                 deadline(dead)
                 {}
            friend class NotesManager;
            ~TacheAvecDeadline();
        public:
            /**
             * @brief getDeadline
             * @return La deadline de la tâche.
             */
            QDate getDeadline() const {return deadline;}
        };

    enum FichierType {image, audio, video};

    /**
     * \class Fichier
     * \brief      Représente un fichier (image, audio ou vidéo).
    */
    class Fichier:public Note
    {
    protected:
        FichierType type;
        QString description;
        QString filename;
        /**
         * \brief     Constructeur d'une note
         * \param id L'identificateur de la note
         * \param t Le titre de la note
         * \param date_c La date de création
         * \param date_m La date de dernière modification
         * \param last Indique si c'est la dernière version de la note ou pas
         * \param v Version de la note
         * \param e Etat de la note (active, archivée ou corbeille).
         * \param ty Type de la note (image, audio, video)
         * \param descr Description
         * \param name Nom du fichier.
         */
        Fichier(QString id, QString t, FichierType ty, QString descr, QString name="", QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active)
            :Note(id,t,date_c,date_m,last,v,e),
             type(ty),
             description(descr),
             filename(name)
             {}
        ~Fichier();
        friend class NotesManager;
    public:
        /**
         * @brief getDescription
         * @return La description
         */
        QString getDescription() const {return description;}
        /**
         * @brief getType
         * @return Le type (image, audio, video)
         */
        FichierType getType() const {return type;}
        /**
         * @brief getFilename
         * @return Le nom du fichier.
         */
        QString getFilename() const {return filename;}
    };

/*============================================================= NotesManager ==========================================================================================*/

    /**
     * \class NotesManager
     * \brief     Classe qui permet de gérer l'ensemble des notes.
     * \details   La classe stocke l'ensemble des notes, permet leur modification, suppression, ainsi que la création de nouvelles notes.
     * Toute modification/suppression/création de note doit obligatoirement passer par le NotesManager.
     */

    class NotesManager
{
    Note** notes;
    unsigned int nbNotes;
    unsigned int nbMaxNotes;
    mutable QString filename;
    bool isLoading;
    bool vidage_corbeille;
    /**
     * @brief Constructeur
     * @details En privé pour le DP singleton
     */
    NotesManager():notes(0),nbNotes(0),nbMaxNotes(0),filename(""),isLoading(false),vidage_corbeille(false){}
    ~NotesManager();
    /**
     * @brief Opérateur de recopie
     * @details En privé pour le DP singleton
     */
    NotesManager (const NotesManager& m);

    /**
     * @brief Opérateur d'affectation
     * @details En privé pour le DP singleton
     */
    NotesManager operator=(const NotesManager& m);
public:
    /**
     * @brief Retourne l'unique objet de la classe NotesManager
     * @details Crée un note manager s'il n'en existe pas encore, renvoie l'instance déjà existante sinon. (DP Singleton)
     */
    static NotesManager& getInstance();
    /**
     * @brief Ajoute une nouvelle note au NotesManager
     * @param n La note à ajouter
     */
    void addNote(Note* n);
    /**
     * @brief Permet de charger un fichier de Notes
     */
    void load();
    /**
     * @brief permet de sauvegarder les notes dans le fichier
     */
    void save() const;
    /**
     * @brief Permet de modifier chemin du fichier à ouvrir.
     * @param Chemin du fichier à ouvrir
     */
    void setFilename(const QString& f) { filename=f; }
    /**
     * @brief Permet de gérer le vidage automatique de la corbeille.
     * @param b true si l'on veut vider la corbeille automatiquement, false sinon
     */
    void setVidage(bool b) {vidage_corbeille=b;}
    /**
     * @brief getNbNotes
     * @return Le nombre de notes stockées par le notesManager.
     */
    unsigned int getNbNotes() const {return nbNotes;}
    /**
     * @brief Permet d'accéder à une note existante
     * @param id L'id de la note à ouvrir
     * @return Référence sur la dernière version de la note souhaitée
     */
    Note& getNote(const QString& id);

    /**
     * @brief Permet d'accéder à une note existante dans une certaine version
     * @param id L'id de la note à ouvrir
     * @param v La version souhaitée
     * @return Référence sur la version souhaitée de la note.
     */
    Note& getVersionNote(const QString& id, unsigned int v);

    /**
     * @brief Permet la création d'un article, qui sera ajouté au manager
     * @param id L'identificateur
     * @param ti Le titre
     * @param te Le contenu
     * @param date_c Date de création
     * @param date_m Date de dernière modif
     * @param v La version (forcément supérieure à toute version déjà existante du même article, et sans "trou" entre  2 version)
     * @param last Indique si c'est la dernière version ou non.
     * @param etat Etat de la note (active, archivee, corbeille).
     */
    void addArticle(const QString& id, const QString& ti, const QString& te,const QDate date_c, const QDate date_m, unsigned int v, bool last, NoteEtat etat);
    /**
     * @brief Permet la création d'une tâche sans deadline ni priorité, qui sera ajouté au manager
     * @param id Identificateur
     * @param t Titre
     * @param text Contenu
     * @param date_c Date de création
     * @param date_m Date de dernière modification
     * @param v La version (forcément supérieure à toute version déjà existante de la même tâche, et sans "trou" entre  2 version)
     * @param last Indique si c'est la dernière version ou non.
     * @param etat Etat de la note (active, archivee, corbeille).
     * @param st Statut (Attente, cours, terminee)
     */
    void addTache(const QString& id, const QString& t, const QString& text, const QDate date_c, const QDate date_m, unsigned int v, bool last, NoteEtat etat, TacheStatut st);

    /**
     * @brief Permet la création d'une tâche avec priorité, qui sera ajouté au manager
     * @param id Identificateur
     * @param t Titre
     * @param text Contenu
     * @param date_c Date de création
     * @param date_m Date de dernière modification
     * @param v La version (forcément supérieure à toute version déjà existante de la même tâche, et sans "trou" entre  2 version)
     * @param last Indique si c'est la dernière version ou non.
     * @param etat Etat de la note (active, archivee, corbeille).
     * @param st Statut (Attente, cours, terminee)
     * @param priorite Priorité
     */
    void addTacheAvecPriorite(const QString& id, const QString& t, const QString& text, const QDate date_c, const QDate date_m, unsigned int v, bool last, NoteEtat etat, TacheStatut st, unsigned int priorite);

    /**
     * @brief Permet la création d'une tâche avec priorité, qui sera ajouté au manager
     * @param id Identificateur
     * @param t Titre
     * @param text Contenu
     * @param date_c Date de création
     * @param date_m Date de dernière modification
     * @param v La version (forcément supérieure à toute version déjà existante de la même tâche, et sans "trou" entre  2 version)
     * @param last Indique si c'est la dernière version ou non.
     * @param etat Etat de la note (active, archivee, corbeille).
     * @param st Statut (Attente, cours, terminee)
     * @param deadLine (date).
     */
    void addTacheAvecDeadline(const QString& id, const QString& t, const QString& text, const QDate date_c, const QDate date_m, unsigned int v, bool last, NoteEtat etat, TacheStatut st, const QDate deadline);

    /**
     * @brief  Permet la création d'un fichier, qui sera ajouté au manager
     * @param id Identificateur
     * @param ti Titre
     * @param descr Description
     * @param date_c Date de création
     * @param date_m Date de dernière modification
     * @param v Version (forcément supérieure à toute version déjà existante du même fichier, et sans "trou" entre  2 version)
     * @param last Indique si c'est la dernière version ou non.
     * @param etat Etat de la note (active, archivee, corbeille).
     * @param filename Nom du fichier
     * @param ty Type (image, audio, video)
     */
    void addFichier(const QString& id, const QString& ti, const QString& descr, const QDate date_c, const QDate date_m, unsigned int v, bool last, NoteEtat etat, const QString& filename, FichierType ty);

    /**
     * @brief Supprime une note
     * @param n Note à supprimer
     */
    void deleteNote(Note& n);
    /**
     * @brief méthode pour rendre active une note qui avait été archivée
     * @param Note à restaurer
     */
    void restoreNote(Note& n);
    /**
     * @brief Vide la corbeille
     */
    void viderCorbeille();

    /**
     * @brief Vérifie pour chaque note archivée si une note active lui fait encore référence. Si non, la note
     * est placée à la corbeille.
     * @details appeler avant viderCorbeille
     */
    void nettoyer_archives();

    /**
     * @brief Vérifie si la note fait référence à d'autres note
     * @param La note à vérifier
     */
    void check_reference(Note& n);

    /**
     * @brief Méthode intermédiaire utilisée par check_reference()
     * @param n La note à vérifier
     * @param texte Selon le type de note (contenu ou description).
     */
    void search_reference(Note& n, const QString& texte);

    class Iterator{
        Note** tab;
        unsigned int courant;
        unsigned int taille;
        friend class NotesManager;
        Iterator(Note** t, unsigned int n):tab(t),courant(0),taille(n){}
    public:
        Note& current() const {return *tab[courant];}
        void next() {if (courant<taille) courant++; else throw NotesException("Iterator is done");}
        bool isDone() const {return courant==taille;}
        void debut() {courant=0;}
    };
    /**
     * @brief Crée un itérateur de la classe NotesManager (DP Iterator)
     * @return Un iterateur
     */
    Iterator getIterator() {return Iterator(notes, nbNotes);}
};


    /**
     * @brief méthode utile à NotesManager::load() (dans notes.cpp) et à RelationsManager::load() (dans relations.cpp)
     */
    inline bool latinCompare(const QString& qstr, const std::string& str)
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


#endif // NOTE

