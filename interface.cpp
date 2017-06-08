#include<interface.h>
#include<typeinfo>
#include<QGroupBox>
#include<QFormLayout>

//=============================FENÊTRE PRINCIPALE======================================


VuePrincipale::VuePrincipale(Note* n) : note(n){
    zoneCentrale=new QWidget;
    setCentralWidget(zoneCentrale);

    //gauche
        affichage_gauche();

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
            layoutPrincipal = new QHBoxLayout;
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

void VuePrincipale::afficher_version(QTreeWidgetItem *item,int i){
    qDebug()<<"signal reçu";
    NotesManager& manager=NotesManager::getInstance();
    bool conversion;
    int version=item->text(0).toUInt(&conversion, 10);
    QString id=note->getId();
    note = &manager.getVersionNote(id,version);
    delete centre;
    delete droite;
    affichage_central();
    affichage_droit();
    layoutPrincipal->addWidget(centre);
    layoutPrincipal->addWidget(droite);
    zoneCentrale->setLayout(layoutPrincipal);
    qDebug()<<"signal reçu";
}

void VuePrincipale::affichage_central(){

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
centre=new QGroupBox("Visualisation de la note", zoneCentrale);
centre->setLayout(editeur);
}

void VuePrincipale::affichage_droit(){
          QVBoxLayout* rightLayout=new QVBoxLayout;
          if (note->IsLast())
          {
              QTreeWidget* liste_relations=new QTreeWidget(this);
              QStringList header;
              header<<"Relations"<<"";
              liste_relations->setHeaderLabels(header);
              liste_relations->setColumnCount(2);
              QObject::connect(liste_relations, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(afficher_note(QTreeWidgetItem*,int)));

              QTreeWidgetItem* arbreAscendants = new QTreeWidgetItem(liste_relations);
              arbreAscendants->setText(0,"Ascendants");
              arbreAscendants->setExpanded(true);
              liste_relations->addTopLevelItem(arbreAscendants);

               QTreeWidgetItem* arbreDescendants = new QTreeWidgetItem(liste_relations);
               arbreDescendants->setText(0,"Descendants");
               arbreDescendants->setExpanded(true);
               liste_relations->addTopLevelItem(arbreDescendants);

               unsigned int nb_items_relations=2;
               QTreeWidgetItem** item = new QTreeWidgetItem*[nb_items_relations];
               unsigned int i=0;
               RelationsManager::Iterator iterator_manager=RelationsManager::getInstance().getIterator();
               while(!iterator_manager.isDone()){ //ici on examine chaque relation
                  qDebug()<<"entree dans la boucle";
                  Relation::Iterator iterator_relation= iterator_manager.current().getIterator();
                  while(!iterator_relation.isDone()) //ici on examine chaque couple de la relation
                  {
                      if(i>nb_items_relations-2) //il reste moins de 2 cases dans le tableau d'items, il faut l'agrandir
                      {
                          qDebug()<<"agrandissement tableau d'item\n";
                          QTreeWidgetItem** new_item = new QTreeWidgetItem*[nb_items_relations+10];
                          for (unsigned int k=0; k<nb_items_relations; k++)
                              new_item[k] = item[k];
                          QTreeWidgetItem** old_item = item;
                          item=new_item;
                          delete [] old_item;
                          nb_items_relations+=10;
                      }
                      if(&(iterator_relation.current_noteX())==note){
                          item[i] = new QTreeWidgetItem();
                          item[i]->setText(0, iterator_relation.current_noteY().getId());
                          item[i]->setText(1,"("+iterator_manager.current().getTitre()+")");
                          arbreDescendants->addChild(item[i]);
                          i++;
                          qDebug()<<"Descendant trouve\n";
                      }
                      if(&(iterator_relation.current_noteY())==note){
                           item[i] = new QTreeWidgetItem();
                           item[i]->setText(0, iterator_relation.current_noteX().getId());
                           item[i]->setText(1,"("+iterator_manager.current().getTitre()+")");
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
          NotesManager& manager=NotesManager::getInstance();
          unsigned int nb_versions=manager.getNote(note->getId()).getVersion();
          if(nb_versions>1)
          {
              QTreeWidget* liste_versions=new QTreeWidget(this);
              liste_versions->setHeaderLabels(QStringList("Versions"));
              QObject::connect(liste_versions, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(afficher_version(QTreeWidgetItem*,int)));
              QTreeWidgetItem** item_version = new QTreeWidgetItem*[nb_versions];
              for (unsigned int i=0; i<=nb_versions-1; i++)
              {
                  item_version[i] = new QTreeWidgetItem();
                  item_version[i]->setText(0, QString::number(i+1));
                  liste_versions->addTopLevelItem(item_version[i]);
              }
              rightLayout->addWidget(liste_versions);

          }
       arboVisible=true;
       relation_details=new QPushButton("Gerer les relations", this);
       rightLayout-> addWidget(relation_details);
       droite=new QGroupBox("Arborescence", zoneCentrale);
       droite->setLayout(rightLayout);

       connect(relation_details, SIGNAL(clicked()), this, SLOT(showRelations()));
}

void VuePrincipale::affichage_gauche(){
    NotesManager& manager=NotesManager::getInstance();
    NotesManager::Iterator iterator=manager.getIterator();
    QVBoxLayout* leftLayout=new QVBoxLayout;
    QTreeWidget* notesActives=new QTreeWidget(this);
    notesActives->setHeaderLabels(QStringList("Actives"));
    QObject::connect(notesActives, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(afficher_note(QTreeWidgetItem*,int)));
    QTreeWidget* taches=new QTreeWidget(this);
    taches->setHeaderLabels(QStringList("Taches"));
    QObject::connect(taches, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(afficher_note(QTreeWidgetItem*,int)));
    QTreeWidget* archives=new QTreeWidget(this);
    archives->setHeaderLabels(QStringList("Archives"));
    QObject::connect(archives, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(afficher_note(QTreeWidgetItem*,int)));
    unsigned int nb_items_relations=2;
    QTreeWidgetItem** item = new QTreeWidgetItem*[nb_items_relations];
    unsigned int i=0;
    for (iterator.debut(); !iterator.isDone(); iterator.next())
    {
        if(i=nb_items_relations)
        {
            qDebug()<<"agrandissement tableau d'item\n";
            QTreeWidgetItem** new_item = new QTreeWidgetItem*[nb_items_relations+10];
            for (unsigned int k=0; k<nb_items_relations; k++)
                new_item[k] = item[k];
            QTreeWidgetItem** old_item = item;
            item=new_item;
            delete [] old_item;
            nb_items_relations+=10;
        }

        if (iterator.current().IsLast())
        {
            item[i]=new QTreeWidgetItem();
            item[i]->setText(0, iterator.current().getId());
            if (iterator.current().getEtat()==active)
            {
                notesActives->addTopLevelItem(item[i]);
                if (typeid(iterator.current())==typeid(Tache))
                    taches->addTopLevelItem(item[i]);
            }
            else if (iterator.current().getEtat()==archivee)
                archives->addTopLevelItem(item[i]);
        }
    }

    arborescence=new QPushButton("Masquer l'arborescence", this);
    leftLayout->addWidget(notesActives);
    leftLayout->addWidget(taches);
    leftLayout->addWidget(archives);
    leftLayout->addWidget(arborescence);
    gauche=new QGroupBox("", zoneCentrale);
    gauche->setLayout(leftLayout);

    connect(arborescence, SIGNAL(clicked()), this, SLOT(afficageArbo()));
}



//==========================================FENÊTRE SECONDAIRE (RELATIONS)========================================

VueSecondaire::VueSecondaire() : manager(&RelationsManager::getInstance()){
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

