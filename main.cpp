#include <QApplication>
#include <QString>
#include "note.h"
#include "noteediteur.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString fichier = QFileDialog::getOpenFileName();
    NotesManager m=NotesManager();
    m.setFilename(fichier);
    m.load();
    Article& a=dynamic_cast<Article&>(m.getNote("id:A2"));
    ArticleEditeur fenetre(a);
    fenetre.show();
    return app.exec();

    /*
    QApplication app(argc, argv);
    QDate date1=QDate::currentDate();
    QDate date2=QDate::currentDate();
    Article a("A1","Titre 1", "Texte 1",date1, date2, 2, (2==2), archivee);
    ArticleEditeur fenetre(a);
    fenetre.show();
    return app.exec();
    */
}
