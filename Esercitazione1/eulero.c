#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#define g 9.81
#define passo 0.01
struct set{
  double x;
  double v;
  double a;
  double e;
  double m;
  double k; //costante elastica o coefficiente d'ATTRITO
  double tmax;
  double L;
};

double energia_oscillatore(struct set);
double accelerazione_oscillatore(struct set);
double energia_pendolo(struct set);
double accelerazione_pendolo(struct set);
void eulero(struct set,double(*)(struct set),double(*)(struct set),char *); //da modificare i prototipi
void eulerocromer(struct set,double(*)(struct set),double(*)(struct set),char *);
void midpoint(struct set,double(*)(struct set),double(*)(struct set),char *);
void leapfrog(struct set,double(*)(struct set),double(*)(struct set),char *);
double lettura(char *);

int main(){
	struct set variabili;
	int scelta;
	char titolo[80];
	double (*f_accelerazione)(struct set);
	double (*f_energia)(struct set);
	// dichiarare due puntatori a funzioni ed assagnarli a accelerazione_pendolo(o accelerazione_oscillatore) e a energia_oscillaotre (o energia_pendlo)
	do{
	printf("Digitare\n1 problema oscillatore armonico\n2 problema pendolo...\n");
	scanf("%d",&scelta);
	}while(scelta!=1 && scelta!=2);
	if(scelta==1){
		f_accelerazione=&accelerazione_oscillatore;
		f_energia=&energia_oscillatore;
		do{
			printf("Digitare:\n1 per Eulero\n2 per Eulero-Cromer\n3 per Midpoint\n4 per Leapfrog...\n");
			scanf("%d",&scelta);
		} while (scelta!=1 && scelta!=2 && scelta!=3 && scelta!=4);
		do{
			variabili.x=lettura("Inserisci posizione iniziale\n");
			variabili.v=lettura("Inserisci velocita' iniziale\n");
			if (variabili.x==0 && variabili.v==0) printf("I due parametri non possono essere entrambi nulli! Riprova\n");
		}while (variabili.x==0 && variabili.v==0);
		do{
			variabili.m=lettura("Inserisci massa\n");
			if(variabili.m<=0) printf("Il parametro inserito non deve essere minore di zero, riprova\n");
		}while (variabili.m<=0);
		do{
			variabili.k=lettura("Inserisci costante elastica\n");
			if(variabili.k<=0) printf("Il parametro inserito non deve essere minore di zero, riprova\n");
		}while (variabili.k<=0);
		do{
			variabili.tmax=lettura("Inserisci tempo finale di integrazione\n");
			if(variabili.tmax<=0) printf("Il parametro inserito non deve essere minore di zero, riprova\n");
		}while (variabili.tmax<=0);
		printf("La fase fi analiticamente vale %lf\n", -atan(variabili.v/variabili.x/sqrt(variabili.k/variabili.m)));
		printf("L'ampiezza A analiticamente vale %lf\n", variabili.x/cos(-atan(variabili.v/variabili.x/sqrt(variabili.k/variabili.m))));
		printf("Omega vale %lf\n",sqrt(variabili.k/variabili.m));
		if(scelta==1){
			sprintf(titolo,"oscillatore_eulero_(x0=%.3lf_v0=%lf.3).dat", variabili.x, variabili.v);
			eulero(variabili,f_accelerazione,f_energia,titolo);
		}
		if(scelta==2){
			 sprintf(titolo, "oscillatore_eulero-cromer_(x0=%.3lf_v0=%.3lf).dat", variabili.x, variabili.v);
			 eulerocromer(variabili,f_accelerazione,f_energia,titolo);
		}
		if(scelta==3){
			 sprintf(titolo, "oscillatore_midpoint_(x0=%.3lf_v0=%.3lf).dat", variabili.x, variabili.v);
			 midpoint(variabili,f_accelerazione,f_energia,titolo);
		}
		if(scelta==4){
			 sprintf(titolo, "oscillatore_leapfrog_(x0=%.3lf_v0=%.3lf).dat", variabili.x, variabili.v);
			 leapfrog(variabili,f_accelerazione,f_energia,titolo);
		}
	}
	if(scelta==2){
		f_accelerazione=&accelerazione_pendolo;
		f_energia=&energia_pendolo;
		do{
			variabili.x=lettura("Inserisci angolo iniziale\n");
			variabili.v=lettura("Inserisci velocita' iniziale\n");
			if (variabili.x==0 && variabili.v==0) printf("I due parametri non possono essere entrambi nulli! Riprova\n");
		}while (variabili.x==0 && variabili.v==0);
		do{
			variabili.L=lettura("Inserire la lunghezza del pendolo\n");
			if(variabili.L<=0) printf("Inserire un valore strettamente positivo!\n\n");
		} while(variabili.L<=0);
		do{
			variabili.m=lettura("Inserisci massa\n");
			if(variabili.m<=0) printf("Il parametro inserito non deve essere minore di zero, riprova\n");
		}while (variabili.m<=0);
		do{
			variabili.k=lettura("Inseriscicoefficiente d'attrito. Digitare 0 per studiare pendolo senza attrito\n");
			if(variabili.k<0) printf("Il parametro inserito non deve essere minore di zero, riprova\n");
		}while (variabili.k<0);
		do{
			variabili.tmax=lettura("Inserisci tempo finale di integrazione\n");
			if(variabili.tmax<=0) printf("Il parametro inserito non deve essere minore di zero, riprova\n");
		}while (variabili.tmax<=0);
		printf("Il periodo del sistema analiticamente vale %.10lf\n\n",2*M_PI/sqrt(g)*sqrt(variabili.L));
		do{
			printf("Digitare:\n1 per Eulero\n2 per Eulero-Cromer\n3 per Midpoint\n4 per Leapfrog...\n");
			scanf("%d",&scelta);
		} while (scelta!=1 && scelta!=2 && scelta!=3 && scelta!=4);
		if(scelta==1){
			 sprintf(titolo, "pendolo_eulero(x0=%.3lf_v0=%.3lf_gamma=%.3lf).dat", variabili.x, variabili.v, variabili.k);
			 eulero(variabili,f_accelerazione,f_energia,titolo);
		}
		if(scelta==2){
			sprintf(titolo, "pendolo_eulero-cromer(x0=%.3lf_v0=%.3lf_gamma=%.3lf).dat", variabili.x, variabili.v, variabili.k);
			eulerocromer(variabili,f_accelerazione,f_energia,titolo);
		}
		if(scelta==3){
			sprintf(titolo, "pendolo_midpoint(x0=%.3lf_v0=%.3lf_gamma=%.3lf).dat", variabili.x, variabili.v, variabili.k);
			midpoint(variabili,f_accelerazione,f_energia,titolo);
		}
		if(scelta==4){
			 sprintf(titolo, "pendolo_leapfrog(x0=%.3lf_v0=%.3lf_gamma=%.3lf).dat", variabili.x, variabili.v, variabili.k);
			 leapfrog(variabili,f_accelerazione,f_energia,titolo);
		}
	}
}
double lettura(char *message){
  double x;
  printf("%s",message);
  scanf("%lf",&x);
  return x;
}
double energia_oscillatore(struct set parametri){
  return parametri.m*pow(parametri.v,2)/2+parametri.k*pow(parametri.x,2)/2; 
}
double accelerazione_oscillatore(struct set parametri ){
  return -parametri.k/parametri.m*parametri.x;
}
double accelerazione_pendolo(struct set parametri){
	return -sin(parametri.x)*g/parametri.L-parametri.k*parametri.v;
}
double energia_pendolo(struct set parametri){
	return parametri.m*parametri.L*(parametri.L/2*pow(parametri.v,2)+g*(1-cos(parametri.x)));
}
void eulero(struct set parametri,double (*accelerazione)(struct set),double (*energia)(struct set),char *titolo){ //calcola il periodo
  int i=0,j=1;
  double E0,temp,prima;
  double *t;
  char titolo2[100];
  FILE *p,*p2;
  t=malloc(1*sizeof(double));
  strcpy(titolo2,"PERIODO");
  p=fopen(titolo, "w");
  strcat(titolo2,titolo);
  p2=fopen(titolo2,"w");
  E0=energia(parametri);
  while (i*passo<parametri.tmax){
    fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*passo, parametri.x, parametri.v, (energia(parametri)-E0)/E0);
    prima=parametri.x; //la posizione prima del passo
    temp=accelerazione(parametri);
    parametri.x=parametri.x+parametri.v*passo;
    parametri.v=parametri.v+temp*passo;
    if(prima*parametri.x<0){
		t=(double *)realloc(t,(j+1)*sizeof(double));
		*(t+j-1)=(i*passo+(i-1)*passo)/2;
		if(j>2) fprintf(p2,"%.10lf\n",*(t+j-1)-(*(t+j-3)));
		j++;
	}
    i++;
  }
  fclose(p);
  fclose(p2);
}
void eulerocromer(struct set parametri,double (*accelerazione)(struct set),double (*energia)(struct set),char *titolo){
    int i=0;
  double E0;
  FILE *p;
  p=fopen(titolo, "w");
  E0=energia(parametri);
  while (i*passo<parametri.tmax) {
    fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*passo, parametri.x, parametri.v, (energia(parametri)-E0)/E0);
    parametri.v=parametri.v+accelerazione(parametri)*passo;
    parametri.x=parametri.x+parametri.v*passo;
    i++;
  }
  fclose(p);
}
void leapfrog(struct set parametri,double (*accelerazione)(struct set),double (*energia)(struct set),char *titolo){
  int i=0;
  double E0,temp,temp2;
  FILE *p;
  p=fopen(titolo, "w");
  E0=energia(parametri);
  temp=parametri.v+accelerazione(parametri)*passo/2;
  while (i*passo<parametri.tmax) {
    fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*passo,parametri.x, parametri.v, (energia(parametri)-E0)/E0);
    temp2=temp+accelerazione(parametri)*passo;
    parametri.x=parametri.x+temp2*passo;
    parametri.v=(temp2+temp)/2;
    temp=temp2;
    i++;
  }
  fclose(p);
}
void midpoint(struct set parametri,double (*accelerazione)(struct set),double (*energia)(struct set),char *titolo){
  int i=0;
  double E0, temp;
  FILE *p;
  p=fopen(titolo, "w");
  E0=energia(parametri);
  temp=parametri.v;
  while (i*passo<parametri.tmax) {
    fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*passo, parametri.x, parametri.v, (energia(parametri)-E0)/E0);
    parametri.v=parametri.v+accelerazione(parametri)*passo;
    parametri.x=parametri.x+(parametri.v+temp)/2*passo;
    temp=parametri.v;
    i++;
  }
  fclose(p);
}
