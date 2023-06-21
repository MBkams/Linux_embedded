# TP Linux Embarqué

Ce mini-projet de 12h a pour objectif la prise en main d'un noyau sur une cible reconfigurable. L’objectif final est le développement de driver et d'application multiprocessus sur la carte VEEK-MT2S au fur et à mesure des séances. 

Répartition conseillée des 12 h:
- Prise en main de la cible 
- Compilation et crosscompilation de module sur cible
- Création d'un périphérique et programmtion d'un module avec configuration automatique

### Prerequis
- Carte VEEK-MT2S
- Machine virtuelle fourni par l'ENSEA
- Emulateur de terminal : Putty, Tera Term

## Prise en main

### Préparation de la carte SD
Sous Windows, à l'aide de l'outil Win32DiskImager, on flashe la carte SD avec l'image fournie par Terasic.
Une fois flashé, on insére la carte SD sur la carte VEEK-MT2S afin de communiquer en liaison série.

### Utilisez un logiciel de liaison série

Pour la suite du TP, j'utilise l'émulateur Tera term.
Après s'être connecter en port série et démarrer la carte , on oberve la séquence de démarage de l'os sur le terminal.
On se identifie avec : 
- login : root
- password : aucun (tapre sur la touche Entrée)

Puis, on va agrandir le volume de la carte SD avec la commande suivante : 
***
./expand_rootfs.sh
***
Et une fois rebooté et loggez : 
***
./resize2fs_once
***

Avant l'augmentation du volume :

![Carte_SD_before_volume](https://github.com/MBkams/Linux_embedded/assets/105196572/3d7ac45f-84e6-4e96-86d0-fd810a660543)

Après l'augmentation du volume :

![Carte_SD_volume](https://github.com/MBkams/Linux_embedded/assets/105196572/c10b3e7b-f1df-4f87-b3ec-595b507b8258)

On oberseve que nous avons bien augmenté le volume de la carte à 16GB.



scp src/Linuxroot@192.168.88.95:~/src/Linux_embedded
