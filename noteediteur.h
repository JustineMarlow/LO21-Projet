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
#include <QSpinBox>
#include <QDateEdit>
#include <QFileDialog>
#include <QPixmap>
#include <note.h>

/**
 * \file      noteediteur.h
 * \brief     Définit la classe NoteEditeur et ses dérivées (pour les articles, tâches et fichiers).
 *
 * \details    Les NoteEditeurs sont des éléments d'interface qui permettent de visualiser et modifier les notes. Ils sont intégrées
 * aux éléments définis dans interface.h
 */

/*============================================================== NoteEditeur ==========================================================================================*/

/**
 * \class NoteEditeur
 * \brief     Classe abstraite, permet de visualiser et modifier les notes (interface).
 * \details   Classe qui est par la suite utilisée pour réaliser l'ensemble de l'interface graphiue (interface.h).
 */

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
    /**
     * @brief Constructeur
     * @param n Note à afficher
     */
    explicit NoteEditeur(Note& n, QWidget* parent=0);
    /**
     * @brief Surcharge du constructeur si on souhaite l'afficher "vide" (sans note)
     */

    explicit NoteEditeur(QWidget* parent=0);
    /**
     * @brief méthode pour récupérer le layout et le modifier en fonction de la sous-classe
     */
    QVBoxLayout* getLayout() {return layout;}

    //méthodes pour récupérer les boutons et les modifier en fonction de la sous-classe
    /**
     * @brief Récupère le bouton save pour le modifier en fonction de la sous-classe
     */
    QPushButton* getButton_save() {return bouton_save;}
    /**
     * @brief Récupère le bouton delete pour le modifier en fonction de la sous-classe
     */
    QPushButton* getButton_delete() {return bouton_delete;}
    /**
     * @brief Récupère le bouton actualiser pour le modifier en fonction de la sous-classe
     */
    QPushButton* getButton_actualize() {return bouton_actualize;}
    /**
     * @brief Récupère le bouton restaurer pour le modifier en fonction de la sous-classe
     */
    QPushButton* getButton_restore() {return bouton_restore;}
    /**
     * @brief Récupère le titre pour le modifier en fonction de la sous-classe
     */
    QLineEdit* getTitle() {return titre;}
    /**
     * @brief Récupère l'id pour le modifier en fonction de la sous-classe
     */
    QLineEdit* getId() {return id;}
    /**
     * @brief Récupère la date de dernière modification
     */
    QLabel* getDate_m(){return date_m;}
    /**
     * @brief Récupère le numéro de version
     */
    QLabel* getVersion(){return version;}
    /**
     * @brief Vérifie si il s'agit de la dernière version de la note.
     */
    QLabel* getLast(){return last;}
    /**
     * @brief Définit la sauvegarde selon la sous-classe de note (appelée par save())
     */
    virtual void extensionsave()=0;
    /**
     * @brief Définit l'actualisation selon la sous-classe de la note (appelée par setAsActual())
     */
    virtual void extensionsetasactual()=0;
    /**
     * @brief Récupère la note affichée
     */
    Note& getNote() const {return *note;}
    /**
     * @brief Permet de modifier la note à afficher
     */
    void setNote(Note* newnote){note=newnote;}
    /**
     * @brief Permet d'interdire toute action à l'utilisateur (désactive tous les boutons)
     */
    virtual void blockall()=0;

signals:

public slots:
    /**
     * @brief Enregistre la note & ses modifications
     */
    void save();
    /**
     * @brief Supprime la note
     */
    void delete_note();
    /**
     * @brief Actualise la note
     */
    void setAsActual();
    /**
     * @brief Restaure une note
     */
    void restore();
private slots:
    void activerBouton_save(QString str="");
};

/*============================================================ ArticleEditeur =========================================================================================*/
/**
 * \class ArticleEditeur
 * \brief    Permet de visualiser et modifier les articles (interface).
 * \details   Classe qui est par la suite utilisée pour réaliser l'ensemble de l'interface graphiue (interface.h).
 */

class ArticleEditeur:public NoteEditeur
{
protected:
    Q_OBJECT
    QLabel* text1;
    QTextEdit* text;
public:
    /**
     * @brief Constructeur
     * @param a Article à afficher
     */
    ArticleEditeur(Article& a, QWidget* parent=0);
    /**
     * @brief Surcharge de constructeur
     * @details Si l'on n'a pas d'article à afficher (éditeur par défaut, vide).
     * @param  a Article à afficher
     */
    ArticleEditeur(QWidget* parent=0);
    /**
     * @brief Permet d'interdire toute action à l'utilisateur (désactive tous les boutons)
     */
    void blockall();
public slots:
    /**
     * @brief Définit la sauvegarde selon la sous-classe de note (appelée par save())
     */
    void extensionsave();
    /**
     * @brief Définit l'actualisation selon la sous-classe de la note (appelée par setAsActual())
     */
    void extensionsetasactual();
    /**
     * @brief Création d'un nouvel objet article depuis l'articleEditeur.
     */
    void create();
};


/*============================================================= TacheEditeur ==========================================================================================*/

/**
 * \class TacheEditeur
 * \brief    Permet de visualiser et modifier les tâches sans deadline ni priorité (interface).
 * \details   Classe qui est par la suite utilisée pour réaliser l'ensemble de l'interface graphiue (interface.h).
 */
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
    /**
     * @brief Constructeur
     * @param t Tâche à afficher
     */
    TacheEditeur(Tache& t, QWidget* parent=0);
    /**
     * @brief Surcharge de constructeur si on n'a pas de tâche à afficher
     */
    TacheEditeur(QWidget* parent=0);
    /**
     * @brief Récupère le bouton radio "tâche en attente"
     */
    QRadioButton* getCheck_attente() {return check_attente;}
    /**
     * @brief Récupère le bouton radio "tâche en cours"
     */
    QRadioButton* getCheck_cours() {return check_cours;}
    /**
     * @brief Récupère le bouton radio "tâche terminée"
     */
    QRadioButton* getCheck_terminee() {return check_terminee;}
    /**
     * @brief Récupère le contenu de la tâche
     */
    QTextEdit* getText() {return text;}
    /**
     * @brief Permet d'interdire toute action à l'utilisateur (désactive tous les boutons)
     */
    void blockall();
public slots:
    /**
     * @brief Définit la sauvegarde selon la sous-classe de note (appelée par save())
     */
    void extensionsave();
    /**
     * @brief Définit l'actualisation selon la sous-classe de la note (appelée par setAsActual())
     */
    void extensionsetasactual();
    /**
     * @brief Création d'un nouvel objet tâche depuis le TacheEditeur.
     */
    void create();
};

/*====================================================== TacheAvecPrioriteEditeur =====================================================================================*/
/**
 * \class TacheAvecPrioriteEditeur
 * \brief    Permet de visualiser et modifier les tâches avec priorité (interface).
 * \details   Classe qui est par la suite utilisée pour réaliser l'ensemble de l'interface graphiue (interface.h).
 */

class TacheAvecPrioriteEditeur:public TacheEditeur
{
protected:
    Q_OBJECT
    QLabel* priorite1;
    QSpinBox* priorite;

public:
    /**
     * @brief Constructeur
     * @param t Tâche à afficher
     */
    TacheAvecPrioriteEditeur(TacheAvecPriorite& t, QWidget* parent=0);
    /**
     * @brief Surcharge de constructeur si on n'a pas de tâche à afficher
     */
    TacheAvecPrioriteEditeur(QWidget* parent=0);
    /**
     * @brief Permet d'interdire toute action à l'utilisateur (désactive tous les boutons)
     */
    void blockall();
public slots:
    /**
     * @brief Définit la sauvegarde selon la sous-classe de note (appelée par save())
     */
    void extensionsave();
    /**
     * @brief Définit l'actualisation selon la sous-classe de la note (appelée par setAsActual())
     */
    void extensionsetasactual();
    /**
     * @brief Création d'un nouvel objet TacheAvecPriorite depuis le tacheAvecPrioriteEditeur.
     */
    void create();
};

/*====================================================== TacheAvecDeadlineEditeur =====================================================================================*/

/**
 * \class TacheAvecDeadlineEditeur
 * \brief    Permet de visualiser et modifier les tâches avec deadline (interface).
 * \details   Classe qui est par la suite utilisée pour réaliser l'ensemble de l'interface graphiue (interface.h).
 */
class TacheAvecDeadlineEditeur:public TacheEditeur
{
protected:
    Q_OBJECT
    QLabel* deadline1;
    QDateEdit* deadline;

public:
    /**
     * @brief Constructeur
     * @param t Tâche à afficher
     */
    TacheAvecDeadlineEditeur(TacheAvecDeadline& t, QWidget* parent=0);
    /**
     * @brief Surcharge de constructeur si on n'a pas de tâche à afficher
     */
    TacheAvecDeadlineEditeur(QWidget* parent=0);
    /**
     * @brief Permet d'interdire toute action à l'utilisateur (désactive tous les boutons)
     */
    void blockall();
public slots:
    /**
     * @brief Définit la sauvegarde selon la sous-classe de note (appelée par save())
     */
    void extensionsave();
    /**
     * @brief Définit l'actualisation selon la sous-classe de la note (appelée par setAsActual())
     */
    void extensionsetasactual();
    /**
     * @brief Création d'un nouvel objet tacheAvecDeadline depuis l'tacheAvecDeadlineEditeur.
     */
    void create();
};

/*============================================================ FichierEditeur ==========================================================================================*/
/**
 * \class FichierEditeur
 * \brief    Permet de visualiser et modifier les images, vidéos et fichiers audios (interface).
 * \details   Classe qui est par la suite utilisée pour réaliser l'ensemble de l'interface graphiue (interface.h).
 */

class FichierEditeur:public NoteEditeur
{
protected:
    Q_OBJECT
    QRadioButton* type_image;
    QRadioButton* type_audio;
    QRadioButton* type_video;
    QLabel* description1;
    QTextEdit* description;
    QPushButton* select;
    QLabel* filename1;
    QString filename;
    QLabel* label_visu;
    QPixmap* visu_image;

public:
    /**
     * @brief Constructeur
     * @param f Fichier à afficer
     */
    FichierEditeur(Fichier& f, QWidget* parent=0);
    /**
     * @brief Surcharge de constructeur si on n'a pas de fichier à afficher
     */
    FichierEditeur(QWidget* parent=0);
    /**
     * @brief Permet d'interdire toute action à l'utilisateur (désactive tous les boutons)
     */
    void blockall();
public slots:
    /**
     * @brief Définit la sauvegarde selon la sous-classe de note (appelée par save())
     */
    void extensionsave();
    /**
     * @brief Définit l'actualisation selon la sous-classe de la note (appelée par setAsActual())
     */
    void extensionsetasactual();
    /**
     * @brief Création d'un nouvel objet fichier depuis le fichierEditeur.
     */
    void create();

    /**
     * @brief Permet d'ouvrir une fenêtre de dialogue d'ouverture de fihcier
     * @details Gère les différents types de fichiers (fenêtre de dialogue différente selon le type).
     */
    void select_file();
};


#endif // NOTEEDITEUR_H

