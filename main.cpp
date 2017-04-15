#include <QApplication>
#include <QString>
#include "note.h"
#include "noteediteur.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Note N=Note("id1","Test");
    NoteEditeur fenetre(N);
    fenetre.show();
    return app.exec();
}
