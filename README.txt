******************************************************
********************** Couveuse **********************
******************************************************

1) Librairies à télécharger : 
- SHT31 : Télécharger le zip :
		-> https://github.com/Seeed-Studio/Grove_SHT31_Temp_Humi_Sensor
	 et l'ajouter depuis l'IDE Arduino : 
		-> Croquis > Inclure une bibliothèque > Ajouter la bibliothèque .ZIP
- u8x8 : Directement depuis l'IDE Arduino 
		-> Croquis > Inclure une bibliothèque > Gérer les bibliothèques > U8g2 > Installer

2) Compilation & téléversement
- Après avoir fait les branchements comme indiqué sur le schéma fonctionnel, cliquer sur l'icône flèche "Téléverser" : cela lance la compilation, puis le téléversement

3) Intéractions avec le système
- Pour que le brumisteur fonctionne correctement :
	-> Se munir d'une coupelle
	-> Y placer une éponge, un carré de tissu épais ou du papier essuie-tout gorgés d'eau
	-> Y poser le brumisateur à plat : il ne doit pas être en contact direct avec de l'eau sous forme de goutte, seulement à un tissu/papier très fortement mouillé
- Pour changer la valeurs de consigne et température :
	-> Appuyer sur le bouton ?? pour incrémenter de 0.5°C la température
	-> Appuyer sur le bouton ?? pour décrémenter de 0.5°C la température
	-> Appuyer sur le bouton ?? pour incrémenter de 0.5%RH l'humidité
	-> Appuyer sur le bouton ?? pour décrémenter de 0.5%RH l'humidité
	/!\ Les connexions étant parfois capricieuses, il se peut qu'il faille appuyer plusieurs fois sur le bouton avant de voir un changement (visible sur l'écran LCD)


4) Précautions d'emploi & sécurité
- /!\ Tension de 65V entre deux PINs du circuit de management du brumisateur /!\



---------------------------
Chams RIMAN & Yiannis MANZO
---------------------------