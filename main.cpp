#include <QApplication>
#include <QString>
#include <QDebug>
#include "note.h"
#include "noteediteur.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString fichier = QFileDialog::getOpenFileName();
    NotesManager &m=NotesManager::getInstance();
    m.setFilename(fichier);
    m.load();
    Article& a=dynamic_cast<Article&>(m.getNote("id:A2"));
    ArticleEditeur fenetre(a,m);
    for(NotesManager::Iterator iterator=NotesManager::getInstance().getIterator(); !iterator.isDone(); iterator.next())
    { if (iterator.current().getId()=="id:A2") qDebug()<<"version "<<iterator.current().getVersion()<<" trouvee"<<"\n"; }
    fenetre.show();
    return app.exec();

}
