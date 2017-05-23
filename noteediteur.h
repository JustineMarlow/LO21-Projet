#ifndef NOTEEDITEUR_H
#define NOTEEDITEUR_H
#include <QApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDate>
#include <QDateEdit>
#include <QMessageBox>
#include <note.h>

class NoteEditeur:public QWidget
{
protected:
    Q_OBJECT
    Note* note;
    QVBoxLayout* layout;
    QLabel* id1;
    QLineEdit* id;
    QLabel* titre1;
    QLineEdit* titre;
    QLabel* date_c;
    QLabel* date_m;
    QLabel* last;
    QLabel* etat;
    QLabel* version;
    QPushButton* bouton;

public:
    explicit NoteEditeur(Note& n, QWidget* parent=0);
    explicit NoteEditeur(QWidget* parent=0);
    QVBoxLayout* getLayout() {return layout;} //méthode pour récupérer le layout et le modifier en fonction de la sous-classe
    QPushButton* getButton() {return bouton;} //méthode pour récupérer le bouton et le modifier en fonction de la sous-classe
    QLineEdit* getTitle() {return titre;} //méthode pour récupérer le titre et le modifier en fonction de la sous-classe
    QLineEdit* getId() {return id;} //méthode pour récupérer l'id et le modifier en fonction de la sous-classe
    virtual void extensionsave()=0;
    virtual void extensionsetasactual()=0;
    Note& getNote() const {return *note;}

signals:

public slots:
    void save();
    void setAsActual();
private slots: //à usage interne
    void activerBouton(QString str="");
};


class ArticleEditeur:public NoteEditeur
{
protected:
    Q_OBJECT
    QLabel* text1;
    QTextEdit* text;
public:
    ArticleEditeur(Article& a, QWidget* parent=0);
    ArticleEditeur(QWidget* parent=0);
public slots:
    void extensionsave();
    void extensionsetasactual();
    void create();

};


#endif // NOTEEDITEUR_H

