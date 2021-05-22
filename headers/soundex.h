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
void cout_asm(const char*);

#endif
