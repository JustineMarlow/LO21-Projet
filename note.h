#ifndef NOTE
#define NOTE
#include <QString>
#include <QDate>

class NotesException{
public:
    NotesException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};

/*======================================================= Note et classes dérivées ====================================================================================*/
enum NoteEtat {active, archivee, corbeille};

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
    QString getId() const {return identificateur;}
    QString getTitre() const {return titre;}
    QDate getCreation() const {return date_creation;}
    QDate getModification() const {return date_Lastmodification;}
    NoteEtat getEtat() const {return etat;}
    bool IsLast() const {return isLastVersion;}
    unsigned int getVersion() const {return version;}
    void setLast(const bool b) {isLastVersion=b;}
    void setEtat(NoteEtat e){etat=e;}

};

    class Article:public Note
    {
    protected:
        QString texte;
        Article(QString id, QString t, QString text, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), unsigned int v=1, bool last=true, NoteEtat e=active)
            :Note(id,t,date_c,date_m,last,v,e),
             texte(text)
             {}
        ~Article();
        friend class NotesManager;
        friend class VuePrincipale;
    public:
        QString getTexte() const {return texte;}
    };

    enum TacheStatut {attente,cours,terminee};

    class Tache:public Note
    {
    protected:
        QString texte;
        TacheStatut statut;
        Tache(QString id, QString t, QString text, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active, TacheStatut st=attente)
            :Note(id,t,date_c,date_m,last,v,e),
             texte(text),
             statut(st)
             {}
        ~Tache();
        friend class NotesManager;
    public:
        QString getTexte() const {return texte;}
        TacheStatut getStatut() const {return statut;}
    };

        class TacheAvecPriorite:public Tache
        {
        protected:
            unsigned int priorite;
            TacheAvecPriorite(QString id, QString t, QString text, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active, TacheStatut st=attente,int p=0)
                :Tache(id,t,text,date_c,date_m,last,v,e,st),
                 priorite(p)
                 {}
            ~TacheAvecPriorite();
            friend class NotesManager;
        public:
            int getPriorite() const {return priorite;}
        };

        class TacheAvecDeadline:public Tache
        {
        protected:
            QDate deadline;
            TacheAvecDeadline(QString id, QString t, QString text, QDate dead, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active, TacheStatut st=attente)
                :Tache(id,t,text,date_c,date_m,last,v,e,st),
                 deadline(dead)
                 {}
            friend class NotesManager;
            ~TacheAvecDeadline();
        public:
            QDate getDeadline() const {return deadline;}
        };

    enum FichierType {image, audio, video};

    class Fichier:public Note
    {
    protected:
        FichierType type;
        QString description;
        QString filename;
        Fichier(QString id, QString t, FichierType ty, QString descr, QString name="", QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active)
            :Note(id,t,date_c,date_m,last,v,e),
             type(ty),
             description(descr),
             filename(name)
             {}
        ~Fichier();
        friend class NotesManager;
    public:
        QString getDescription() const {return description;}
        FichierType getType() const {return type;}
        QString getFilename() const {return filename;}
    };

/*============================================================= NotesManager ==========================================================================================*/
class NotesManager
{
    Note** notes;
    unsigned int nbNotes;
    unsigned int nbMaxNotes;
    mutable QString filename;
    bool isLoading;
    bool vidage_corbeille;
    NotesManager():notes(0),nbNotes(0),nbMaxNotes(0),filename(""),isLoading(false),vidage_corbeille(false){}
    ~NotesManager();
    NotesManager (const NotesManager& m);
    NotesManager operator=(const NotesManager& m);
public:
    static NotesManager& getInstance();
    void addNote(Note* n);
    void load();
    void save() const;
    void setFilename(const QString& f) { filename=f; }
    void setVidage(bool b) {vidage_corbeille=b;}
    unsigned int getNbNotes() const {return nbNotes;}
    Note& getNote(const QString& id);
    Note& getVersionNote(const QString& id, unsigned int v);
    void addArticle(const QString& id, const QString& ti, const QString& te,const QDate date_c, const QDate date_m, unsigned int v, bool last, NoteEtat etat);
    void addTache(const QString& id, const QString& t, const QString& text, const QDate date_c, const QDate date_m, unsigned int v, bool last, NoteEtat etat, TacheStatut st);
    void addTacheAvecPriorite(const QString& id, const QString& t, const QString& text, const QDate date_c, const QDate date_m, unsigned int v, bool last, NoteEtat etat, TacheStatut st, unsigned int priorite);
    void addTacheAvecDeadline(const QString& id, const QString& t, const QString& text, const QDate date_c, const QDate date_m, unsigned int v, bool last, NoteEtat etat, TacheStatut st, const QDate deadline);
    void addFichier(const QString& id, const QString& ti, const QString& descr, const QDate date_c, const QDate date_m, unsigned int v, bool last, NoteEtat etat, const QString& filename, FichierType ty);
    void deleteNote(Note& n);
    void restoreNote(Note& n);
    void viderCorbeille();
    void nettoyer_archives();
    void check_reference(Note& n);
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
    Iterator getIterator() {return Iterator(notes, nbNotes);}
};

//méthode utile à NotesManager::load() (dans notes.cpp) et à RelationsManager::load() (dans relations.cpp), donc inline
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

