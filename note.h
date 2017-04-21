#ifndef NOTE
#define NOTE
#include <QString>
#include <QDate>
//https://qt.developpez.com/doc/4.7/qdate/
#include <iostream>
using namespace std;

class NotesException{
public:
    NotesException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};

enum NoteEtat {active, archivee, corbeille};

class Note
{
private:
    QString identificateur;
    QString titre;
    QDate date_creation;
    QDate date_Lastmodification;
    bool isLastVersion;
    int version;
    //Note** VersionAnterieures;
    NoteEtat etat;

public:
    Note(QString id, QString t, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active)
        :identificateur(id),
         titre(t),
         date_creation(date_c),
         date_Lastmodification(date_m),
         isLastVersion(last),
         version(v),
         etat(e){}
    QString getId() const {return identificateur;}
    QString getTitre() const {return titre;}
    QDate getCreation() const {return date_creation;}
    QDate getModification() const {return date_Lastmodification;}
    NoteEtat getEtat() const {return etat;}
    bool IsLast() const {return isLastVersion;}
    int getVersion() const {return version;}
    virtual void fonction_bidon()=0; //pour créer le polymorphisme, en attendant d'avoir une vraie méthode virtuelle pure
    virtual ~Note();

};

    class Article:public Note
    {
        QString texte;
    public:
        Article(QString id, QString t, QString text, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active)
            :Note(id,t,date_c,date_m,last,v,e),
             texte(text)
             {}
        QString getTexte() const {return texte;}
        void fonction_bidon() {cout<<"Je suis un article.";}
        virtual ~Article();
    };

    enum TacheStatut {attente,cours,terminee};

    class Tache:public Note
    {
        QString texte;
        TacheStatut statut;
    public:
        Tache(QString id, QString t, QString text, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active, TacheStatut st=attente)
            :Note(id,t,date_c,date_m,last,v,e),
             texte(text),
             statut(st)
             {}
        QString getTexte() const {return texte;}
        TacheStatut getStatut() const {return statut;}
        void fonction_bidon() {cout<<"Je suis une tâche.";}
        virtual ~Tache();
    };

        class TacheAvecPriorite:public Tache
        {
            int priorite;
        public:
            TacheAvecPriorite(QString id, QString t, QString text, int p=0, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active, TacheStatut st=attente)
                :Tache(id,t,text,date_c,date_m,last,v,e,st),
                 priorite(p)
                 {}
            int getPriorite() const {return priorite;}
            void fonction_bidon() {cout<<"Je suis une tâche avec priorité.";}
            virtual ~TacheAvecPriorite();
        };

        class TacheAvecDeadline:public Tache
        {
            QDate deadline;
        public:
            TacheAvecDeadline(QString id, QString t, QString text, QDate dead, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active, TacheStatut st=attente)
                :Tache(id,t,text,date_c,date_m,last,v,e,st),
                 deadline(dead)
                 {}
            QDate getDeadline() const {return deadline;}
            void fonction_bidon() {cout<<"Je suis une tâche avec deadline.";}
            virtual ~TacheAvecDeadline();
        };

    enum AutreType {image, audio, video};

    class Autre:public Note
    {
        AutreType type;
        QString description;
        QString filename;
    public:
        Autre(QString id, QString t, AutreType ty, QString descr, QString name, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, int v=1, NoteEtat e=active)
            :Note(id,t,date_c,date_m,last,v,e),
             type(ty),
             description(descr),
             filename(name)
             {};
        QString getDescription() const {return description;}
        AutreType getType() const {return type;}
        QString getFilename() const {return filename;}
        void fonction_bidon() {cout<<"Je suis un autre.";}
        virtual ~Autre();
    };


class NotesManager
{
    Note** notes;
    unsigned int nbNotes;
    unsigned int nbMaxNotes;
    mutable QString filename;
public:
    //constructeur unique et sans argument
    NotesManager():notes(0),nbNotes(0),nbMaxNotes(0),filename("timp.dat"){}
    void addNote(Note* n);
    //Note& NewVersionNote(const QString& id);
    //Note& getNote(const QString& id); -> permet recherche d'une Note en particulier (utile ?) -> avec iterator ?

    //destructeur
    ~NotesManager();
    //constructeurs de recopie et opérateur d'affectation
    //NotesManager (const NotesManager& m);
    //NotesManager operator=(const NotesManager& m);
    void load();
    void save() const;
    void setFilename(const QString& f) { filename=f; }
    Note& getNote(const QString& id); // return the note with identificator id
    void addArticle(const QString& id, const QString& ti, const QString& te);
                    //const QDate date_c, const QDate date_m, bool last, int v);
};

#endif // NOTE

