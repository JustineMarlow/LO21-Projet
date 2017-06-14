#include <QApplication>
#include <QString>
#include <QDebug>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "note.h"
#include "relation.h"
#include "noteediteur.h"
#include "interface.h"
#include "relationediteur.h"

int main(int argc, char *argv[])
{
    try{
    QApplication app(argc, argv);
    //permet la francisation de la boîte de dialogue
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translator);

    RelationsManager &manager_relations=RelationsManager::getInstance();
    manager_relations.createRelation("Reference","La note x fait reference a la note y",true);

    QString filename_note=QFileDialog::getOpenFileName();
    NotesManager &manager_notes=NotesManager::getInstance();
    manager_notes.setFilename(filename_note);
    manager_notes.load();

    QString filename_relation=QFileDialog::getOpenFileName();
    manager_relations.setFilename(filename_relation);
    manager_relations.load();

    VuePrincipale fenetre;
    fenetre.show();
    return app.exec();
    }
    catch(NotesException e){qDebug()<<e.getInfo();}
    catch(InterfaceException e){qDebug()<<e.getInfo();}
}
