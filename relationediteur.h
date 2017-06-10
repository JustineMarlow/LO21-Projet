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
#include <QListView>
#include <QStringListModel>
#include <relation.h>

/**
 * \file      relationediteur.h
 * \brief     Définit la classe RelationEditeur.
 */

/**
 * \class RelationEditeur
 * \brief    Permet la visualisation, modification, suppression des relations (interface).
 * \details  Classe qui sera par la suite utilisée pour l'ensemble de l'interface (interface.h).
     */

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
    QHBoxLayout* new_couple;
    QLabel* couple2;
    QListView* liste_x;
    QListView* liste_y;
    QLineEdit* new_label;
    QPushButton* add_couple;
    QStringListModel* model_x;
    QStringListModel* model_y;


public:
    /**
     * @brief Constructeur (relation existante)
     * @param r Relation à ouvrir
     */
    explicit RelationEditeur(Relation& r, QWidget* parent=0);
    /**
     * @brief Surcharge du constructeur si on souhaite l'afficher "vide" (pour créer une relation)
     */
    explicit RelationEditeur(QWidget* parent=0);

signals:
    void monSignal(int);

public slots:
    /**
     * @brief Enregistre les modifications
     */
    void save();
    /**
     * @brief Crée une relation depuis le RelationEditeur
     */
    void create();
    /**
     * @brief Supprime une relation depuis le RelationEditeur
     */
    void delete_relation();
    /**
     * @brief Création d'un nouveau couple depuis le RelationEditeur
     */
    void add_new_couple();
    /**
     * @brief Modifie le label d'un couple de la relation
     * @param i indice du couple
     */
    void set_label(int i);
    /**
     * @brief Supprime un couple de la relation
     * @param i indice du couple
     */
    void remove_couple(int i);
private slots: //à usage interne
    /**
     * @brief Active le bouton "Save" quand du texte est modifié.
     */
    void activerBouton(QString str="");
};


#endif // RELATIONEDITEUR

