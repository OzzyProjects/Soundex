/* ARMANGAUU Etienne 21703622 */

#include <array>
#include <algorithm>
#include <cctype>
#include <string_view>
#include "vector_string.h"

/* constexpr est un mot clef pour dire que la variable est une expression d�duite � la compilation. C'est-�-dire que tout le traitement effectu� 
 * pour l'initialiser ne se trouvera pas dans le programme final, seulement le r�sultat (pour faire bref) = gain de temps �norme
 * La variable normalise_char_table est initialis�e avec le retour d'une lambda ([]{ ... }()) qui est appel� directement (sur place).
 */

constexpr auto normalise_char_table = []
{
	// table = un tableau de tous les caract�res possibles
	std::array<char, 256> table {};
	for (std::size_t i = 0; i < table.size(); ++i) 
	{
		table[static_cast<unsigned char>(i)] = char(i);
	}
 
	/*utilisation des string_view car un objet string_view ne stocke pas la cha�ne de caract�res en elle-m�me
	 *puisqu'il s'agit d'une vue sur des donn�es non modifiables.
	 *Declaration de la structure Diac qui aura pour champs :
	 *char c = le caractere non diacrit� correspondant
	 *std::string_view str = la concatenation des variantes diacrit�es possibles pour ce caractere c
	 */
	struct Diac { char c; std::string_view str; };
 
	// modification de certaines correspondances
	for (Diac pair : {
	Diac{'a', "����"sv},
	Diac{'e', "��������"sv},
	Diac{'i', "����"sv},
	Diac{'o', "����"sv},
	Diac{'u', "������"sv},
	Diac{'c', "��"sv},
	Diac{'y', "�"sv},
	}) 
	{
		// on fait correspondre chaque caractere simple (pair.c) avec chacune de ses variantes diacrit�es (c)
		for (char c : pair.str) 
		{
		table[static_cast<unsigned char>(c)] = pair.c;
		}
	}	
 
	return table;
}(); // fin de l'expression lambda

/* normalise_char -- fonction qui normalise les caracteres diacrit�s par leur equivalents non diacrit�s                       *                            
 * et les caracteres non diacrit�s par leur forme minuscule                                                                   *
 * parametres : c, caractere � normaliser                                                                                     *
 * retour : le caractere equivalent non diacrit� en minuscule ou le caractere non diacrit� en minuscule                       *
 */
char normalise_char(char c)
{
	// si le caractere est en majuscule, on le met en minuscule
	if (isupper(c)) c = tolower(c);
	// on renvoie sa correspondance non diacrit� et en minuscule dans le tableau normalise_char_table
	return normalise_char_table[static_cast<unsigned char>(c)];
}

/* compareSC -- verifie qu'une string en precede une autre dans l'ordre alphabetique *
 * insensiblement � la casse et aux diacrit�es                                       *
 * parametres : string1 et string2, const string&, les 2 chaines � comparer          *
 * retour : un boolean, true si string1 < string2, false sinon                       *
 */
bool compareSC(const string& string1, const string& string2)
{
    
    // on instancie nos interateurs au debut de chaque string
    str_it iter1 = string1.begin(), iter2 = string2.begin();
    
    // si les deux strings ne font qu'un seul caractere et sont identiques, on met celle diacrit�e en premier dans l'ordre
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
        
        // on incr�mente nos iterateurs
        iter1++;
        iter2++;
    }
    
    // les deux chaines sont identiques
    return false;

}// compareSC

/* egaleSC -- affirme que 2 strings sont �gales ind�pendamment de la casse   *
 * donn�es : string1 et string2, const string&, les 2 chaines à comparer    *
 * retour : un boolean, true si string1 = string2, false sinon               *
 */
bool egaleSC(const string& string1, const string& string2)
{
    // on instancie nos iterateurs au debut de chaque string
    str_it iter1 = string1.begin(), iter2 = string2.begin();

    // si les deux strings sont de taille diff�rente, elles sont forcement diff�rentes
    if (string1.size() != string2.size()) return false;
    
    // tant qu'on n'a pas atteint la fin de nos 2 strings
    while(true)
    {
    	if (iter1 == string1.end() and iter2 == string2.end()) break;
        // les deux strings sont diff�rentes donc on retourne false
        if (tolower(*iter1) != tolower(*iter2) )
        {
			return false;
		}             
         // on incr�mente nos iterateurs
         iter1++;
         iter2++;
	 }
	 
	 // les deux chaines sont identiques
	 return true;
}// egaleSC

/* fonction qui normalise les caract�res d'une string en vue d'une comparaison. Elle remplace les diacrit�es par leur �quivalent non diacrit�s   *
 * et elle convertit la string en minuscule uniquement en g�n�rant une nouvelle string                                                           *
 */
string normalise_string(const string& s)
{
	string normalisee;
	for (const auto& c : s)
	{
		// on standardise chaque caract�re de la chaine (minuscule et remplacement des diacrit�es)
		normalisee.push_back(normalise_char(c));
	}
	// On renvoie la chaine normalis�e
	return normalisee;
}