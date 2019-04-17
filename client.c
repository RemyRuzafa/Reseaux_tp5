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

	struct sockaddr_in * addr;
	int Sn = h_socket(AF_INET,SOCK_STREAM);
	adr_socket(SERVICE_DEFAUT,SERVEUR_DEFAUT,SOCK_STREAM,&addr);
	h_connect(Sn,addr);
	scanf("%d",&t);
	char tmp[t];
	tmp[0] = t;
	l = t;
	h_writes(Sn,tmp,1);
	int i=0;
	do{
		fini = 1;
		do{
			scanf("%d",&t);
			tmp[i] = t;
		}while(i<l);
  		h_writes(Sn,tmp,l);
		h_reads(Sn,tmp,l);
		for(int i=0;i<l;i++){
			printf("%d",tmp[i]);
			if(tmp[i]!=-2){
				fini = 0;
			}
		}
	}while(!fini);
	printf("Bravo\n");
}

/*****************************************************************************/
