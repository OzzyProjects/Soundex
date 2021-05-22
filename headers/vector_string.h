/* ARMANGAUU Etienne 21703622 */

#ifndef VECTOR_STRING
#define VECTOR_STRING

#include <string>
#include <vector>

using std::string;
using namespace std;
using namespace std::string_view_literals;

// alias pour nos pointeurs de fonction
using ptr_str = bool (*)(const string&, const string&);
// typedef sur string::const_iterator
typedef string::const_iterator str_it;
// typedef pour les vector<strong>::const_iterator
typedef vector<string>::const_iterator const_it;

// prototype des fonctions
/* les fonctions compare et egale ont ete remplacées par des fonctions lambdas*/
bool compareSC(const string& , const string& );

bool egaleSC(const string& , const string& );

char normalise_char(char);

string normalise_string(const string&);

#endif