#ifndef INTERFACE_H
#define INTERFACE_H

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
#include<QtGui>
#include<QGroupBox>
#include<QDockWidget>
#include<QMainWindow>
#include<QMenuBar>
#include<QMenu>
#include<QShortcut>
#include<QKeySequence>
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include<QFormLayout>
#include <note.h>
#include <noteediteur.h>
#include<relation.h>
#include<relationediteur.h>

/**
 * \file     interface.h
 * \brief    Définit les classes nécessaires à l'interface de l'application (fenêtres).
 */


/**
 * \class InterfaceException
 * \brief      Permet de gérer les exceptions liées à l'interface.
 */
class InterfaceException{
public:
    InterfaceException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};


/**
 * \class VuePrincipale
 * \brief      Représente la fenêtre principale de l'application
 * \details   Permet la visualisation/modification d'une note & de l'arborescence de ses relations, la visualisation des notes actives,
 * des tâches et des notes archivées. Permet aussi la création d'une nouvelle note.
 */
class VuePrincipale : public QMainWindow{
    Q_OBJECT
public:
    /**
     * \brief      Permet d'accéder au seul objet de la classe VuePrincipale (DP Singleton).
     * \details    Crée une instance s'il n'en existe pas, retourne l'instance déjà existante sinon.
     * \param    n       La note que l'on souhaite afficher (par défaut, on n'affiche aucune note).
     * \return    Une référence sur la seule instance de la fenêtre.
     */
    static VuePrincipale& getInstance(Note* n=0);

    /**
     * \brief      Permet la création & l'affichage de la partie centrale de la fenêtre.
     * \details Utilisée dans le constructeur.
     */
    void affichage_central();

    /**
     * \brief      Permet la création & l'affichage de la partie droite de la fenêtre.
     * \details Utilisée dans le constructeur.
     */
    void affichage_droit();

    /**
     * \brief      Permet la création & l'affichage de la partie gauche de la fenêtre.
     * \details Utilisée dans le constructeur.
     */
    void affichage_gauche();

    /**
     * \brief      Permet la création & l'affichage du menu.
     * \details Utilisée dans le constructeur.
     */
    void creerMenu();

private slots:
    /**
     * \brief      Appelle le constructeur de la fenêtre secondaire (gestion & visualisation des relations)
     */
     void showRelations();

     /**
      * \brief      gère la partie droite (masque ou affiche l'arborescence)
      */
     void afficageArbo();

     /**
      * \brief     Permet l'affichage d'une nouvelle note
      * \details Slot appelé lorsque l'utilisateur sélectionne une note dans l'arborescence ou dans la colonne de gauche
      */
     void afficher_note(QTreeWidgetItem* item);

     //void interfaceSave(){noteEdit->save();}
     //void activerMenuSave(QString str="");

     /**
      * \brief      Permet l'affichage d'une autre version de la note
      * \details Slot appelé lorsque l'utilisateur sélectionne une autre version de la note visualisée (colonne de droite).
      */
     void afficher_version(QTreeWidgetItem* item);

     /**
      * \brief      Permet la création d'une nouvelle note
      * \param i Marqueur, indique le type de note à créer
      * (1 pour Article, 2 pour Tache, 3 pour TacheAvecPriorite, 4 pour TacheAvecDeadline, 5 pour Fichier)
      */
     void new_note(int i);

     /**
      * \brief      Permet d'actualiser l'affichage si une note/relation a été modifiée/créée/supprimée.
      */
     void actualiser_fenetre();

     /**
      * \brief      Permet de créer un nouvel article via la barre de menu.
      */
     void creerArticle();
     /**
      * \brief      Permet de créer une nouvelle tâche sans deadline/priorité via la barre de menu.
      */
     void creerTache();

     /**
      * \brief      Permet de créer une nouvelle tâche avec deadline via la barre de menu.
      */
     void creerTacheDeadline();

     /**
      * \brief      Permet de créer un nouveau fichier via la barre de menu.
      */
     void creerFichier();

     /**
      * \brief      Permet de créer une nouvelle tâche avec priorité via la barre de menu.
      */
     void creerTachePriorite();

private:
    static VuePrincipale* instance;

    /**
     * \brief      Constructeur de la fenêtre.
     * \details En privé pour le DP singleton.
     * \param n Note à afficher (par défaut, on n'affiche aucune note).
     */
    VuePrincipale(Note* n=0);
    unsigned int marqueur; //sert à identifier une demande de nouvelle note (1 pour Article, 2 pour Tache, 3 pour TacheAvecPriorite, 4 pour TacheAvecDeadline, 5 pour Fichier)
    QWidget* zoneCentrale;
    QHBoxLayout* layoutPrincipal;
    QPushButton* actualiser;
    QPushButton* arborescence;

    //Menu
    QAction* actionSupp;
    QMenu* menuFichier;

    //Partie gauche
    QGroupBox* gauche;

    //Partie centrale
    QGroupBox* centre;
    Note* note;
    NoteEditeur* noteEdit;
    QAction* actionSave;
    bool saveActive;

    //Partie droite
    QGroupBox* droite;
    bool arboVisible;
    QPushButton* relation_details;
protected:

    /**
     * \brief      Permet de gérer le vidage automatique de la corbeille.
     * \details Si l'option de vidage automatique est activée dans le menu, on ferme la fenetre et on vide la corbeille. Sinon,
     * on affiche une boîte de dialogue demandant à l'utilisateur s'il désire supprimer le contenu de la corbeille au moment de la
     * fermeture de l'application.
     * \param event Si l'option de vidage automatique est activée ou non.
     */
     void closeEvent(QCloseEvent* event);

};

/**
 * \class VueSecondaire
 * \brief      Représente la fenêtre secondaire de l'application (gestion des relations).
 * \details   Permet la visualisation/modification de l'ensemble des relations. La partie droite affiche les relations existantes, la partie
 * principale permet la visualisation & modification d'une relation en particulier.
 */

class VueSecondaire : public QWidget{
    Q_OBJECT
public :
    /**
     * \brief      Permet d'accéder au seul objet de la classe VueSecondaire (DP Singleton).
     * \details    Crée une instance s'il n'en existe pas, retourne l'instance déjà existante sinon.
     * \return    Une référence sur la seule instance de la fenêtre.
     */
    static VueSecondaire& getInstance();

    /**
     * \brief      Permet la création & l'affichage de la partie centrale de la fenêtre.
     * \details Utilisée dans le constructeur.
     */
    void affichage_central();

    /**
     * \brief      Permet la création & l'affichage de la partie gauche de la fenêtre.
     * \details Utilisée dans le constructeur.
     */
    void affichage_gauche();
private slots:
    /**
     * \brief     Ouvre le relationEditeur d'une relation sélectionnée par l'utilisateur.
     * \param item Une relation présente dans l'arborescence.
     */
    void openRelation(QTreeWidgetItem* item);

    /**
     * \brief      Permet d'actualiser l'affichage si une relation a été modifiée/créée/supprimée.
     */
    void actualiser_fenetre();

private :
    VueSecondaire();
    static VueSecondaire* instance;
    QHBoxLayout* layoutPrincipal;
    QVBoxLayout* principal;

    //Partie centrale
    QGroupBox* centre;
    Relation* relation;
    RelationEditeur* relationEdit;

    //Partie gauche
    QGroupBox* gauche;
    QPushButton* quitter;
    QPushButton* actualiser;
};

/*
class ArbreRelations : public QTreeWidgetItem{
    QString value;
private :
    QString getValue() const {return value;}
    ArbreRelations(const QString& text, const QString& v="Undefined", int type=Type) :
        QTreeWidgetItem(QStringList(text), type), value(v){}
};
*/

#endif // INTERFACE_H
