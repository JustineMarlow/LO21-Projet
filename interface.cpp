#include<interface.h>
#include<typeinfo>
#include<QGroupBox>
#include<QFormLayout>

//=============================FENÊTRE PRINCIPALE======================================


VuePrincipale::VuePrincipale(Note* n) : note(n){
    zoneCentrale=new QWidget;
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
        gauche=new QGroupBox("", zoneCentrale);
        gauche->setLayout(leftLayout);

        connect(arborescence, SIGNAL(clicked()), this, SLOT(afficageArbo()));

       //centre
        affichage_central();


     //partie droite
         affichage_droit();


    //menu
        QMenu *menuFichier = menuBar()->addMenu("&Fichier");
           QMenu *nouveau = menuFichier->addMenu("&Nouvelle note");
               QAction *actionArt=new QAction("Nouvel article", this);
               nouveau->addAction(actionArt);
               QAction* actionTache=new QAction("Nouvelle &tâche", this);
               nouveau->addAction(actionTache);
               QAction* actionImage=new QAction("Nouvelle Image", this);
               nouveau->addAction(actionImage);
            actionSave=new QAction("&Enregistrer", this);
            menuFichier->addAction(actionSave);
            actionSave->setShortcut(QKeySequence("Ctrl+S"));
            actionSave->setDisabled(true);
            connect(actionSave, SIGNAL(triggered()), this, SLOT(interfaceSave()));
            QObject::connect(noteEdit->getTitle(), SIGNAL(textChanged(QString)), this, SLOT(activerMenuSave(QString)));
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
            layoutPrincipal = new QHBoxLayout;
            layoutPrincipal->addWidget(gauche);
            layoutPrincipal->addWidget(centre);
            layoutPrincipal->addWidget(droite);
            zoneCentrale->setLayout(layoutPrincipal);
            setWindowTitle("Pluri'notes");
}

void VuePrincipale::activerMenuSave(QString str){
    actionSave->setDisabled(false);
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

void VuePrincipale::afficher_note(QTreeWidgetItem *item,int i){
    qDebug()<<"signal reçu";
    NotesManager& manager=NotesManager::getInstance();
    note = &manager.getNote(item->text(0));
    delete centre;
    delete droite;
    affichage_central();
    affichage_droit();
    layoutPrincipal->addWidget(centre);
    layoutPrincipal->addWidget(droite);
    zoneCentrale->setLayout(layoutPrincipal);
}

void VuePrincipale::affichage_central(){
        if(typeid(*note)==typeid(Article)){
            Article& a=dynamic_cast<Article&>(*note);
            noteEdit=new ArticleEditeur(a);
            //QObject::connect(a.getTexte(), SIGNAL(textChanged(QString)), this, SLOT(activerMenuSave(QString)));
        }
        else if(typeid(*note)==typeid(TacheAvecPriorite)){
            TacheAvecPriorite& t=dynamic_cast<TacheAvecPriorite&>(*note);
            noteEdit=new TacheAvecPrioriteEditeur(t);
           // QObject::connect(t.getTexte(), SIGNAL(textChanged(QString)), this, SLOT(activerMenuSave(QString)));
        }
        else if (typeid(*note)==typeid(TacheAvecDeadline)){
            TacheAvecDeadline& t=dynamic_cast<TacheAvecDeadline&>(*note);
            noteEdit=new TacheAvecDeadlineEditeur(t);
          //  QObject::connect(t.getTexte(), SIGNAL(textChanged(QString)), this, SLOT(activerMenuSave(QString)));
        }
        else if(typeid(*note)==typeid(Tache)){
            Tache& t=dynamic_cast<Tache&>(*note);
            noteEdit=new TacheEditeur(t);
          // QObject::connect(t.getTexte(), SIGNAL(textChanged(QString)), this, SLOT(activerMenuSave(QString)));
        }
        else if(typeid(*note)==typeid(Fichier)){
            Fichier& f=dynamic_cast<Fichier&>(*note);
            noteEdit=new FichierEditeur(f);
           // QObject::connect(f.getDescription(), SIGNAL(textChanged(QString)), this, SLOT(activerMenuSave(QString)));
         }
        else throw InterfaceException("Ce type de note n'existe pas");

    QFormLayout* editeur=new QFormLayout;
    editeur->addRow("", noteEdit);
    centre=new QGroupBox("Visualisation de la note", zoneCentrale);
    centre->setLayout(editeur);
}

void VuePrincipale::affichage_droit(){
    //partie droite
          QVBoxLayout* rightLayout=new QVBoxLayout;
          if (note->IsLast())
          {
              liste_relations=new QTreeWidget();
              liste_relations->setHeaderLabels(QStringList("Relations"));
              QObject::connect(liste_relations, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(afficher_note(QTreeWidgetItem*,int)));

              QTreeWidgetItem* arbreAscendants = new QTreeWidgetItem(liste_relations);
              arbreAscendants->setText(0,"Ascendants");
              arbreAscendants->setExpanded(true);
              liste_relations->addTopLevelItem(arbreAscendants);

               QTreeWidgetItem* arbreDescendants = new QTreeWidgetItem(liste_relations);
               arbreDescendants->setText(0,"Descendants");
               arbreDescendants->setExpanded(true);
               liste_relations->addTopLevelItem(arbreDescendants);

               QTreeWidgetItem** item = new QTreeWidgetItem*[10];
               unsigned int i=0;
               RelationsManager::Iterator iterator_manager=RelationsManager::getInstance().getIterator();
               while(!iterator_manager.isDone()){ //ici on examine chaque relation
                  qDebug()<<"entree dans la boucle";
                  Relation::Iterator iterator_relation= iterator_manager.current().getIterator();
                  while(!iterator_relation.isDone()) //ici on examine chaque couple de la relation
                  {
                      if(&(iterator_relation.current_noteX())==note){
                          item[i] = new QTreeWidgetItem();
                          item[i]->setText(0, iterator_relation.current_noteY().getId());
                                        //+" ("+iterator_manager.current().getTitre()+")");
                          arbreDescendants->addChild(item[i]);
                          i++;
                          qDebug()<<"Descendant trouve\n";
                      }
                      if(&(iterator_relation.current_noteY())==note){
                           item[i] = new QTreeWidgetItem();
                           item[i]->setText(0, iterator_relation.current_noteX().getId());
                                         //+" ("+iterator_manager.current().getTitre()+")");
                           arbreAscendants->addChild(item[i]);
                           i++;
                           qDebug()<<"Ascendant trouve\n";
                      }
                      iterator_relation.next();
                  }
                  qDebug()<<"Ajout de la relation "<<iterator_manager.current().getTitre()<<" à l'arborescence\n";
                  iterator_manager.next();
                }
               rightLayout->addWidget(liste_relations);
          }
          //if(note->getVersion()>1)
          //{


          //}
       arboVisible=true;
       relation_details=new QPushButton("Gerer les relations", this);
       rightLayout-> addWidget(relation_details);
       droite=new QGroupBox("Arborescence", zoneCentrale);
       droite->setLayout(rightLayout);

       connect(relation_details, SIGNAL(clicked()), this, SLOT(showRelations()));
}



//==========================================FENÊTRE SECONDAIRE (RELATIONS)========================================

VueSecondaire::VueSecondaire() : manager(&RelationsManager::getInstance()){
    //colonne de gauche : liste de toutes les relations
    quitter=new QPushButton("Quitter", this);
    gauche=new QGroupBox;
    leftLayout=new QVBoxLayout;

    arboRelations=new QTreeWidget();
    arboRelations->setHeaderLabels(QStringList("Relations"));

    QObject::connect(arboRelations, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(openRelation(QTreeWidgetItem*, int)));

    relation_titre = new QTreeWidgetItem*[manager->getNbRelations()];
    RelationsManager::Iterator it=manager->getIterator();
    unsigned int i=0;
    while(!it.isDone())
    {
        qDebug()<<"Entree dans la boucle";
        relation_titre[i]=new QTreeWidgetItem;
        relation_titre[i]->setText(0, it.current().getTitre());
        qDebug()<<"Creation de l'affichage de la relation "<<it.current().getTitre();
        arboRelations->addTopLevelItem(relation_titre[i]);
        qDebug()<<"Ajout du widget a la liste";
        it.next();
        i++;
    }
    leftLayout->addWidget(arboRelations);
    leftLayout->addWidget(quitter);
    gauche->setLayout(leftLayout);


    //partie principale
    editeur=new QFormLayout;
    editeur->addRow("", &relations);
    blocPrincipal=new QGroupBox("Gestion des relations");
    qDebug()<<"Bloc principal créé";
    blocPrincipal->setLayout(editeur);
    principal=new QVBoxLayout;
    principal->addWidget(blocPrincipal);

    layout=new QHBoxLayout;
    layout->addWidget(gauche);
    layout->addWidget(blocPrincipal);
    setLayout(layout);
    setWindowTitle("Gestion des relations");
    resize(350, 450);
   connect(quitter, SIGNAL(clicked()), this, SLOT(close()));
}


void VueSecondaire::openRelation(QTreeWidgetItem* item, int i){
    qDebug()<<"Entree dans OpenRelation";
    delete editeur;
    delete blocPrincipal;
    RelationsManager::Iterator it=manager->getIterator();
    while(!it.isDone() && it.current().getTitre()!=item->text(0)) it.next();
    if (it.isDone() && it.current().getTitre()!=item->text(0)) throw InterfaceException("Erreur, cette relation n'existe pas");
    qDebug()<<"On a trouve la bonne relation";
    editeur=new QFormLayout;
    editeur->addRow("", new RelationEditeur(it.current()));
    qDebug()<<"RelationEditeur créé";
    blocPrincipal=new QGroupBox;
    blocPrincipal->setLayout(editeur);
    layout->addWidget(blocPrincipal);

}

/*QFormLayout* editeur=new QFormLayout;
editeur->addRow("", noteEdit);
centre=new QGroupBox("Visualisation de la note", zoneCentrale);
centre->setLayout(editeur);*/
