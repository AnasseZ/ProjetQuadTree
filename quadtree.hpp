#ifndef __QUADTREE_HPP__
#define __QUADTREE_HPP__
/**
 * @file quadtree.hpp
 * @author Zougarh Anasse, Gouhier Thomas
 * @since Avril 2016
 * @brief Fichier définissant une classe QuadTree pour représenter, 
 * et compresser, des images bitmap
 * 
 * 
**/

#include <queue>
#include <vector>
#include "imagepng.hpp"

/**
 * @brief Classe QuadTree qui représente une image bitmap sous forme arborescente
 * 
 * À COMPLÉTER
**/
class QuadTree
{
    public:
        /** --------------------------------------------------------------------
         * @brief Constructeur d'un quadtree vide
         * 
         * @b Complexité  θ(1)
        **/
        QuadTree();

        /** --------------------------------------------------------------------
         * @brief Destructeur d'un quadtree
         * 
         * @b Complexité  θ(_taille)
        **/
        ~QuadTree();

        /** --------------------------------------------------------------------
         * @brief Affichage textuel préfixe indenté du Quadtree
         * @attention À fin de débogage
         * 
         * @b Complexité  θ(_taille)
        **/
        void afficher() const;

        /** --------------------------------------------------------------------
         * @brief Encodage d'une image dans le QuadTree
         * @param img L'image à représenter
         * @pre L'image est de taille 2^n * 2^n, avec n>0
         * 
         * @b Complexité  θ(H*L)
        **/
        void importer(const ImagePNG & img);

        /** --------------------------------------------------------------------
         * @brief Exportation d'une image depuis le QuadTree
         * @return l'image bitmap correspondant au contenu du QuadTree
         * 
         * @b Complexité  θ(_taille²)
        **/
        ImagePNG exporter() const;

        /** --------------------------------------------------------------------
         * @brief Compression du QuadTree
         * @param delta le seuil maximal de différence de luminance (cf. sujet).
         * La valeur par défaut, delta=0, provoque une compression sans dégradation.
         * @pre delta<255
         * 
         * @b Complexité Θ(H*L,delta,pixels de l'image) 
        **/
        void compressionDelta(unsigned delta);

        /** --------------------------------------------------------------------
         * @brief Compression du QuadTree
         * @param phi le nombre maximal de feuilles après compression (cf. sujet).
         * @pre phi>0 car on ne peut supprimer la racine
         * 
         * @b Complexité À COMPLÉTER
        **/

        void compressionPhi(unsigned phi);


    private:
    //////////////////////////////////////////////////////////////////////////////////////
    // Attributs et types internes représentant la structure arborescente
    //////////////////////////////////////////////////////////////////////////////////////

        
        unsigned _taille;

        // Type des noeuds de l'arbre (structure pere-fils)
        struct Noeud
        {
            Noeud* pere; // nullptr si racine
            Noeud* fils[4]; // nullptr si feuille
            Couleur rvb; // du pixel si feuille, moyenne des fils sinon
        };
        
        // Noeud racine
        Noeud _racine;
        
    //////////////////////////////////////////////////////////////////////////////////////
    // Fonctionnalités internes simplifiant l'implémentation des méthodes
    //////////////////////////////////////////////////////////////////////////////////////

        // Type interne représentant 1 bit (valeur 0 ou 1)
        typedef bool bit;

        // Fonction statique retournant la valeur du k-ième bit d'un entier positif n
        static bit kiemeBit(unsigned n, unsigned k);

        // Fonction statique réalisant l'affichage récursivement depuis le noeud *n
        static void afficher_rec(const Noeud * n, std::string tabs="");
        
        // Fonctionnalités internes que nous avons rajoutés
        // Explications dans le fichier cpp
        void deleteNoeud(Noeud * n);

        bool estFeuille(Noeud * n);
        
        void creerDescendants(Noeud * n, int tailleNoeud, int x, int y, const ImagePNG & img);
        
        void parcoursDescendants(const Noeud * n, int tailleNoeud, int x, int y, ImagePNG & img) const;

		void testLuminance(Noeud * n, unsigned delta);

        void elagage(Noeud * n, unsigned delta);

       
       
};

#endif
