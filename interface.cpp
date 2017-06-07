#include<interface.h>
#include<typeinfo>
#include<QGroupBox>
#include<QFormLayout>

//=============================FENÊTRE PRINCIPALE======================================


VuePrincipale::VuePrincipale(Note* n, RelationsManager* m) : note(n), manager(m){
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
                else if(typeid(*note)==typeid(TacheAvecPriorite)){
                    TacheAvecPriorite& t=dynamic_cast<TacheAvecPriorite&>(*note);
                    noteEdit=new TacheAvecPrioriteEditeur(t);
                }
                else if (typeid(*note)==typeid(TacheAvecDeadline)){
                    TacheAvecDeadline& t=dynamic_cast<TacheAvecDeadline&>(*note);
                    noteEdit=new TacheAvecDeadlineEditeur(t);
                }
                else if(typeid(*note)==typeid(Tache)){
                    Tache& t=dynamic_cast<Tache&>(*note);
                    noteEdit=new TacheEditeur(t);
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
           QVBoxLayout* rightLayout=new QVBoxLayout;
           liste_relations=new QTreeWidget;
           rightLayout->addWidget(liste_relations,4,0);
            RelationsManager::Iterator it=manager->getIterator();
           /* QStringList list = QStringList(it.current().getTitre()+"\n"+it.current().getDescription() );
            QTreeWidgetItem* item = new QTreeWidgetItem(list);
            liste_relations->addTopLevelItem(item);
            it.next();
            QStringList list2 = QStringList(it.current().getTitre()+"\n"+it.current().getDescription() );
            QTreeWidgetItem* item2 = new QTreeWidgetItem(list2);
            liste_relations->addTopLevelItem(item2);
            it.next();
            QStringList list3 = QStringList(it.current().getTitre()+"\n"+it.current().getDescription() );
            QTreeWidgetItem* item3 = new QTreeWidgetItem(list3);
            liste_relations->addTopLevelItem(item3);*/
            unsigned int i=1;
            QStringList* list=new QStringList[manager->getNbRelations()];
            QTreeWidgetItem* item=new QTreeWidgetItem[manager->getNbRelations()];
            while(!it.isDone()){
               qDebug()<<"entree dans la boucle";
               list[i] = QStringList(it.current().getTitre()+"\n"+it.current().getDescription());
               item[i] = QTreeWidgetItem(list[i]);
               liste_relations->addTopLevelItem(&item[i]);
               qDebug()<<"Ajout de la relation "<<it.current().getTitre()<<" à l'arborescence\n";
               i++;
               it.next();
            }
        qDebug()<<"Sortie de boucle maggle";
        arboVisible=true;
        relation_details=new QPushButton("Gérer les relations", this);
        rightLayout->addWidget(liste_relations);
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
    VueSecondaire* fenetreRelations=new VueSecondaire(manager);
    fenetreRelations->show();
}




//==========================================FENÊTRE SECONDAIRE (RELATIONS)========================================

VueSecondaire::VueSecondaire(RelationsManager* m) : manager(m){
    //colonne de gauche : liste de toutes les relations
     quitter=new QPushButton("Quitter", this);
    QGroupBox* gauche=new QGroupBox;
    QVBoxLayout* leftLayout=new QVBoxLayout;
    
    liste_relations = new QHBoxLayout*[manager->getNbRelations()];
    relation_titre = new QPushButton*[manager->getNbRelations()];
    relation_description=new QLabel*[manager->getNbRelations()];
    //editer = new QPushButton*[manager->getNbRelations()];
    //mapper_openRelation = new QSignalMapper(this);
   // connect(mapper_openRelation, SIGNAL(mapped(int)), this, SLOT(editRelation());
    unsigned int i=0;
    for(unsigned int i=0; i<manager->getNbRelations(); i++)
    {
        relation_titre[i]=new QPushButton(manager->getIRelation(i).getTitre(), this);
        relation_description[i]=new QLabel(manager->getIRelation(i).getDescription(), this);
        liste_relations[i]->addWidget(relation_titre[i]);
        liste_relations[i]->addWidget(relation_description[i]);
        leftLayout->addLayout(liste_relations[i]);
    }
    leftLayout->addWidget(quitter);
    gauche->setLayout(leftLayout);


    //partie principale
    QFormLayout* editeur=new QFormLayout;
    editeur->addRow("", &relations);
    QGroupBox* blocPrincipal=new QGroupBox("Gestion des relations");
    blocPrincipal->setLayout(editeur);


    QVBoxLayout *principal=new QVBoxLayout;
    principal->addWidget(blocPrincipal);
    QHBoxLayout* layout=new QHBoxLayout;
    layout->addWidget(gauche);
    layout->addWidget(blocPrincipal);
    setLayout(layout);
    setWindowTitle("Gestion des relations");
    resize(350, 450);
   connect(quitter, SIGNAL(clicked()), this, SLOT(close()));
}

