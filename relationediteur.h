#ifndef RELATIONEDITEUR
#define RELATIONEDITEUR

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
#include <QCheckBox>
#include <relation.h>

class RelationEditeur:public QWidget
{
protected:
    Q_OBJECT
    Relation* relation;
    QVBoxLayout* layout;
    QLabel* titre1;
    QLineEdit* titre;
    QLabel* description1;
    QTextEdit* description;
    QCheckBox* orientee;
    QPushButton* bouton_save;
    QPushButton* bouton_couple;
    QPushButton* bouton_delete;

public:
    explicit RelationEditeur(Relation& r, QWidget* parent=0);
    //explicit RelationEditeur(QWidget* parent=0);

signals:

public slots:
    void save();
    //void create();
    void delete_relation();
private slots: //à usage interne
    void activerBouton(QString str="");
};


#endif // RELATIONEDITEUR

