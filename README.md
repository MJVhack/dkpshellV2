Basé sur le projet [dkpshell](https://github.com/MJVhack/dkpshell) en python.

# dkpshell

Dkpshell est un shell intéractif fait pour renouveler les anciens bash et sh. Fait uniquement pour Linux, il n'est pas compatible windows.
Sa première version a été développer en python, pour cette version, c'est le langage C++ qui a été choisi.

# Installation

Dans un premier temps, cloner le projet:
```
git clone -b master https://github.com/MJVhack/dkpshellV2
```
Ensuite, entrez dans le dossier

```
cd dkpshellV2
```

CAS 1:

Rendez executable le script

```
chmod +x scripts/compil.sh
```
Vous executez le script

```
./scripts/compil.sh
```
Et enfin l'executer

```
./dkpshell.out
```

Cas 2:

Rendre executable directement le pré compile

```
chmod +x compile/dkpshell.out
```

Et enfin l'executer

```
./compile/dkpshell.out
```

# Utilisation

Voici son rendu au démarage:

<img width="2621" height="1552" alt="dkpshell_root" src="https://github.com/user-attachments/assets/ffcbe0a7-f6f5-4132-a540-b252aece5b69" />


Le prefixe est le nom donné au mot ici "ICI@EDDKP:/#"
Sans root, le demande du PATH sera ignoré et jamais demander.

# Custom Commande

Voici les custom command disponible:
- dkpconfig
  Qui ne présente uniquement -restartshell
- dkptool
  Qui as jutse AddToPath
  
- dkptheme
  Qui presente uniquement le thème P1 et P2










