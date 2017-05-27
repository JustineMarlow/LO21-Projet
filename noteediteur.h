#ifndef NOTEEDITEUR_H
#define NOTEEDITEUR_H
#include <QApplication>
#include <QFileDialog>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDate>
#include <QDateEdit>
#include <QMessageBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <note.h>

/*============================================================== NoteEditeur ==========================================================================================*/
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
    QPushButton* bouton_save;
    QPushButton* bouton_delete;
    QPushButton* bouton_actualize;
    QPushButton* bouton_restore;

public:
    explicit NoteEditeur(Note& n, QWidget* parent=0);
    explicit NoteEditeur(QWidget* parent=0);
    QVBoxLayout* getLayout() {return layout;} //méthode pour récupérer le layout et le modifier en fonction de la sous-classe
    //méthodes pour récupérer les boutons et les modifier en fonction de la sous-classe
    QPushButton* getButton_save() {return bouton_save;}
    QPushButton* getButton_delete() {return bouton_delete;}
    QPushButton* getButton_actualize() {return bouton_actualize;}
    QPushButton* getButton_restore() {return bouton_restore;}
    QLineEdit* getTitle() {return titre;} //méthode pour récupérer le titre et le modifier en fonction de la sous-classe
    QLineEdit* getId() {return id;} //méthode pour récupérer l'id et le modifier en fonction de la sous-classe
    QLabel* getDate_m(){return date_m;}
    QLabel* getVersion(){return version;}
    QLabel* getLast(){return last;}
    virtual void extensionsave()=0;
    virtual void extensionsetasactual()=0;
    Note& getNote() const {return *note;}
    void setNote(Note* newnote){note=newnote;}
    virtual void blockall()=0;

signals:

public slots:
    void save();
    void delete_note();
    void setAsActual();
    void restore();
private slots:
    void activerBouton_save(QString str="");
};

/*============================================================ ArticleEditeur =========================================================================================*/
class ArticleEditeur:public NoteEditeur
{
protected:
    Q_OBJECT
    QLabel* text1;
    QTextEdit* text;
public:
    ArticleEditeur(Article& a, QWidget* parent=0);
    ArticleEditeur(QWidget* parent=0);
    void blockall();
public slots:
    void extensionsave();
    void extensionsetasactual();
    void create();
};


/*============================================================= TacheEditeur ==========================================================================================*/
class TacheEditeur:public NoteEditeur
{
protected:
    Q_OBJECT
    QLabel* text1;
    QTextEdit* text;
    QRadioButton* check_attente;
    QRadioButton* check_cours;
    QRadioButton* check_terminee;

public:
    TacheEditeur(Tache& t, QWidget* parent=0);
    TacheEditeur(QWidget* parent=0);
    void blockall();
public slots:
    void extensionsave();
    void extensionsetasactual();
    void create();
};


#endif // NOTEEDITEUR_H

