/* ARMANGAU Etienne 21703622 */

#include "soundex.h"
#include "classecar.h"
#include "vector_string.h"

// on déclare notre tableau map_items en static constexpr pour l'optimisation au moment du runtime
// pas d'allocation dynamique ici car c'est inutile, les valeurs sont déjà connues à la compilation
// on gagne plus de 30% par rapport à un conteneur map alloué dynamiquement
// sur plusieurs millions de mots, c'est vraiment intéressant comme gains en terme de vitesse d'exécution
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

// fonction qui renvoie la valeur correspondant à la clé dans le tableau de pair de maniéré récursive. Si la clé est introuvable, on lance une exception
// elle aussi déclarée static constexpr (inline) = gain de temps au moment de la recherche car elle est souvent appelée
static constexpr char findValue(const char& key, const size_t& range = SIZE_MAP_ITEMS)
{
    // on cherche la valeur associée à la clé par récursivité en commençant par la fin du tableau
    // si range == 0 on déclenche une exception, le caractère est invalide. Ce n'est pas un caractère consonantique.
    return (range == 0) ? throw string("Caractere invalide !") : (map_items[range - 1].first == key) ? map_items[range - 1].second : findValue(key, range - 1);
}

/* suppAposTiret -- suppression des traits d'union et des apostrophes dans une chaîne 	*
 * entrée : str, const string&, la chaîne à modifier									*
 *retour : un objet string, équivalent à str sans trait d'union et sans apostrophe 		*
 */
string suppAposTiret(const string& str)
{
	string clean_word;
	// on tient aussi compte de l'apostrophe "française" en plus de la simple quote '
	// en utilisant copy_if, back_inserter et une lambda pour ne recopier dans clean_word que les caractères souhaités
	// c'est la méthode la plus rapide que j'ai pu trouver en me documentant
	// le back_inserter ajoute toujours à la fin les éléments au conteneur donc pas de pénalité de temps
	copy_if(str.cbegin(), str.cend(), back_inserter(clean_word), [&](const char& c) { return (c != '-' && c != '\'' && c != '\''); });
	return clean_word;
}

/* token -- extrait d'une chaîne le premier mot graphique contenu dans cette chaine		*
 * entrée : str, const string&, la chaîne de laquelle on extrait						*
 * retour : un objet string, le premier mot graphique 									*
*/
string token(const string& str)
{
	string token;
	// si le caractère n'est pas une espace ou un signe de ponctuation, on l'ajoute à token
	// on se débarrasse ainsi de tous les espaces et signes de ponctuation dans la string str
	copy_if(str.cbegin(), str.cend(), back_inserter(token), [&](const char& c) { return !(estSeparateur(c)); });
	return token;
}

/* dediacr -- transforme une chaîne qui contient des car. diacrités en une chaîne ascii en minuscule
 * entrée : s1, const string&, la chaîne à transformer
 * retour : un objet string ne contenant que des caractères ascii en minuscule
 * Je me suis permis de factoriser votre code en regroupant 2 fonctions en une seule car j'ai déjà écrit une fonction qui normalise une chaine de caractères
 * dans le module vector_string pour un autre devoir. Il s'agit de la fonction normalise_string qui prend en entrée une chaine et qui renvoie
 * cette chaine sans diacritées (les formes diacritées sont remplacées par leur équivalent non diacrité) et le tout en minuscule.
 * De ce fait, dediacr et enMin sont regroupées dans une seule et même fonction.
 */
string dediacr_enMin(const string& str)
{
	// fonction déclarée dans le module vector_string. Elle normalise une string :
	// tout en minuscule et dediacritisation
	return normalise_string(str);
}

/* transcode -- transcodage par approximation phonétique d'un caractère    *
 * entrée : c, char, le caractère à transcoder                             *
 * retour : le code du caractère c ou c lui-même                           *
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

/* transcode -- transcodage par approximation phonétique d'une chaîne      *
 * entrée : str, const string&, la chaîne à transcoder                     *
 * retour : un objet string représentant le transcodage phonétique de str  *
 */
string transcode(const string& str)
{
	// si la string ne fait qu'un caractère, on la renvoie telle quelle. Pas besoin de transcodage
	if (str.size() == 1) return str;
	// on initialise la string trans avec la première lettre du mot telle quelle (sans transcodage)
	string trans {str.at(0)};

	// gestion des exceptions
	try
	{
		// on transcode les caractères restants de str dans trans avec la fonction for_each() de algorithm
		// c'est plus rapide de travailler avec des string_const::iterator plutôt qu'avec une boucle for classique
		const string fin = str.substr(1);
		for_each(fin.cbegin(), fin.cend(), [&](const char& c){ trans.push_back(transcode(c)); });
		return trans;
	}
	// si le caractère est invalide (chiffre ou autre), on en informe l'utilisateur en affichant l'exception capturée
	catch (const string& erreur)
	{
		// affichage de l'erreur sur la sortie d'erreur
		cerr << "\n" << erreur << "\n" << endl;
		// on retourne une chaine vide
		return std::string();
	}
}

/* suppDouble -- supprime les car. redoublés dans une chaîne                      *
 * entrée : str, const string&, la chaîne à transformer                           *
 * retour : un objet string équivalent à str ne contenant plus aucun car. double  *
 */
string suppDouble(const string& str)
{
	// si la chaine fait un caractère ou moins(exception déclenchée par exemple), on la renvoie telle quelle
	if (str.size() <= 1) return str;
	
	string token{str};
	// on élimine les doublons avec unique
	auto it = unique(token.begin(), token.end(), [&](const char& c1, const char& c2){ return c1 == c2;});
	// on resize notre string avec std::distance pour avoir la taille de la string
	token.resize(distance(token.begin(), it));
	// on renvoie la string sans doublons
	return token;
}

/* ajuste -- met une chaîne de car. à une longueur de 4 caractères                *
 * entrée : str, const string&, la chaîne à formater                              *
 * retour : un objet string réduite aux 4 premiers car. de str ou si la longueur  *
 * de str < 4, str complétée avec #                                               *
 */
string ajuste(const string& str, str_size s)
{
	// chaine vide = erreur pendant le transcodage
	if (str.empty()) return str;
	// si la string fait le nombre de caractères fixés ou plus, on ne récupère que les 4 premiers caractères
	else if (str.size() >= s) return str.substr(0, SIZE_KEY);
	// sinon, on complète avec des '#' jusqu'à atteindre le nombre de caractères demandés avec une boucle while
	else
	{
		string fin_mot(str);
		// on complète la chaine avec '#' pour arriver à la taille demandée (SIZE_KEY = 4)
		while (fin_mot.size() < SIZE_KEY) fin_mot.push_back('#');
		// on retourne la chaine complétée avec des '#' pour atteindre le nombre de caractères requis (s)
		return fin_mot;
	}
}

/* suppTouteOcc -- supprime toutes les occurrences d'un car. donné dans une chaîne 		*
 * entrée : str, const string&, la chaîne à transformer                           		*
 * cc, char, le car. à supprimer                                        				*
 * retour : un objet string équivalent à str ne contenant plus aucune occ. de cc  		*
 */
string suppTouteOcc(const string& str, const char& cc)
{
	string unique;
	copy_if(str.cbegin(), str.cend(), back_inserter(unique), [&](const char& c) { return c != cc; });
	return unique;
}

/* cleSoundex -- génération de la clé soundex d'une chaîne          *
 * entrée : mot, const string&, la chaîne à soundexer               *
 * retour : un objet string, la clé soundex de mot                  *
 */
string cleSoundex(const string& mot)
{
	// on génère la clé Soundex du mot
	string preprocess = dediacr_enMin(suppAposTiret(mot));
	string soundex = ajuste(suppTouteOcc(suppDouble(transcode(preprocess)), '0'));
	return soundex;
}

/* impression d'un map<string, list<string> >                        *
 * entrée : table, référence sur le map à imprimer                   *
 * out, le flot de sortie, par défaut cout                           *
 */
void Print(const map<string, list<string>>& all_soundex, ostream& cout)
{
	// on itère sur chaque clé Soundex
	for (const auto& soundex: all_soundex)
	{
		cout << "****************** clef Soundex : " << soundex.first << endl;
		// impression de la liste de mots qui partagent la même clé Soundex
		for (const auto& str: soundex.second)
		{
			cout << '\t' << str << endl;
		}
	}
}

/* impression d'une list<string>                                     *
 * entrée : l, référence sur la liste à imprimer                     *
 * out, le flot de sortie, par défaut cout                           *
 */
void Print(const list<string>& liste, ostream& cout)
{
	// impression de la liste de mots qui partagent la même clé Soundex
	for (auto str: liste)
	{
  		// on affiche chaque mot ayant la meme clé Soundex avec notre cout en assembleur
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
    // on recupere largement assez de mémoire dans la pile
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
