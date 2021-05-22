/* ARMANGAU Etienne 21703622 */

#include "soundex.h"
#include "classecar.h"
#include "vector_string.h"

// on d�clare notre tableau map_items en static constexpr pour l'optimisation au moment du runtime
// pas d'allocation dynamique ici car c'est inutile, les valeurs sont d�j� connues � la compilation
// on gagne plus de 30% par rapport � un conteneur map allou� dynamiquement
// sur plusieurs millions de mots, c'est vraiment int�ressant comme gains en terme de vitesse d'ex�cution
static constexpr Item map_items[SIZE_MAP_ITEMS] =
{
	{'p', '1'}, {'b', '1'},
	{'d', '2'}, {'t', '2'},
	{'c', '3'}, {'q', '3'}, {'k', '3'},
	{'g', '4'}, {'j', '4'},
	{'f', '5'}, {'v', '5'},
	{'s', '6'}, {'z', '6'}, {'x', '6'},
	{'l', '7'},
	{'m', '8'}, {'n', '8'},
	{'r', '9'}
};

// fonction qui renvoie la valeur correspondant � la cl� dans le tableau de pair de mani�r� r�cursive. Si la cl� est introuvable, on lance une exception
// elle aussi d�clar�e static constexpr (inline) = gain de temps au moment de la recherche car elle est souvent appel�e
static constexpr char findValue(const char& key, const size_t& range = SIZE_MAP_ITEMS)
{
    // on cherche la valeur associ�e � la cl� par r�cursivit� en commen�ant par la fin du tableau
    // si range == 0 on d�clenche une exception, le caract�re est invalide. Ce n'est pas un caract�re consonantique.
    return (range == 0) ? throw string("Caractere invalide !") : (map_items[range - 1].first == key) ? map_items[range - 1].second : findValue(key, range - 1);
}

/* suppAposTiret -- suppression des traits d'union et des apostrophes dans une cha�ne 	*
 * entr�e : str, const string&, la cha�ne � modifier									*
 *retour : un objet string, �quivalent � str sans trait d'union et sans apostrophe 		*
 */
string suppAposTiret(const string& str)
{
	string clean_word;
	// on tient aussi compte de l'apostrophe "fran�aise" en plus de la simple quote '
	// en utilisant copy_if, back_inserter et une lambda pour ne recopier dans clean_word que les caract�res souhait�s
	// c'est la m�thode la plus rapide que j'ai pu trouver en me documentant
	// le back_inserter ajoute toujours � la fin les �l�ments au conteneur donc pas de p�nalit� de temps
	copy_if(str.cbegin(), str.cend(), back_inserter(clean_word), [&](const char& c) { return (c != '-' && c != '\'' && c != '\''); });
	return clean_word;
}

/* token -- extrait d'une cha�ne le premier mot graphique contenu dans cette chaine		*
 * entr�e : str, const string&, la cha�ne de laquelle on extrait						*
 * retour : un objet string, le premier mot graphique 									*
*/
string token(const string& str)
{
	string token;
	// si le caract�re n'est pas une espace ou un signe de ponctuation, on l'ajoute � token
	// on se d�barrasse ainsi de tous les espaces et signes de ponctuation dans la string str
	copy_if(str.cbegin(), str.cend(), back_inserter(token), [&](const char& c) { return !(estSeparateur(c)); });
	return token;
}

/* dediacr -- transforme une cha�ne qui contient des car. diacrit�s en une cha�ne ascii en minuscule
 * entr�e : s1, const string&, la cha�ne � transformer
 * retour : un objet string ne contenant que des caract�res ascii en minuscule
 * Je me suis permis de factoriser votre code en regroupant 2 fonctions en une seule car j'ai d�j� �crit une fonction qui normalise une chaine de caract�res
 * dans le module vector_string pour un autre devoir. Il s'agit de la fonction normalise_string qui prend en entr�e une chaine et qui renvoie
 * cette chaine sans diacrit�es (les formes diacrit�es sont remplac�es par leur �quivalent non diacrit�) et le tout en minuscule.
 * De ce fait, dediacr et enMin sont regroup�es dans une seule et m�me fonction.
 */
string dediacr_enMin(const string& str)
{
	// fonction d�clar�e dans le module vector_string. Elle normalise une string :
	// tout en minuscule et dediacritisation
	return normalise_string(str);
}

/* transcode -- transcodage par approximation phon�tique d'un caract�re    *
 * entr�e : c, char, le caract�re � transcoder                             *
 * retour : le code du caract�re c ou c lui-m�me                           *
 */
char transcode(const char& c)
{
	// si c'est une voyelle, on la transcode en '0'
	if (estVoyelle_fr(c)) return '0';
	//si c'est h ou w, on les garde telles quelles, sans transcodage
	else if (c == 'h'|| c == 'w') return c;
	// si c'est une consonne, on la transcode avec la table de correspondance
	else return findValue(c);
}

/* transcode -- transcodage par approximation phon�tique d'une cha�ne      *
 * entr�e : str, const string&, la cha�ne � transcoder                     *
 * retour : un objet string repr�sentant le transcodage phon�tique de str  *
 */
string transcode(const string& str)
{
	// si la string ne fait qu'un caract�re, on la renvoie telle quelle. Pas besoin de transcodage
	if (str.size() == 1) return str;
	// on initialise la string trans avec la premi�re lettre du mot telle quelle (sans transcodage)
	string trans {str.at(0)};

	// gestion des exceptions
	try
	{
		// on transcode les caract�res restants de str dans trans avec la fonction for_each() de algorithm
		// c'est plus rapide de travailler avec des string_const::iterator plut�t qu'avec une boucle for classique
		const string fin = str.substr(1);
		for_each(fin.cbegin(), fin.cend(), [&](const char& c){ trans.push_back(transcode(c)); });
		return trans;
	}
	// si le caract�re est invalide (chiffre ou autre), on en informe l'utilisateur en affichant l'exception captur�e
	catch (const string& erreur)
	{
		// affichage de l'erreur sur la sortie d'erreur
		cerr << "\n" << erreur << "\n" << endl;
		// on retourne une chaine vide
		return std::string();
	}
}

/* suppDouble -- supprime les car. redoubl�s dans une cha�ne                      *
 * entr�e : str, const string&, la cha�ne � transformer                           *
 * retour : un objet string �quivalent � str ne contenant plus aucun car. double  *
 */
string suppDouble(const string& str)
{
	// si la chaine fait un caract�re ou moins(exception d�clench�e par exemple), on la renvoie telle quelle
	if (str.size() <= 1) return str;
	
	string token{str};
	// on �limine les doublons avec unique
	auto it = unique(token.begin(), token.end(), [&](const char& c1, const char& c2){ return c1 == c2;});
	// on resize notre string avec std::distance pour avoir la taille de la string
	token.resize(distance(token.begin(), it));
	// on renvoie la string sans doublons
	return token;
}

/* ajuste -- met une cha�ne de car. � une longueur de 4 caract�res                *
 * entr�e : str, const string&, la cha�ne � formater                              *
 * retour : un objet string r�duite aux 4 premiers car. de str ou si la longueur  *
 * de str < 4, str compl�t�e avec #                                               *
 */
string ajuste(const string& str, str_size s)
{
	// chaine vide = erreur pendant le transcodage
	if (str.empty()) return str;
	// si la string fait le nombre de caract�res fix�s ou plus, on ne r�cup�re que les 4 premiers caract�res
	else if (str.size() >= s) return str.substr(0, SIZE_KEY);
	// sinon, on compl�te avec des '#' jusqu'� atteindre le nombre de caract�res demand�s avec une boucle while
	else
	{
		string fin_mot(str);
		// on compl�te la chaine avec '#' pour arriver � la taille demand�e (SIZE_KEY = 4)
		while (fin_mot.size() < SIZE_KEY) fin_mot.push_back('#');
		// on retourne la chaine compl�t�e avec des '#' pour atteindre le nombre de caract�res requis (s)
		return fin_mot;
	}
}

/* suppTouteOcc -- supprime toutes les occurrences d'un car. donn� dans une cha�ne 		*
 * entr�e : str, const string&, la cha�ne � transformer                           		*
 * cc, char, le car. � supprimer                                        				*
 * retour : un objet string �quivalent � str ne contenant plus aucune occ. de cc  		*
 */
string suppTouteOcc(const string& str, const char& cc)
{
	string unique;
	copy_if(str.cbegin(), str.cend(), back_inserter(unique), [&](const char& c) { return c != cc; });
	return unique;
}

/* cleSoundex -- g�n�ration de la cl� soundex d'une cha�ne          *
 * entr�e : mot, const string&, la cha�ne � soundexer               *
 * retour : un objet string, la cl� soundex de mot                  *
 */
string cleSoundex(const string& mot)
{
	// on g�n�re la cl� Soundex du mot
	string preprocess = dediacr_enMin(suppAposTiret(mot));
	string soundex = ajuste(suppTouteOcc(suppDouble(transcode(preprocess)), '0'));
	return soundex;
}

/* impression d'un map<string, list<string> >                        *
 * entr�e : table, r�f�rence sur le map � imprimer                   *
 * out, le flot de sortie, par d�faut cout                           *
 */
void Print(const map<string, list<string>>& all_soundex, ostream& cout)
{
	// on it�re sur chaque cl� Soundex
	for (const auto& soundex: all_soundex)
	{
		cout << "****************** clef Soundex : " << soundex.first << endl;
		// impression de la liste de mots qui partagent la m�me cl� Soundex
		for (const auto& str: soundex.second)
		{
			cout << '\t' << str << endl;
		}
	}
}

/* impression d'une list<string>                                     *
 * entr�e : l, r�f�rence sur la liste � imprimer                     *
 * out, le flot de sortie, par d�faut cout                           *
 */
void Print(const list<string>& liste, ostream& cout)
{
	// impression de la liste de mots qui partagent la m�me cl� Soundex
	for (auto str: liste)
	{
  		// on affiche chaque mot ayant la meme cl� Soundex avec notre cout en assembleur
  		str.push_back('\n');
  		// on convertit notre string en un tableau de char sur lequel pointe notre variable mot
  		const char* mot = str.c_str();
    	cout_asm(mot);
	}
	cout << "\n" << endl;
}

// fonction en assembleur inline synthaxe intel qui affiche un tableau de chars
void cout_asm(const char* message)
{
   	// pas d'argument pour printf
   	char fpargs = 0;

    __asm__ volatile (  

    "mov %%rsp, %%rbx;"  
    "and $~0xf, %%rsp;"  
    // on recupere largement assez de m�moire dans la pile
    "sub $128, %%rsp;"
    // on appelle printf   
    "call printf;"       
    "mov %%rbx, %%rsp"   
    :                    
    "+a"(fpargs),        
    "+D"(message)            
    :                    
    :                  
    "rsi", "rdx",       
    "rcx", "r8", "r9",  
    "r10", "r11",        
    "rbx",               
    "cc",                
    "memory");
}
