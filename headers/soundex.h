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

// on définit la taille de notre table de correspondance avec une macro
#define SIZE_MAP_ITEMS 20

using namespace std;

// Item = un objet std::pair avec en clé une consonne (char) et son transcodage en valeur (char)
using Item = std::pair<const char, const char>;

// alias pour les string::size_type
using str_size = string::size_type;

// taille de la clé Soundex par défaut
static constexpr str_size SIZE_KEY = 4;

// normalisation
string suppAposTiret(const string&);
string token(const string&);
string dediacr_enMin(const string&);

// génération de la clé
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
   	// pas d'argument pour printf donc fpargs = 0 dans le type primitif des entiers non signés sur 8 bitss
   	uint8_t fpargs = 0;

    // avec volatile, on indique entre autre au compilateur qu'il ne doit pas toucher à notre code en assembleur pour tenter de l'optimiser
    // il pourrait parfaitement ajouter des instructions supplémentaires qu'il jugerait nécessaires, ce qu'on ne veut pas,
    // ou au contraire, il pourrait refactoriser notre code en réduisant le nombre des instructions. 
    // mais en général, un compilateur sait mieux qu'un bon programmeur comment optimiser le code.
    __asm__ volatile (  

    // première instruction et la plus fondamentale : on sauvegarde l'état du stack (de la pile mémoire) que pointe le registre RSP
    // on va copier cette adresse dans le registre RBX qui est utilisé comme pointeur sur les données en général car on va la modifier pour allouer 
    // de la mémoire pour notre sous-routine pour pouvoir afficher nos tokens l'un après l'autre.
    "mov %%rsp, %%rbx;"  
    "and $~0xf, %%rsp;"  
    // on s'alloue assez de mémoire depuis la pile pour afficher sans problème un token sans effet de bord
    // c'est pour cela qu'ici j'ai décidé de ne pas faire de test out of range.
    // j'ai choisi ici 37 octets (au hasard) pour avoir de la marge, ce qui fait 36 chars au maximum possibles (plus le caractère de fin de chaine '\0')
    "sub $128, %%rsp;"
    // on appelle la fonction printf pour afficher notre tableau de caractères et on lui passe en argument la variable message  
    "call printf;"       
    // on rétablit ensuite l'état de la pile tel qu'il était avant l'appel de notre sous fonction en assembleur
    // et dont on a sauvegardé l'adresse dans le registre RBX pour faire simple
    "mov %%rbx, %%rsp"   
    :                    
    "+a"(fpargs),        
    "+D"(message)            
    :                    
    :                  
    // on définit dans un clobber la liste des registres du processeur utilisés directement et indirectement pour éviter que le compilateur
    // n'y fasse des modifications qui pourraient perturber la bonne exécution de notre code
    // il pourrait par exemple se servir d'un de ces registres pour stocker le contenu de certaines variables comme la valeur de fpargs, message,
    // ou la taille de message. Cela entrainerait de forts risques de undefined behaviors (UB)
    
    // début du clobber
    "rsi", "rdx",       
    "rcx", "r8", "r9",  
    "r10", "r11",        
    "rbx",
    
    // la séquence 'cc' n'est pas un registre mais c'est un mot clé de l'extented GNU Assembly pour indiquer au compilateur explicitement que le code
    // va modifier la valeur de certains registres, dans le but d'éviter certains blocages de sécurité de sa part.
    // niveau interopérabilité entre C/C++ et assembleur,  Visual C++ est le meilleur IDE et en plus c'est la syntaxe de NASM qui est
    // beaucoup plus claire que celle d'Intel. L'assembleur est déjà assez compliqué à maitriser de base, mais l'assembleur inline 
    //l'est encore davantage surtout le GNU avec toutes les architectures processeurs possibles prises en charge.               
    "cc",
    // fin du clobber                
    "memory");
}


#endif
