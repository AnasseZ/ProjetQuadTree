/* @brief: Fichier test principal
*  @author: Anasse Zougarh, Thomas Gouhier, 402
*
*/


#include "imagepng.hpp"
#include "quadtree.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <fstream>



clock_t chrono;
#define START chrono=clock();
#define STOP chrono=clock()-chrono;
#define TEMPS double(chrono)/(CLOCKS_PER_SEC/1000) // en milli secondes


using namespace std;

/** --------------------------------------------------------------------
 * @brief  Fonction permettant de créer des images dont les pixels sont choisis aléatoirement
 * @param 
 * @pre 
 *
 **/
void imageAleatoire()
{
    int taillePixel[11] = {2,4,8,16,32,64,128,256,512,1024,2048}; // différentes tailles d'images

    for (int z = 0; z < 11; ++z)
    {
        ImagePNG img(taillePixel[z],taillePixel[z]);
        Couleur c;

        for(int i=0; i< taillePixel[z]; ++i)
        {
            for(int j=0; j<taillePixel[z]; ++j)
            {
                c.R = rand() % 256;
                c.V = rand() % 256;
                c.B = rand() % 256;
                img.ecrirePixel((unsigned)i,(unsigned)j,c);
            }   
        }
        img.sauver(to_string(taillePixel[z])+".png");
    } 
} 


/** --------------------------------------------------------------------
 * @brief  Créer des images d'une seule couleur
 * @param 
 * @pre 
 *
 **/
void imageUniforme()
{
    int taillePixel[11] = {2,4,8,16,32,64,128,256,512,1024,2048}; // différentes tailles d'images

    for (int z = 0; z < 11; ++z)
    {   
        int uni = rand() % 256;

        ImagePNG img(taillePixel[z],taillePixel[z]);
        Couleur c;
        c.R = uni;
        c.V = uni;
        c.B = uni;

        for(int i=0; i< taillePixel[z]; ++i)
        {
            for(int j=0; j<taillePixel[z]; ++j)
            {        
                img.ecrirePixel((unsigned)i,(unsigned)j,c);
            }   
        }
        img.sauver(to_string(taillePixel[z])+".png");
    } 
} 

/** --------------------------------------------------------------------
 * @brief  Fonction qui calcule le temps de la fonction importer
 * @param  la taille de l'image
 * @pre    taille parmi {2^1,...,2^n) avec n appartenant à {1,...,11}
 *
 **/
void testImporter(int taille) // choisir la taille de l'image qu'on veut importer
{
    string fichier;
    ImagePNG originale;
    QuadTree arbre;

    fichier = to_string(taille)+".png";
    originale.charger(fichier);

    START;
    arbre.importer(originale);
    STOP;
    
    cout << "Temps de l'importation de l'image de  taille " << taille << "x" << taille << "pixels : " << TEMPS << "ms" << endl;
} 

/** --------------------------------------------------------------------
 * @brief  Fonction qui calcule le temps de la fonction exporter
 * @param 
 * @pre 
 *
 **/
void testExporter()
{   
    int taillePixel[11] = {2,4,8,16,32,64,128,256,512,1024,2048};

    for (int i = 0; i < 11; ++i)
    {
        string fichier;
        ImagePNG originale;
        QuadTree arbre;
    
        fichier = to_string(taillePixel[i])+".png";
        originale.charger(fichier);
        arbre.importer(originale);
  
        START;
        originale = arbre.exporter();
        STOP;

        cout << "Temps de l'exportation de l'image de  taille " << taillePixel[i] << "x" << taillePixel[i] << "pixels : " << TEMPS << "ms" << endl;
    }
}

/** --------------------------------------------------------------------
 * @brief  Fonction qui calcule le temps d'execution de la compression delta avec 6 delta différents
 * @param  la taille de l'image
 * @pre    taille parmi {2^1,...,2^n) avec n appartenant à {1,...,11}
 *
 **/
void testVariationDelta(int taille) 
{

    string fichier;
    ImagePNG originale;
    QuadTree arbre;

    int differentsDelta[6]={0,25,50,75,100,128};

    fichier = to_string(taille)+".png";
    originale.charger(fichier);
    arbre.importer(originale);
    
    
   for (int i = 0; i < 6; ++i)
   {   
        START; 
        arbre.compressionDelta(differentsDelta[i]);
        STOP;

        cout << "Temps pour delta =  " << differentsDelta[i] << "   : " << TEMPS << "ms" << endl;
        originale.charger(fichier);
        arbre.importer(originale);
   }
}

/** --------------------------------------------------------------------
 * @brief  Fonction qui calcule le temps d'execution de la compression delta avec delta compris entre 0 et 128 compris
 * @param  la taille de l'image
 * @pre    taille parmi {2^1,...,2^n) avec n appartenant à {1,...,11}
 *
 **/
void testVariationDeltaUnAUn(int taille) 
{

    string fichier;
    ImagePNG originale;
    QuadTree arbre;

    

    fichier = to_string(taille)+".png";
    originale.charger(fichier);
    arbre.importer(originale);
    
    
   for (int i = 0; i < 129; ++i)
   {   
        START; 
        arbre.compressionDelta(i);
        STOP;

        cout << "Temps pour delta =  " << i << "   : " << TEMPS << "ms" << endl;
        originale.charger(fichier);
        arbre.importer(originale);
   }
}

int main()
{

    // CHOISIR UNE FONCTION POUR EFFECTUER UN TEST

    // testImporter(512); 
    
    // imageUniforme();
    // testVariationDelta(1024);

    testVariationDeltaUnAUn(1024);
   
    //testExporter();

    //imageAleatoire();

    

    return 0;
}
