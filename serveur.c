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
	struct sockaddr_in * addr;
	char tmp[256];
	int Sn = h_socket(AF_INET,SOCK_STREAM);
	adr_socket(service,"127.0.0.1",SOCK_STREAM,&addr);
	h_bind(Sn,addr);
	h_listen(Sn,1);
	h_accept(Sn,addr);
	h_reads(Sn,tmp,1);
	int l = tmp[0];
	char grille[l];
	char ncouleurs[8];
	int i;
	int t;
	for(i = 0; i<l; i++){
		t = (rand()%8)+1;
		grille[i] = t;
	}
	char diff;
	do{
		diff = 0;
		h_reads(Sn,tmp,l);
		for(i = 0; i<8; i++){
			ncouleurs[i] = 0;
		}
		for(i = 0; i<l; i++){
			ncouleurs[grille[i]]++;
		}
		for(i = 0; i<l; i++){	//Priorité aux couleurs bien placées
			if(tmp[i]==grille[i]){
				ncouleurs[tmp[i]]--;
				tmp[i] = -2;	// -2 = couleur bien placée
			}
		}
		for(i = 0; i<l; i++){	//Ensuite, on traite les couleurs mal placées ou incorrectes
			if(tmp[i]!=-2){
				if(ncouleurs[tmp[i]]>0){
					ncouleurs[tmp[i]]--;
					tmp[i] = -1;	// -1 = couleur mal placée
					diff = 1;
				}
				else{
					tmp[i] = 0;	// 0 = couleur qui n'a rien à faire là
					diff = 1;
				}
			}
		}
		h_writes(Sn, tmp, l);
	}while(diff);
	h_close (Sn);
}
