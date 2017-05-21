#include<interface.h>
#include<QGroupBox>
#include<QFormLayout>
VuePrincipale::VuePrincipale(Article& a) : article(a){

    //partie gauche
    QVBoxLayout* leftLayout=new QVBoxLayout;
    QGroupBox* notesActives=new QGroupBox("Notes Actives");
    QGroupBox* affichageErgo=new QGroupBox("Tâches");
    QGroupBox* archives=new QGroupBox("Archives");
    leftLayout->addWidget(notesActives);
    leftLayout->addWidget(affichageErgo);
    leftLayout->addWidget(archives);
    QGroupBox* gauche=new QGroupBox("");
    gauche->setLayout(leftLayout);


    //partie centrale
    ArticleEditeur* note=new ArticleEditeur(article);
    QFormLayout* editeur=new QFormLayout;
    editeur->addRow("", note);
    QGroupBox* centre=new QGroupBox("Visualisation de l'article");
    centre->setLayout(editeur);

    //partie droite
    Relation reference("Reference","La note x fait reference a la note y",false);
    relation_titre= new QLabel(reference.getTitre());
    relation_description= new QLabel(reference.getDescription());
    relation_details=new QPushButton("Plus de détails");
    QVBoxLayout* rightLayout=new QVBoxLayout;
    rightLayout->addWidget(relation_titre);
    rightLayout-> addWidget(relation_description);
    rightLayout-> addWidget(relation_details);
    QGroupBox* droite=new QGroupBox("Arborescence des relations");
    droite->setLayout(rightLayout);

    //Définition du layout principal
     QHBoxLayout* layoutPrincipal = new QHBoxLayout;
     layoutPrincipal->addWidget(gauche);
     layoutPrincipal->addWidget(centre);
     layoutPrincipal->addWidget(droite);

     setLayout(layoutPrincipal);
     setWindowTitle("Pluri'notes");
     //setWindowIcon(QIcon("icone.png"));
     resize(400, 450);

}
