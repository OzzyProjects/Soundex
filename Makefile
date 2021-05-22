# pour générer l’exécutable, faites la commande sudo make dans le répertoire de votre projet/makefile.
# il est peut-être censé fonctionner sur Mac mais ce n'est pas certain.

# sur Debian et Ubuntu, la librairie libcurl doit être installée :
# dans votre terminal :

# sudo apt install libcurl4-openssl-dev

# VARIABLES A MODIFIER SELON VOTRE ENVIRONEMENT ET VOS GOUTS

# répertoire des fichiers sources (*.cpp)
SRC  := sources
# répertoire des fichiers headers (*.h)
INC  := headers
# nom du fichier main.cpp (à modifier absolument)
MAIN := soundex_main.cpp
# nom de l’exécutable qui va être généré (à modifier)
EXEC := soundex

# liste des fichiers sources y compris le main.cpp qui peut être dans le répertoire des sources ou dans
# le répertoire courant parent du projet
ifneq ("$(wildcard $(MAIN))","")
	sources := $(MAIN) $(wildcard $(SRC)/*.cpp)
else
	sources := $(wildcard $(SRC)/*.cpp)
endif
# liste des fichiers objets
objects := $(sources:.cpp=.o)
# liste des fichier de dépendance
deps    := $(objects:.o=.d)

# choix du compilateur et de l’éditeur de liens
CXX := g++
# options du compilateur : choix de recevoir des informations complètes sur les warnings
CXXFLAGS := -I $(INC) -MMD -MP -g -std=c++17 -pedantic -Wall

# nom de l'OS
UNAME := $(shell uname -s)

ifeq ($(UNAME), Linux)
	# options personnelles pour mon linker sur Debian
	LDFLAGS  := -L/usr/lib/x86_64-linux-gnu -no-pie
	LDLIBS   := -lcurl
else
	# si c'est Darwin (MAC), on effectue quelques modifications
	CXXFLAGS += -D OSX
	LDFLAGS  := -stdlib=libstdc++
endif

# édition de liens
$(EXEC) : $(objects)
	@echo "Generation de l'executable..."
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(RM) $(objects) $(deps)
	@echo "Generation réussie !"

# compilation des fichiers sources
$(SRC)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

# routine pour supprimer l’exécutable
.PHONY: clean
clean:
	$(RM) $(EXEC)

# pour une desinstallation des fichiers obj et dep si une compilation s'est mal passée
.PHONY: allclean
allclean:
	$(RM) $(objects) $(deps)

# on prend en compte les dépendances entre les fichiers
-include $(deps)
