/* ARMANGAUU Etienne 21703622 */

// d�finitions de sous-programmes repr�sentant des op�rations sur le type string
// ces sous-programmes sont d�clar�s dans le fichier m_string.h

#include "m_string.h"

/* normalisePonct -- normalise une cha�ne en ajoutant un blanc devant les signes de ponctuation      *
 *                   et un blanc devant l'apostrophe                                                 *
 * donn�e: s, const string&, la cha�ne � normaliser                                                  *
 * retour: une cha�ne, string, contenant s o� un blanc est ajout� devant chaque signe de ponctuation *
 */
string normalisePonct(const string& s)
{
    // const string ponct(".,;:?!��\"'"); d�clar� dans classecar.cpp et d�fini dans classecar.h

     string norms(s);
     str_size l = s.size(),
              itrouve = 0;
     while(true)
     {
          if ( itrouve >= l ) break;
          itrouve = norms.find_first_of(ponct, itrouve);
          
          if ( itrouve == string::npos ) break;
                              
          if ( norms[itrouve] == '\'' ) // apostrophe
          {
               if ( norms.substr(itrouve+1, 3) != "hui" )
               {
                    norms.insert(itrouve+1, 1, ' ');
               }
          }
          else                          // tous les autes
          {
               norms.insert(itrouve, 1, ' ');
          }
          itrouve+=2;

          l++;
     }
     return norms;

}// normalise_ponct

/* supPonc -- suppression d'un signe de ponctuation � la fin d'une cha�ne *
 *  donn�e : m, sting &, une r�f�rence sur une cha�ne de caract�re        *
 *  r�sultat : m, la cha�ne r�f�renc�e nettoy�e si elle se terminait par  *
 *             un signe de ponctuation, sinon la cha�ne telle quelle      *
 */
void supPonct(string &m)
{
     string::size_type idcar = m.size()-1;
     if (estPonc(m[idcar]))
     {
          m.erase(idcar,1);
     }

}// supPonct


/* split -- segmente une cha�ne de caract�res en mots graphiques *
 *  donn�e : s, string, la cha�ne � segmenter                    *
 *  retour : la liste des mots graphiques de s, vector<string>   *
 */
vector<string> split(const string& s)
{ // version avec it�rarteur
     vector<string> listeMots;
     
     string::const_iterator it = s.begin();
     
     // tant qu'on n'a pas parcouru toute la string
     while(it != s.end())
     {
          // ignorer les s�quences de caract�res blancs et trouver le 1er caract�re d'un mot
          // utilisation d'une expression lambda qui v�rifie si un caract�re donn� n'est pas ou est un s�parateur
          it = find_if(it, s.end(), [](const char& c){return !(isspace(c));});
                    
          // trouver la fin du prochain mot, c'est-�-dire le dernier caract�re d'un mot
          // utilisation d'une expression lambda qui v�rifie si un caract�re donn� est oui ou non un s�parateur
          string::const_iterator j = find_if(it, s.end(), [](const char& c){return isspace(c);});
          
          // un mot trouv�
          if ( it != s.end() )
          {
               listeMots.push_back(string(it, j)); // string(it, j) cr�e un objet anonyme 
                                                  // construit par copy de la sous-cha�ne de s[it, j[
          }
          it = j;          
     }

     return listeMots;
}// split


/* printVstr -- imprime un vecteur de strings                                *
 *   donn�e: vstring, vector<string>, le vecteur � imprimer                  *
 *           out, ostream&, le flot de sortie, par d�faut le flot std cout   *
 */
void printVstr(const vector<string>& vstring, ostream& out )
{
     if ( vstring.empty() )
     {
          cout << "\t(vecteur vide)" << endl;
     }
     for(auto s : vstring )
     {
          if ( s.empty() )
          {
              cout << "\t(chaine vide)" << endl; 
          }
          out << s << endl;
     }
}// vstring  -- avec param�trage du flot de sortie, par d�faut le flot std vstring  -- avec param�trage du flot de sortie, par d�faut le flot std

