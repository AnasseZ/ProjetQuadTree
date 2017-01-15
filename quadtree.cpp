#include <bitset>
#include <iostream>
#include <cassert>
#include <cmath>
#include "quadtree.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Interface publique
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
QuadTree::QuadTree()
{
    _taille = 0;
    _racine.pere=nullptr;

    /*for(auto f:_racine.fils)
    {
       f = nullptr;
    }*/
}

//------------------------------------------------------------------------------
QuadTree::~QuadTree()
{
	 if(_taille > 0)
    {
		for(auto f:_racine.fils)
    	{
      		if( f != nullptr){
    			deleteNoeud(f);      			
      		}
    	}
    }
	
}

//------------------------------------------------------------------------------
void QuadTree::afficher() const
{
    if ( _taille == 0 )
    {
        cout << "Quadtree vide." << endl;
    }
    else
    {
        // utilise une fonction privée récursive
        afficher_rec(&_racine);
    }
}

//------------------------------------------------------------------------------
void QuadTree::importer(const ImagePNG & img)
{	
	// on test si c'est bien une image carré
	 assert( img.hauteur() == img.largeur());

	 // On supprime le contenu de l'arbre si y'en a  comme dans le destructeur
	 if(_taille > 0)
    {
        for(auto f:_racine.fils)
        {
            if(f != nullptr)
                deleteNoeud(f);
        }
    }
	
	_taille = 0;
	int valTempo = img.largeur();

	while(valTempo > 1)
	{
		_taille++;
		valTempo/=2;
	}

	// appel recursive de notre fonction qui créera les descendances
	creerDescendants(&_racine, img.largeur(), 0, 0, img);
}

//------------------------------------------------------------------------------
ImagePNG QuadTree::exporter() const
{
	int tailleTempo = pow(2,_taille); // Représente la taille de l'image qui est de 2^n x 2^n avec n la taille de l'arbre
    ImagePNG img(tailleTempo, tailleTempo);
	
	parcoursDescendants(&_racine, tailleTempo, 0, 0, img);


    return img;
}

//------------------------------------------------------------------------------
void QuadTree::compressionDelta(unsigned delta)
{
	elagage(&_racine, delta);
}

//------------------------------------------------------------------------------
void QuadTree::compressionPhi(unsigned phi)
{
// À COMPLÉTER
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Fonctions facilitatrices (privées)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/*static*/ QuadTree::bit QuadTree::kiemeBit(unsigned n, unsigned k)
{
    
    bit b = 0;
    if ( k < 31 )
    {
        std::bitset<32> bits(n);
        b = bits[k];
    }
    return b;    
}

//------------------------------------------------------------------------------
/*static*/ void QuadTree::afficher_rec(const /*QuadTree::*/Noeud * n, string tabs/*=""*/)
{
    if (n != nullptr)
    {   // affichage du noeud
        cout << tabs << "- " << n << " de " << n->pere << " vers ";
        for (auto f : n->fils)
        {
            cout << f << " ";
        }
        cout << "RVB=" << n->rvb << endl;
        // et de ses fils récursivement
        for (auto f : n->fils)
        {
            afficher_rec(f,tabs+"   ");
        }
    }
}

//------------------------------------------------------------------------------
void QuadTree::deleteNoeud(QuadTree::Noeud * n)
{
	// On supprime tout les noeuds récursivement avec des appels sur tout les fils 

	for(auto f:n->fils)
    {
        if( f != nullptr)
            deleteNoeud(f);
    }
    delete n;
}

//------------------------------------------------------------------------------
bool QuadTree::estFeuille(Noeud * n)
{
	bool estFeuille;
	// vu que nous travaillons sur des arbres enraciné 4-aire complet il suffit de vérifier uniquement qu'un seul fils
	// La complexité temporelle est donc en O(1) càd constant.
	
	estFeuille = ( n->fils[0] != nullptr) ? false : true;

	return estFeuille;
}

//------------------------------------------------------------------------------
// fonction récursive qui créer notre quadtree
void QuadTree::creerDescendants(QuadTree::Noeud * n, int tailleNoeud, int x, int y, const ImagePNG & img)
{	
	if(tailleNoeud==1)
	{
		for(int i=0; i < 4; ++i)
        {
           n->fils[i] = nullptr;
        }

        n->rvb = img.lirePixel(x,y);
	}

	else
	{
		
        for(int i=0; i < 4; ++i)
        {
            n->fils[i] = new Noeud;
            n->fils[i]->pere = n;
        }

		
		int newTaille = tailleNoeud/2;

		// On appelle récursivement sur les 4 fils en se déplaçant sur l'image
		creerDescendants(n->fils[0], newTaille,x,y,img);
		creerDescendants(n->fils[1], newTaille,x,y+newTaille,img);
		creerDescendants(n->fils[2], newTaille,x+newTaille,y,img);
		creerDescendants(n->fils[3], newTaille,x+newTaille,y+newTaille,img);
		
		// on stock les 4 couleurs des fils dans un vector pour faire la moyenne du noeud via la méthode moyenne
		vector<Couleur> couleurNoeud = {n->fils[0]->rvb,n->fils[1]->rvb,n->fils[2]->rvb,n->fils[3]->rvb};
		n->rvb = moyenne(couleurNoeud);
	}
}

//------------------------------------------------------------------------------	
void QuadTree::parcoursDescendants(const QuadTree::Noeud * n, int tailleNoeud, int x, int y, ImagePNG & img) const
{
	
	// on modifie la couleur des pixels de l'image
	if(n->fils[0] == nullptr)
	{
		for(int i = 0; i < tailleNoeud; ++i)
		{
			for(int j = 0; j < tailleNoeud; ++j)
			{
				img.ecrirePixel(x+i,y+j,n->rvb);
			}
		}
	}

	else
	{
		int newTaille = tailleNoeud/2;

		// On appelle récursivement sur les 4 fils pour changer toute l'image
		parcoursDescendants(n->fils[0], newTaille, x, y, img);
		parcoursDescendants(n->fils[1], newTaille, x, y+newTaille, img);
		parcoursDescendants(n->fils[2], newTaille, x+newTaille, y, img);
		parcoursDescendants(n->fils[3], newTaille, x+newTaille, y+newTaille, img);
	}
}

//------------------------------------------------------------------------------	
void QuadTree::testLuminance(QuadTree::Noeud * n, unsigned delta)
{

	// on calcule la différence maximum de luminance pour l'élagage des feuilles 
	

	bool verif = true;

	for(auto f : n->fils)
	{
		verif = verif && estFeuille(f);
	}
	
	if(verif)
	{
		unsigned differencesLumi[3]; // on stock 3 différences de luminance pour faire nos test du max

		// on choisis un seuil parmi les 4 différences de luminance de couleurs(entre la couleur d'un noeud et des fils)
		// Qui changera ou non lor du for qui compare le seuil avec celles de notre tableau differencesLumi
		unsigned seuil = diff_lum(n->rvb, n->fils[0]->rvb);

		differencesLumi[0] = diff_lum(n->rvb, n->fils[1]->rvb);
		differencesLumi[1] = diff_lum(n->rvb, n->fils[2]->rvb);
		differencesLumi[2] = diff_lum(n->rvb, n->fils[3]->rvb);


		

		for(auto f:differencesLumi)
		{
			if(f > seuil) // calcul du max
				seuil = f;
		}

		if(seuil <= delta){ // compression faisable donc on élague 
			for(int i=0; i<4;++i)
			{
				delete n->fils[i];
				n->fils[i] = nullptr;
			}	

			if(n->pere != nullptr && n == n->pere->fils[3])
				testLuminance(n->pere,delta); // et on remonte dans la hiérarchie de l'arbre 
		}
	}
}

//------------------------------------------------------------------------------	
void QuadTree::elagage(QuadTree::Noeud * n, unsigned delta)
{
	if(n->fils[0]->fils[0] != nullptr)
	{
		for(auto f : n->fils)
			elagage(f,delta);
			
	}
	else
		testLuminance(n,delta);
}