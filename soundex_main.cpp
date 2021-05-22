/* ARMANGAU Étienne 21703622                                                                        *
 * Soundex génère les clés Soundex pour chacun des mots du dictionnaire indiqué en argument         *
 * Il classe les mots en fonction de leur clé Soundex et permet à l'utilisateur de chercher         *
 * un mot avec sa clé Soundex qui s'affiche et qui affiche tous les autres mots ayant la même       *
 * clé Soundex que le mot recherché                                                                 *
 * Par ailleurs, il gère les exceptions si un mot entré contient des caractères non valides         *
 * comme des symboles ou des chiffres et en avertit l'utilisateur                                   *
 * argv[1] = chemin du dictionnaire à soundexer                                                     *
 * sortie standard, écran                                                                           *
 * Je tiens à dire que tout ce code a été sur Vim et que c'est long au début 					    *
*/

#include <fstream>
#include "m_string.h"
#include "classecar.h"
#include "vector_string.h"
#include "soundex.h"

int main(int argc, const char* argv[])
{

	// un argument obligatoire en ligne de commande sinon on met fin au programme
	if ( argc != 2)
	{
		cout << "***** Erreur " << endl;
		cout << "\t utilisation : " << argv[0] << " nom du dictionnaire " << endl;
		return 10;
	}

	// ouverture d'un flot d'entrée sur le dictionnaire
	ifstream dic_in(argv[1]);

	if ( !dic_in )
	{
		cout << "***** Erreur " << endl;
		cout << '\t' << argv[1] << " ne peut pas être ouvert" << endl;
		return 20;
	}

	// soundexisation du dictionnaire
	map<string, list<string> > dico;
	while(true)
	{
		string lex;
		dic_in >> lex;
		if ( dic_in.eof() ) break;
		if ( lex.empty() ){ continue; }
		string cle = cleSoundex(token(lex));
		dico[cle].push_back(lex);
	}

	dic_in.close();
	Print(dico);

	// Recherche d'un mot dans le dictionnaire
	while(true)
	{

		string mot_ch;
		cout << "\nQuel est le mot a chercher ? " ;
		cin >> mot_ch;
		if (mot_ch.empty())
		{
			cout << "Rien à chercher" << endl;
			continue;
		}

		string cle = cleSoundex(mot_ch);
		// si la clé n'est pas vide,  les caractères saisies sont valides donc on poursuit (= pas d'exception déclenchée)
		// sinon, on passe à la recherche suivante
		if (!cle.empty())
		{
			map<string, list<string>>::const_iterator mtrouve = dico.find(cle);
			cout << "\n\n--- mot \t" << mot_ch << '\t' << " (clef : " << cle << ')' << "\n";

			if ( mtrouve == dico.end() )
			{
				cout << "Rien trouvé" << endl;
			}
			else
			{
				// affiche la liste des mots associés à cette clé Soundex
				Print(mtrouve->second);
			}
		}

		// continuation
		char rep;
		cin.ignore(256, '\n');
		cout << "Recherche d'un autre mot ? oui (n'importe quel touche) - non (N/n) ";
		cin.get(rep);
		if (tolower(rep) == 'n' ) break;

	}

	// message de remerciements et de bonne continuation
	const char* message = "\n  Bonne retraite Madame ! :) \n\n";
	cout_asm(message);

	// fin du programme en assembleur
	asm
	(    
    	"movl $1,%eax;"
        "xorl %ebx,%ebx;"
        "int  $0x80"
    );

}// main
