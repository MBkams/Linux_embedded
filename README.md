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

### Configuration réseau

Après avoir branché la carte VEEK sur le switch, on vérifie si on dispose d'une adresse IP.
![old_ip_adress](https://github.com/MBkams/Linux_embedded/assets/105196572/e4caecc6-9e7f-4fd8-9dd9-b9a2f9d76344)

Puis, on va attribuer automatiquement une nouvelle adresse IP en éditant le fichier /etc/network/interfaces avec les lignes suivantes : 

***
source-directory /etc/network/interfaces.d  

auto eth0  
iface eth0 inet dhcp  
allow-hotplug eth0  
***

On reboot la machine et une nouvelle adreese IP sera attribué.  

![ip_adress](https://github.com/MBkams/Linux_embedded/assets/105196572/9e64eb3c-fd2b-4235-91d3-f3b7d02758e2)

On se connecte en ssh par la suite :

![Connexion_via_ssh](https://github.com/MBkams/Linux_embedded/assets/105196572/70b5cc84-6579-48f9-b8d2-b2a3d964a0fb)

Pour établir la connection, on oublie pas de connecter l'ordinateur dans le même sous-réseau wifi.

### Découverte de la cible

#### Exploration des dossiers /sys/class et /proc

On oberve les informations du CPU.
![cpu_info](https://github.com/MBkams/Linux_embedded/assets/105196572/f8b74caf-0a58-47b4-9ea7-aa6f6d856708)

Et on retrouve l'aborlescence des périphériques et leurs adresses.
![file_sys_class](https://github.com/MBkams/Linux_embedded/assets/105196572/91930f66-f96c-4ce4-b8f3-d0a2c971c497)

![sopc](https://github.com/MBkams/Linux_embedded/assets/105196572/0db3c50b-a30d-40b0-a7b4-57442e9ccf09)

#### Hello World !

On va réaliser un programme Hello World, compiler sur VM et executer sur la carte.

https://github.com/MBkams/Linux_embedded/blame/2b75e9046c46ef3dac6879930a789f455f7b30f3/hello.c#L1

![Hello_World](https://github.com/MBkams/Linux_embedded/assets/105196572/3179e6f6-7c72-4e53-9cb2-2116b468d9e8)

Le programme s'est bien executé.  

#### Chenillard (Et oui, encore !)

On va accéder au driver afin d'allumer les leds. Il suffit d'écrire un '1' dans le bon fichier.
Par exmemple,

***
echo "1" > /sys/class/leds/fpga_led1/brightness
***

On écrit par la suite un programme afin de réaliser un jeu de lumière.  

https://github.com/MBkams/Linux_embedded/blame/a69ce25d7511a97f11050c5d4a0a2b85ebc9488f/chenillard.c#L1

Après exécution du fichier object sur la carte :  
![Chenillard](https://github.com/MBkams/Linux_embedded/assets/105196572/1824e710-aa77-4a34-b9b6-081154fd1485)

On oberve un chenillard comme décrit dans le programme.

## Modules kernel

### Compilation de module noyau sur la VM

Pour ce TP, nous allons développer nos propres modules noyau. Nous allez avoir besoin des sources du noyau cible (en fait en théorie il faut seulement les includes). Il nous faut les sources exactes du noyau sur lequel le module va être
chargé. 

À partir du Makefile et des fichiers .c, on va compiler les modules suivants avec les commandes modinfo, lsmod, insmod et rmmod.

- Utilisation de paramètres au chargement du module  
https://github.com/MBkams/Linux_embedded/blame/d270774954c66fd99242e95470f95a3a4ab4a8a9/Module/hello.c#L1

- Création d’un entrée dans /proc  
https://github.com/MBkams/Linux_embedded/blame/d270774954c66fd99242e95470f95a3a4ab4a8a9/Module/proc.c#L1

![module_proc](https://github.com/MBkams/Linux_embedded/assets/105196572/60bf5aad-390d-4a65-aabe-2b51fa4220a9)


- Utilisation d’un timer

https://github.com/MBkams/Linux_embedded/blob/2c512b4ba51c60fe7b31a8d704a69c125a9b8f3a/Module/timer.c#L1C1-L45C1
 
![module_timer](https://github.com/MBkams/Linux_embedded/assets/105196572/8109fccc-a2d3-4a7a-b3c2-68d930274d53)

### CrossCompilation de modules noyau

À cause de la puissance limitée du processeur de la carte cible, la compilation, en particulier la compilation de modules noyau, est relativement longue. Nous allons donc, une fois encore, cross-compiler les modules noyau pour la carte SoC, à l’aide de la VM.

Nous allons recompiler les fichiers modules sur la VM et executé sur la carte.

#### Récupéreation de la configuration actuelle du noyau

- Les lignes commençant par "export" sont utilisées pour définir des variables d'environnement
- Le chemin qui termine par un tiret "-" est utilisé pour indiquer à make qu'il doit rechercher les outils de compilation dans les répertoires du PATH

On observe que les modules ont bien "cross-compilés" sur la carte.

#### Chenillard (Yes !)

Début d'implémentation

https://github.com/MBkams/Linux_embedded/blame/ab80039854fdb4dd1c786d5754555df4427999fd/Module/led_timer.c#L1


# Conclusion

Grâce à ce mini-projet, j'ai pu  approfondir mes connaissances en noyau linux sur une cible embarqué plus généralement. 
Bien que l'implémentation des premiers modules donne des résultats plutôt satisfaisants indépendamment, il reste un certain sentiment de regret quant à l’impossibilité de mener véritablement ce projet à son terme par moi-même. Notamment sur la création de son propore module et périphérique.
