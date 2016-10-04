#include<stdio.h>
#include<math.h>
#define passo 0.01
struct set{
  double x;
  double v;
  double a;
  double e;
  double m;
  double k;
  double tmax;
  double fi;
  double ampiezza;
};

double energia(struct set);
double accelerazione(double, struct set);
void eulero(struct set);
double lettura(char *);

int main(){
  struct set variabili;
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
  eulero(variabili);
  return 0;
}

double energia(struct set parametri){
  return parametri.m*pow(parametri.v,2)/2+parametri.k*pow(parametri.x,2)/2; 
}

double accelerazione(double temp, struct set parametri ){
  return -parametri.k/parametri.m*temp;
}

void eulero(struct set parametri){
  int i=0;
  double E0,temp;
  FILE *p;
  p=fopen("dati.dat", "w");
  E0=energia(parametri);
  while (i*passo<parametri.tmax) {
    fprintf(p, "%lf\t %lf\t %lf\t %lf\t\n", i*passo, parametri.x, parametri.v, (energia(parametri)-E0)/E0);
    temp=parametri.x;
    parametri.x=parametri.x+parametri.v*passo;
    parametri.v=parametri.v+accelerazione(temp,parametri)*passo;
    i++;
  }
  fclose(p);
}
double lettura(char *message){
  double x;
  printf("%s",message);
  scanf("%lf",&x);
  return x;
}
