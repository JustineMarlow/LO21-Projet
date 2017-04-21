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
    try {m.load();
    Article& a=dynamic_cast<Article&>(m.getNote("id:A1"));
    ArticleEditeur fenetre(a);
    fenetre.show();
    return app.exec();}
    catch(const char* erreur){cout<<erreur;};
}
