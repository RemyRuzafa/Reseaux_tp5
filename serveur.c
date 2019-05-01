/******************************************************************************/
/*			Application: ....			              */
/******************************************************************************/
/*									      */
/*			 programme  SERVEUR 				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs :  ....						      */
/*		Date :  ....						      */
/*									      */
/******************************************************************************/

#include <stdio.h>


#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

#include "fon.h"     		/* Primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"

void serveur_appli (char *service);   /* programme serveur */

/*---------------- programme serveur ------------------------------*/

int main(int argc,char *argv[]){
	char *service= SERVICE_DEFAUT; /* numero de service par defaut */

	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc)
 	{
   	case 1:
		  printf("defaut service = %s\n", service);
		  break;
 		case 2:
		  service=argv[1];
      break;
   	default :
		  printf("Usage:serveur service (nom ou port) \n");
		  exit(1);
 	}

	/* service est le service (ou numero de port) auquel sera affecte
	ce serveur*/

	serveur_appli(service);
}

/* Procedure correspondant au traitemnt du serveur de votre application */
void serveur_appli(char *service){
	srand(time(NULL));
	struct sockaddr_in * addrS;
	char tmp[256];

	int SIDS = h_socket(AF_INET,SOCK_STREAM);	//On crée un socket TCP

	adr_socket(service,NULL,SOCK_STREAM,&addrS);

	h_bind(SIDS,addrS);	//On bind ce socket

	h_listen(SIDS,5);	//On attend qu'un client se connecte

	struct sockaddr_in addrC;
	int SIDC = h_accept(SIDS,&addrC);	//On accepte la connexion

	h_reads(SIDC,tmp,1);	//On lit la difficulté voulue par le client
	int l = tmp[0];
	char grille[l];
	char ncouleurs[8];
	int i;
	int t;
	printf("Niveau reçu : %d\nGénération de la grille\n",l);
	for(i = 0; i<l; i++){	//On génère la grille
		t = (rand()%8)+1;
		grille[i] = t;
		printf("%d ",t);
	}
	printf("\n");
	char diff;
	do{
		diff = 0;
		printf("Attente d'une proposition\n");
		h_reads(SIDC,tmp,l);	//On attend que le client propose quelque chose
		for(i = 0; i<8; i++){
			ncouleurs[i] = 0;
		}
		for(i = 0; i<l; i++){
			ncouleurs[grille[i]-1]++;
		}
		for(i = 0; i<l; i++){	//Priorité aux couleurs bien placées
			if(tmp[i]==grille[i]){
				ncouleurs[tmp[i]-1]--;
				tmp[i] = -2;	// -2 = couleur bien placée
			}
		}
		for(i = 0; i<l; i++){	//Ensuite, on traite les couleurs mal placées ou incorrectes
			if(tmp[i]!=-2){
				if(ncouleurs[tmp[i]-1]>0){
					ncouleurs[tmp[i]-1]--;
					tmp[i] = -1;	// -1 = couleur mal placée
				}
				else{
					tmp[i] = 0;	// 0 = couleur qui n'a rien à faire là
				}
				diff = 1;
			}
		}
		printf("Envoi de la réponse\n");
		h_writes(SIDC, tmp, l);	//On envoie la réponse au client
	}while(diff);	//On recommence tant que le client n'a pas envoyé la bonne proposition
	h_close(SIDC);	//On ferme la connexion
	h_close(SIDS);
}
