/* ARMANGAU Etienne 21703622 */

#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <string>
#include <iostream>
#include <algorithm>
#include <list>
#include <map>
#include <utility>
#include <iterator>
#include <cstring>
#include <cstdio>

// on d�finit la taille de notre table de correspondance avec une macro
#define SIZE_MAP_ITEMS 20

using namespace std;

// Item = un objet std::pair avec en cl� une consonne (char) et son transcodage en valeur (char)
using Item = std::pair<const char, const char>;

// alias pour les string::size_type
using str_size = string::size_type;

// taille de la cl� Soundex par d�faut
static constexpr str_size SIZE_KEY = 4;

// normalisation
string suppAposTiret(const string&);
string token(const string&);
string dediacr_enMin(const string&);

// g�n�ration de la cl�
char transcode(const char&);
string transcode(const string&);
string suppDouble(const string&);
string suppTouteOcc(const string&, const char&);
string ajuste(const string&, str_size=SIZE_KEY);
string cleSoundex(const string&);

// impression
void Print(const map<string, list<string>>&, ostream& = cout);
void Print(const list<string>&, ostream& = cout );

// fonction en assembleur inline syntaxe Intel qui affiche un tableau de chars
inline void cout_asm(const char* message)
{
   	// pas d'argument pour printf donc fpargs = 0 dans le type primitif des entiers non sign�s sur 8 bitss
   	uint8_t fpargs = 0;

    // avec volatile, on indique entre autre au compilateur qu'il ne doit pas toucher � notre code en assembleur pour tenter de l'optimiser
    // il pourrait parfaitement ajouter des instructions suppl�mentaires qu'il jugerait n�cessaires, ce qu'on ne veut pas,
    // ou au contraire, il pourrait refactoriser notre code en r�duisant le nombre des instructions. 
    // mais en g�n�ral, un compilateur sait mieux qu'un bon programmeur comment optimiser le code.
    __asm__ volatile (  

    // premi�re instruction et la plus fondamentale : on sauvegarde l'�tat du stack (de la pile m�moire) que pointe le registre RSP
    // on va copier cette adresse dans le registre RBX qui est utilis� comme pointeur sur les donn�es en g�n�ral car on va la modifier pour allouer 
    // de la m�moire pour notre sous-routine pour pouvoir afficher nos tokens l'un apr�s l'autre.
    "mov %%rsp, %%rbx;"  
    "and $~0xf, %%rsp;"  
    // on s'alloue assez de m�moire depuis la pile pour afficher sans probl�me un token sans effet de bord
    // c'est pour cela qu'ici j'ai d�cid� de ne pas faire de test out of range.
    // j'ai choisi ici 37 octets (au hasard) pour avoir de la marge, ce qui fait 36 chars au maximum possibles (plus le caract�re de fin de chaine '\0')
    "sub $128, %%rsp;"
    // on appelle la fonction printf pour afficher notre tableau de caract�res et on lui passe en argument la variable message  
    "call printf;"       
    // on r�tablit ensuite l'�tat de la pile tel qu'il �tait avant l'appel de notre sous fonction en assembleur
    // et dont on a sauvegard� l'adresse dans le registre RBX pour faire simple
    "mov %%rbx, %%rsp"   
    :                    
    "+a"(fpargs),        
    "+D"(message)            
    :                    
    :                  
    // on d�finit dans un clobber la liste des registres du processeur utilis�s directement et indirectement pour �viter que le compilateur
    // n'y fasse des modifications qui pourraient perturber la bonne ex�cution de notre code
    // il pourrait par exemple se servir d'un de ces registres pour stocker le contenu de certaines variables comme la valeur de fpargs, message,
    // ou la taille de message. Cela entrainerait de forts risques de undefined behaviors (UB)
    
    // d�but du clobber
    "rsi", "rdx",       
    "rcx", "r8", "r9",  
    "r10", "r11",        
    "rbx",
    
    // la s�quence 'cc' n'est pas un registre mais c'est un mot cl� de l'extented GNU Assembly pour indiquer au compilateur explicitement que le code
    // va modifier la valeur de certains registres, dans le but d'�viter certains blocages de s�curit� de sa part.
    // niveau interop�rabilit� entre C/C++ et assembleur,  Visual C++ est le meilleur IDE et en plus c'est la syntaxe de NASM qui est
    // beaucoup plus claire que celle d'Intel. L'assembleur est d�j� assez compliqu� � maitriser de base, mais l'assembleur inline 
    //l'est encore davantage surtout le GNU avec toutes les architectures processeurs possibles prises en charge.               
    "cc",
    // fin du clobber                
    "memory");
}


#endif
