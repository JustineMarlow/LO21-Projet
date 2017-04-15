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
    QString getId(){return identificateur;}
    QString getTitre(){return titre;}
    QDate getCreation(){return date_creation;}
    QDate getModification(){return date_Lastmodification;}
    bool LastVersion(){return isLastVersion;}

};

#endif // NOTE

