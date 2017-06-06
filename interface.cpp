#include<interface.h>
#include<typeinfo>
#include<QGroupBox>
#include<QFormLayout>

VuePrincipale::VuePrincipale(Note* n) : note(n){
    QWidget* zoneCentrale=new QWidget;
    setCentralWidget(zoneCentrale);

    //gauche
        QVBoxLayout* leftLayout=new QVBoxLayout;
        QGroupBox* notesActives=new QGroupBox("Notes Actives");
        QGroupBox* affichageErgo=new QGroupBox("Taches");
        QGroupBox* archives=new QGroupBox("Archives");
        arborescence=new QPushButton("Masquer l'arborescence", this);
        leftLayout->addWidget(notesActives);
        leftLayout->addWidget(affichageErgo);
        leftLayout->addWidget(archives);
        leftLayout->addWidget(arborescence);
        QGroupBox* gauche=new QGroupBox("", zoneCentrale);
        gauche->setLayout(leftLayout);

        connect(arborescence, SIGNAL(clicked()), this, SLOT(afficageArbo()));

       //centre

                if(typeid(*note)==typeid(Article)){
                    Article& a=dynamic_cast<Article&>(*note);
                    noteEdit=new ArticleEditeur(a);
                }
                else if(typeid(*note)==typeid(Tache)){
                    Tache& t=dynamic_cast<Tache&>(*note);
                    noteEdit=new TacheEditeur(t);
                }
                else if(typeid(*note)==typeid(TacheAvecPriorite)){
                    TacheAvecPriorite& t=dynamic_cast<TacheAvecPriorite&>(*note);
                    noteEdit=new TacheAvecPrioriteEditeur(t);
                }
                else if (typeid(*note)==typeid(TacheAvecDeadline)){
                    TacheAvecDeadline& t=dynamic_cast<TacheAvecDeadline&>(*note);
                    noteEdit=new TacheAvecDeadlineEditeur(t);
                }
                else if(typeid(*note)==typeid(Fichier)){
                    Fichier& f=dynamic_cast<Fichier&>(*note);
                    noteEdit=new FichierEditeur(f);
                 }
                else throw InterfaceException("Ce type de note n'existe pas");

            QFormLayout* editeur=new QFormLayout;
            editeur->addRow("", noteEdit);
            QGroupBox* centre=new QGroupBox("Visualisation de la note", zoneCentrale);
            centre->setLayout(editeur);


     //partie droite
        arboVisible=true;
        Relation reference("Reference","La note x fait reference a la note y",false);
        relation_titre= new QLabel(reference.getTitre());
        relation_description= new QLabel(reference.getDescription());
        relation_details=new QPushButton("Gérer les relations", this);
        QVBoxLayout* rightLayout=new QVBoxLayout;
        rightLayout->addWidget(relation_titre);
        rightLayout-> addWidget(relation_description);
        rightLayout-> addWidget(relation_details);
        droite=new QGroupBox("Arborescence des relations", zoneCentrale);
        droite->setLayout(rightLayout);

        connect(relation_details, SIGNAL(clicked()), this, SLOT(showRelations()));


    //menu
        QMenu *menuFichier = menuBar()->addMenu("&Fichier");
           QMenu *nouveau = menuFichier->addMenu("&Nouvelle note");
               QAction *actionArt=new QAction("Nouvel article", this);
               nouveau->addAction(actionArt);
               QAction* actionTache=new QAction("Nouvelle &tâche", this);
               nouveau->addAction(actionTache);
               QAction* actionImage=new QAction("Nouvelle Image", this);
               nouveau->addAction(actionImage);
            QAction *actionSave=new QAction("&Enregistrer", this);
            menuFichier->addAction(actionSave);
            actionSave->setShortcut(QKeySequence("Ctrl+S"));
            QAction *actionQuitter = new QAction("&Quitter", this);
            actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
            connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
            menuFichier->addAction(actionQuitter);


        QMenu *menuEdition = menuBar()->addMenu("&Edition");
           QAction* actionAnnuler=new QAction("Annuler", this);
           menuEdition->addAction(actionAnnuler);
           actionAnnuler->setShortcut(QKeySequence("Ctrl+Z"));
           QAction* actionRetablir=new QAction("Rétablir", this);
           actionRetablir->setShortcut(QKeySequence("Ctrl+Y"));
           menuEdition->addAction(actionRetablir);

        QMenu *menuCorbeille = menuBar()->addMenu("&Corbeille");
           QAction* actionSupp=new QAction("Supprimer automatiquement les notes archivées à la fermeture de l'application", this);
           menuCorbeille->addAction(actionSupp);
           actionSupp->setCheckable(true); //pour le slot : avec vérifier avec isChecked()
           QAction* actionArchives=new QAction("Voir les notes &archivées", this);
           menuCorbeille->addAction(actionArchives);

       //Définition du layout principal
           QHBoxLayout* layoutPrincipal = new QHBoxLayout;
            layoutPrincipal->addWidget(gauche);
            layoutPrincipal->addWidget(centre);
            layoutPrincipal->addWidget(droite);
            zoneCentrale->setLayout(layoutPrincipal);
            setWindowTitle("Pluri'notes");
}



void VuePrincipale::afficageArbo(){
    if(arboVisible){
        arborescence->setText("Voir l'arborescence");
        droite->hide();
        arboVisible=false;
    }
    else{
        arborescence->setText("Masquer l'arborescence");
        arboVisible=true;
        droite->show();
    }
}

void VuePrincipale::showRelations(){
    VueSecondaire* fenetreRelations=new VueSecondaire();
    fenetreRelations->show();
}

VueSecondaire::VueSecondaire() {
    //layout principal
    //relations=new RelationEditeur();
    QFormLayout* editeur=new QFormLayout;
    editeur->addRow("", &relations);
    QGroupBox* blocPrincipal=new QGroupBox("Gestion des relations");
    blocPrincipal->setLayout(editeur);

    quitter=new QPushButton("Quitter", this);
    QVBoxLayout *layout=new QVBoxLayout;
    layout->addWidget(blocPrincipal);
    layout->addWidget(quitter);
    setLayout(layout);
    setWindowTitle("Gestion des relations");
    resize(350, 450);
   connect(quitter, SIGNAL(clicked()), this, SLOT(close()));
}

