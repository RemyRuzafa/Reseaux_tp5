/******************************************************************************/
/*			Application: ...					*/
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : ... 					*/
/*									      */
/******************************************************************************/


#include <stdio.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include<stdlib.h>

#include "fon.h"   		/* primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"
#define SERVEUR_DEFAUT "127.0.0.1"

void client_appli (char *serveur, char *service);


/*****************************************************************************/
/*--------------- programme client -----------------------*/

int main(int argc, char *argv[])
{

	char *serveur= SERVEUR_DEFAUT; /* serveur par defaut */
	char *service= SERVICE_DEFAUT; /* numero de service par defaut (no de port) */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch(argc)
	{
 	case 1 :		/* arguments par defaut */
		  printf("serveur par defaut: %s\n",serveur);
		  printf("service par defaut: %s\n",service);
		  break;
  	case 2 :		/* serveur renseigne  */
		  serveur=argv[1];
		  printf("service par defaut: %s\n",service);
		  break;
  	case 3 :		/* serveur, service renseignes */
		  serveur=argv[1];
		  service=argv[2];
		  break;
    default:
		  printf("Usage:client serveur(nom ou @IP)  service (nom ou port) \n");
		  exit(1);
	}

	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */

	client_appli(serveur,service);
}

/* procedure correspondant au traitement du client de votre application */
void client_appli (char *serveur,char *service){
	int t;
	int l;
	int fini;
	struct sockaddr_in * addrC;

	int SIDC = h_socket(AF_INET,SOCK_STREAM);	//On crée un socket TCP

	adr_socket(SERVICE_DEFAUT,SERVEUR_DEFAUT,SOCK_STREAM,&addrC);

	h_connect(SIDC,addrC);	//On se connecte au serveur via ce socket

	printf("Connexion effectuée\nNiveau : ");
	scanf("%d",&t);	//Lecture du niveau (nombre d'emplacements de couleurs à trouver)

	char tmp[t];	//Tampon qui sert à envoyer et recevoir les données
	tmp[0] = t;
	l = t;
	h_writes(SIDC,tmp,1);	//On envoie la difficulté lue plus haut au serveur
	int i=0;

	int bp, mp, in;
	printf("Les couleurs sont représentées par les entiers de 1 à %d\nLes résultats : 2 = bien placée, 1 = mal placée, 0 = incorrecte\n",l);
	do{
		fini = 1;
		printf("\nProposition :\n");
		for(i=0;i<l;i++){
			scanf("%d",&t);	//On lit la proposition de l'utilisateur
			tmp[i] = t;
		}
  		h_writes(SIDC,tmp,l);	//On transmet cette proposition au serveur
		h_reads(SIDC,tmp,l);	//On lit sa réponse
		bp = 0;
		mp = 0;
		for(int i=0;i<l;i++){	//Cette boucle calcule le nombre de couleurs justes et le nombre de couleurs mal placées
					//(Pour les raisons évoquées dans le compte rendu)
			switch(tmp[i]){
				case(-2):
					bp++;
					break;
				case(-1):
					mp++;
					fini = 0;
					break;
				case(0):
					fini = 0;
					break;
				default:
					printf("ERREUR");
					return;
			}
		}
		printf("\n\n%d bien placées\n%d mal placées\n",bp,mp);	//On affiche ces nombres
	}while(!fini);	//Et on recommence tant que l'utilisateur n'a pas donné la bonne réponse
	printf("\n\nBravo\n\n\n");
	h_close(SIDC);	//On ferme la connexion
}
