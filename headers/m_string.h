/* ARMANGAUU Etienne 21703622 */

// d�claration de sous-programmes repr�sentant des op�rations sur le type string
// ces sous-programmes sont d�finis dans le fichier m_string.cpp
#ifndef MSTRING_H
#define MSTRING_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "classecar.h"

using namespace std;

// alias sur les types size_type
typedef string::size_type str_size;
typedef vector<string>::size_type vstr_size;
typedef vector<vector<string>>::size_type vvstr_size;

/* split -- segmente une cha�ne de caract�res en mots graphiques *
 *  donn�e : s, string, la cha�ne � segmenter                    *
 *  retour : la lisete des mots graphiques de s, vector<string>  *
 */
     vector<string> split(const string& );

/* normalisePonct -- normalise une cha�ne en ajoutant un blanc devant les signes de ponctuation      *
 *                   et un blanc devant l'apostrophe                                                 *
 * donn�e: s, const string&, la cha�ne � normaliser                                                  *
 * retour: une cha�ne, string, contenant s o� un blanc est ajout� devant chaque signe de ponctuation *
 */
     string normalisePonct(const string& );

/* supPonc -- suppression d'un signe de ponctuation � la fin d'une cha�ne *
 *  donn�e : m, sting &, une r�f�rence sur une cha�ne de caract�re        *
 *  r�sultat : m, la cha�ne r�f�renc�e nettoy�e si elle se terminait par  *
 *             un signe de ponctuation, sinon la cha�ne telle quelle      *
 */
     void supPonct(string &);



/* printVstr -- imprime un vecteur de strings                                *
 *   donn�e: vstring, vector<string>, le vecteur � imprimer                  *
 *           out, ostream&, le flot de sortie, par d�faut le flot std cout   *
 */
     void printVstr(const vector<string>&, ostream& = cout );

#endif
