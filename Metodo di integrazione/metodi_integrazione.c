#include <stdio.h>
#include <math.h>
#define K 0.3
#define m 1.5
#define x0 0.0
#define v0 2.2
#define passo 0.02

//	F=-Kx --> a=-K/m*x
struct SpacePhase{
    double x;
    double v;
    double E;
    double a;
};
struct Puntatori{
    FILE *fp1;
    FILE *fp2;
    FILE *fp3;
};
typedef struct SpacePhase Parametri;
typedef struct Puntatori Liste;
double energia(double,double);
void eulero(Parametri,Liste,double);
void eulerocromer(Parametri,Liste,double);
void puntocentrale(Parametri,Liste,double);
void mezzopasso(Parametri,Liste,double);
void verlet(Parametri,Liste,double);
void stampa(Parametri,Liste,double,double);
void chiusura(Liste);
int main(){
	int scelta=0;
	Parametri spazio;
	Liste data;
	double E0;
	do{
        printf("Premere:\n1 per integrazione Eulero\n2per integrazione Eulero-Cromer\n3per integrazione Punto Centrale\n4 per Mezzo Passo\n5 per Verlet autosufficiente");
        scanf("%d",&scelta);
    } while (scelta!=1 && scelta!=2 && scelta!=3 && scelta !=4 && scelta!=5);
	spazio.x=x0;
	spazio.v=v0;
	E0=energia(spazio.x,spazio.v);
	if(scelta==1) eulero(spazio,data,E0);
	if(scelta==2) eulerocromer(spazio,data,E0);
	if(scelta==3) puntocentrale(spazio,data,E0);
	if(scelta==4) mezzopasso(spazio,data,E0);
	if(scelta==5) verlet(spazio,data,E0);
}
double energia(double x,double v){
	return m*v*v/2+K*x*x/2;
}
void puntocentrale(Parametri spazio,Liste data,double E0){
	int i=0;
	double intervallo;
	data.fp1=fopen("Legge oraria moto armonico(PC).dat","w");
	data.fp2=fopen("Dispersione energia vs t(PC).dat","w");
	data.fp3=fopen("Posizione vs Velocita'(PC).dat","w");
	printf("\t\tMETODO PUNTO CENTRALE\n\nInserire lunghezza intervallo integrazione:\t");
	scanf("%lf",&intervallo);
	fprintf(data.fp1,"0\t%lf\n",x0);
	while(i*passo!=intervallo){
		spazio.a=-K/m*spazio.x;
		spazio.x=spazio.x+spazio.v*passo+pow(spazio.a,2)*pow(passo,2)/2;
		spazio.v=spazio.v+spazio.a*passo;
		i++;
        stampa(spazio,data,i*passo,E0);
	}
	chiusura(data);
}
void eulero(Parametri spazio,Liste data,double E0){
	int i=0;
	double intervallo;
	data.fp1=fopen("Legge oraria moto armonico(E).dat","w");
	data.fp2=fopen("Dispersione energia vs t(E).dat","w");
	data.fp3=fopen("Posizione vs Velocita'(E).dat","w");
	printf("\t\tMETODO EULERO\n\nInserire lunghezza intervallo integrazione:\t");
	scanf("%lf",&intervallo);
	fprintf(data.fp1,"0\t%lf\n",x0);
	while(i*passo!=intervallo){
		spazio.a=-K/m*spazio.x;
		spazio.x=spazio.x+spazio.v*passo; // aggiorni prima posizione
		spazio.v=spazio.v+spazio.a*passo;
		i++;
		stampa (spazio,data,i*passo,E0);
	}
	chiusura(data);
}
void eulerocromer(Parametri spazio,Liste data,double E0){
	int i=0;
	double intervallo;
	data.fp1=fopen("Legge oraria moto armonico(E-C).dat","w");
	data.fp2=fopen("Dispersione energia vs t(E-C).dat","w");
	data.fp3=fopen("Posizione vs Velocita'(E-C).dat","w");
	printf("\t\tMETODO EULERO-CROMER\n\nInserire lunghezza intervallo integrazione:\t");
	scanf("%lf",&intervallo);
	fprintf(data.fp1,"0\t%lf\n",x0);
	while(i*passo!=intervallo){
		spazio.a=-K/m*spazio.x;
		spazio.v=spazio.v+spazio.a*passo; //prima aggiorna velocit'
		spazio.x=spazio.x+spazio.v*passo;
		i++;
    stampa (spazio,data,i*passo,E0);
	}
	chiusura(data);
}
void mezzopasso(Parametri spazio,Liste data,double E0){

	int i=0;
	double intervallo;
	data.fp1=fopen("Legge oraria moto armonico(MP).dat","w");
	data.fp2=fopen("Dispersione energia vs t(MP).dat","w");
	data.fp3=fopen("Posizione vs Velocita'(MP).dat","w");
	printf("\t\tMETODO MEZZO PASSO\n\nInserire lunghezza intervallo integrazione:\t");
	scanf("%lf",&intervallo);
	fprintf(data.fp1,"0\t%lf\n",x0);
	while(i*passo!=intervallo){
		spazio.a=-K/m*spazio.x;
		if(i==0)spazio.v=v0+spazio.a*passo/2; // per il primo passo si determina la velocita nel punto di mezzo del primo intervallo
		else spazio.v=spazio.v+spazio.a*passo; //prima aggiorna velocit'
		spazio.x=spazio.x+spazio.v*passo;
		i++;
		stampa(spazio,data,i*passo,E0);
	}
	chiusura(data);
}
void verlet(Parametri spazio,Liste data,double E0){

	int i=0;
	double intervallo,temp;
	data.fp1=fopen("Legge oraria moto armonico(V).dat","w");
	data.fp2=fopen("Dispersione energia vs t(V).dat","w");
	data.fp3=fopen("Posizione vs Velocita'(V).dat","w");
	printf("\t\tMETODO VERLET\n\nInserire lunghezza intervallo integrazione:\t");
	scanf("%lf",&intervallo);
	fprintf(data.fp1,"0\t%lf\n",x0);
	while(i*passo!=intervallo){
		spazio.a=-K/m*spazio.x;
		spazio.x=spazio.x+spazio.v*passo+spazio.a*pow(passo,2)/2;
		temp=-K/m*spazio.x;
		spazio.v=spazio.v+(temp+spazio.a)*passo/2;
		i++;
		stampa(spazio,data,i*passo,E0);
	}
	chiusura(data);
}
void stampa (Parametri spazio, Liste data,double t,double E0){
		fprintf(data.fp1,"%lf\t%lf\n",t,spazio.x);
		fprintf(data.fp2,"%lf\t%lf\n",t,(energia(spazio.x,spazio.v)-E0)/E0);
		fprintf(data.fp3,"%lf\t%lf\n",spazio.v,spazio.x);
}
void chiusura(Liste data){
    fclose(data.fp1);
	fclose(data.fp2);
	fclose(data.fp3);
}
