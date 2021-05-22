/* ARMANGAUU Etienne 21703622 */

#include <array>
#include <algorithm>
#include <cctype>
#include <string_view>
#include "vector_string.h"

/* constexpr est un mot clef pour dire que la variable est une expression déduite à la compilation. C'est-à-dire que tout le traitement effectué 
 * pour l'initialiser ne se trouvera pas dans le programme final, seulement le résultat (pour faire bref) = gain de temps énorme
 * La variable normalise_char_table est initialisée avec le retour d'une lambda ([]{ ... }()) qui est appelé directement (sur place).
 */

constexpr auto normalise_char_table = []
{
	// table = un tableau de tous les caractères possibles
	std::array<char, 256> table {};
	for (std::size_t i = 0; i < table.size(); ++i) 
	{
		table[static_cast<unsigned char>(i)] = char(i);
	}
 
	/*utilisation des string_view car un objet string_view ne stocke pas la chaîne de caractères en elle-même
	 *puisqu'il s'agit d'une vue sur des données non modifiables.
	 *Declaration de la structure Diac qui aura pour champs :
	 *char c = le caractere non diacrité correspondant
	 *std::string_view str = la concatenation des variantes diacritées possibles pour ce caractere c
	 */
	struct Diac { char c; std::string_view str; };
 
	// modification de certaines correspondances
	for (Diac pair : {
	Diac{'a', "àÀâÂ"sv},
	Diac{'e', "éèêëÉÈÊË"sv},
	Diac{'i', "îïÎÏ"sv},
	Diac{'o', "ôöÔÖ"sv},
	Diac{'u', "ùûüÙÛÜ"sv},
	Diac{'c', "çÇ"sv},
	Diac{'y', "ÿ"sv},
	}) 
	{
		// on fait correspondre chaque caractere simple (pair.c) avec chacune de ses variantes diacritées (c)
		for (char c : pair.str) 
		{
		table[static_cast<unsigned char>(c)] = pair.c;
		}
	}	
 
	return table;
}(); // fin de l'expression lambda

/* normalise_char -- fonction qui normalise les caracteres diacrités par leur equivalents non diacrités                       *                            
 * et les caracteres non diacrités par leur forme minuscule                                                                   *
 * parametres : c, caractere à normaliser                                                                                     *
 * retour : le caractere equivalent non diacrité en minuscule ou le caractere non diacrité en minuscule                       *
 */
char normalise_char(char c)
{
	// si le caractere est en majuscule, on le met en minuscule
	if (isupper(c)) c = tolower(c);
	// on renvoie sa correspondance non diacrité et en minuscule dans le tableau normalise_char_table
	return normalise_char_table[static_cast<unsigned char>(c)];
}

/* compareSC -- verifie qu'une string en precede une autre dans l'ordre alphabetique *
 * insensiblement à la casse et aux diacritées                                       *
 * parametres : string1 et string2, const string&, les 2 chaines à comparer          *
 * retour : un boolean, true si string1 < string2, false sinon                       *
 */
bool compareSC(const string& string1, const string& string2)
{
    
    // on instancie nos interateurs au debut de chaque string
    str_it iter1 = string1.begin(), iter2 = string2.begin();
    
    // si les deux strings ne font qu'un seul caractere et sont identiques, on met celle diacritée en premier dans l'ordre
    if (string1.size() == 1 && string2.size() == 1 && normalise_char(string1[0]) == normalise_char(string2[0])) return string1 > string2;
    
    // tant qu'on n'a pas atteint la fin de nos 2 strings
    while(!(iter1 == string1.end() && iter2 == string2.end()))
    {
        if (normalise_char(*iter1) > normalise_char(*iter2) )
        {
			// string1 succede string2 donc on retourne false
            return false; 
        }
        else if ( normalise_char(*iter1) < normalise_char(*iter2) )
        {
             // string1 precede string2 donc on retourne true
             return true;
        }
        
        // on incrémente nos iterateurs
        iter1++;
        iter2++;
    }
    
    // les deux chaines sont identiques
    return false;

}// compareSC

/* egaleSC -- affirme que 2 strings sont égales indépendamment de la casse   *
 * données : string1 et string2, const string&, les 2 chaines Ã  comparer    *
 * retour : un boolean, true si string1 = string2, false sinon               *
 */
bool egaleSC(const string& string1, const string& string2)
{
    // on instancie nos iterateurs au debut de chaque string
    str_it iter1 = string1.begin(), iter2 = string2.begin();

    // si les deux strings sont de taille différente, elles sont forcement différentes
    if (string1.size() != string2.size()) return false;
    
    // tant qu'on n'a pas atteint la fin de nos 2 strings
    while(true)
    {
    	if (iter1 == string1.end() and iter2 == string2.end()) break;
        // les deux strings sont différentes donc on retourne false
        if (tolower(*iter1) != tolower(*iter2) )
        {
			return false;
		}             
         // on incrémente nos iterateurs
         iter1++;
         iter2++;
	 }
	 
	 // les deux chaines sont identiques
	 return true;
}// egaleSC

/* fonction qui normalise les caractères d'une string en vue d'une comparaison. Elle remplace les diacritées par leur équivalent non diacrités   *
 * et elle convertit la string en minuscule uniquement en générant une nouvelle string                                                           *
 */
string normalise_string(const string& s)
{
	string normalisee;
	for (const auto& c : s)
	{
		// on standardise chaque caractère de la chaine (minuscule et remplacement des diacritées)
		normalisee.push_back(normalise_char(c));
	}
	// On renvoie la chaine normalisée
	return normalisee;
}