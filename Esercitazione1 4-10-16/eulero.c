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
void eulerocromer(struct set);
void midpoint(struct set);
void leapfrog(struct set);
double lettura(char *);

int main(){
  struct set variabili;
  int scelta;
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
  if(scelta==1) eulero(variabili);
  if(scelta==2) eulerocromer(variabili);
  if(scelta==3) midpoint(variabili);
  if(scelta==4) leapfrog(variabili);
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
  char titolo[100];
  FILE *p;
  sprintf(titolo, "oscillatorearmonico_eulero_x0=%.2lf_v0=%.2lf_massa=%.2lf.dat", parametri.x, parametri.v, parametri.m);
  p=fopen(titolo, "w");
  E0=energia(parametri);
  while (i*passo<parametri.tmax) {
    fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*passo, parametri.x, parametri.v, (energia(parametri)-E0)/E0);
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
void eulerocromer(struct set parametri){
    int i=0;
  double E0;
  char titolo[100];
  FILE *p;
  sprintf(titolo, "oscillatorearmonico_eulerocromer_x0=%.2lf_v0=%.2lf_massa=%.2lf.dat", parametri.x, parametri.v, parametri.m);
  p=fopen(titolo, "w");
  E0=energia(parametri);
  while (i*passo<parametri.tmax) {
    fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*passo, parametri.x, parametri.v, (energia(parametri)-E0)/E0);
    parametri.v=parametri.v+accelerazione(parametri.x,parametri)*passo;
    parametri.x=parametri.x+parametri.v*passo;
    i++;
  }
  fclose(p);
}
void leapfrog(struct set parametri){
  int i=0;
  double E0,temp,temp2;
  char titolo[100];
  FILE *p;
  sprintf(titolo, "oscillatorearmonico_leapfrog_x0=%.2lf_v0=%.2lf_massa=%.2lf.dat", parametri.x, parametri.v, parametri.m);
  p=fopen(titolo, "w");
  E0=energia(parametri);
  temp=parametri.v+accelerazione(parametri.x,parametri)*passo/2;
  while (i*passo<parametri.tmax) {
    fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*passo,parametri.x, parametri.v, (energia(parametri)-E0)/E0);
    temp2=temp+accelerazione(parametri.x,parametri)*passo;
    parametri.x=parametri.x+temp2*passo;
    parametri.v=(temp2+temp)/2;
    temp=temp2;
    i++;
  }
  fclose(p);
}
void midpoint(struct set parametri){
  int i=0;
  double E0, temp;
  FILE *p;
  char titolo[100];
  sprintf(titolo, "oscillatorearmonico_midpoint_x0=%.2lf_v0=%.2lf_massa=%.2lf.dat", parametri.x, parametri.v, parametri.m);
  p=fopen(titolo, "w");
  E0=energia(parametri);
  temp=parametri.v;
  while (i*passo<parametri.tmax) {
    fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*passo, parametri.x, parametri.v, (energia(parametri)-E0)/E0);
    parametri.v=parametri.v+accelerazione(parametri.x,parametri)*passo;
    parametri.x=parametri.x+(parametri.v+temp)/2*passo;
    temp=parametri.v;
    i++;
  }
  fclose(p);
}
