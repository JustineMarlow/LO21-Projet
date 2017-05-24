#include<interface.h>
#include<QGroupBox>
#include<QFormLayout>

VuePrincipale::VuePrincipale(Article& a) : article(a){
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
    ArticleEditeur* note=new ArticleEditeur(article);
    QFormLayout* editeur=new QFormLayout;
    editeur->addRow("", note);
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
    /*RelationEditeur* relations=new RelationEditeur(RelationsManager::getInstance());
    QFormLayout* editeur=new QFormLayout;
    editeur->addRow("", relations);
    QGroupBox* blocPrincipal=new QGroupBox("Gestion des relations");
    blocPrincipal->setLayout(editeur);*/

    quitter=new QPushButton("Quitter", this);
    QVBoxLayout *layout=new QVBoxLayout;
    //layout->addWidget(blocPrincipal);
    layout->addWidget(quitter);
    setLayout(layout);
    setWindowTitle("Gestion des relations");
    resize(350, 450);
   connect(quitter, SIGNAL(clicked()), this, SLOT(close()));
}


