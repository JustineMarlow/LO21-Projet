#ifndef NOTE
#define NOTE
#include <QString>
#include <QDate>
//https://qt.developpez.com/doc/4.7/qdate/
#include <iostream>
using namespace std;

class Note
{
private:
    QString identificateur;
    QString titre;
    QDate date_creation;
    QDate date_Lastmodification;
    bool isLastVersion;
    Note** VersionAnterieures;

public:
    Note(QString id, QString t, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, Note** tab=0)
        :identificateur(id),titre(t),date_creation(date_c),date_Lastmodification(date_m),isLastVersion(last),VersionAnterieures(tab){};
    QString getId() const {return identificateur;}
    QString getTitre() const {return titre;}
    QDate getCreation() const {return date_creation;}
    QDate getModification() const {return date_Lastmodification;}
    bool IsLast() const {return isLastVersion;}

};

class Article:public Note
{
    QString texte;
public:
    Article(QString id, QString t, QString text, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, Note** tab=0)
        :Note(id,t,date_c,date_m,last,tab),texte(text){};
    QString getTexte() const {return texte;}
};

enum TacheStatut {attente,cours,terminee};

class Tache:public Note
{
    QString texte;
    TacheStatut statut;
public:
    Tache(QString id, QString t, QString text, QDate date_c=QDate::currentDate(), QDate date_m=QDate::currentDate(), bool last=true, Note** tab=0, TacheStatut st=attente)
        :Note(id,t,date_c,date_m,last,tab),texte(text),statut(st){};
    QString getTexte() const {return texte;}
    TacheStatut getStatut() const {return statut;}
};

#endif // NOTE

