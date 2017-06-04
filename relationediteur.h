#ifndef RELATIONEDITEUR
#define RELATIONEDITEUR
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
#include <QCheckBox>
#include <QSignalMapper>
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
    QPushButton* bouton_delete;
    QLabel* couple1;
    QHBoxLayout** box;
    QLabel** idX;
    QLabel** idY;
    QSignalMapper* mapper_label;
    QLineEdit** edit_label;
    QPushButton** buttons_edit;
    QSignalMapper* mapper_remove;
    QPushButton** buttons_remove;


public:
    explicit RelationEditeur(Relation& r, QWidget* parent=0);
    explicit RelationEditeur(QWidget* parent=0);
    ~RelationEditeur();

signals:
    void monSignal(int);

public slots:
    void save();
    void create();
    void delete_relation();
    void set_label(int i);
    void remove_couple(int i);
private slots: //à usage interne
    void activerBouton(QString str="");
};


#endif // RELATIONEDITEUR

