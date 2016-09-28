#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define g 9.8171
#define passo 0.002
struct pendolo{
    double L;
    double alpha;
    double t;
    double v;
    double intervallo;
    double energia;
    double accelerazione;
    double massa;
};
typedef struct pendolo spazio;
double read(char *);
double acc(spazio);
double energy(spazio);
void verlet(spazio);
int main(){
    spazio variabili;
    double A,omega,teta;
    variabili.L=read("Inserire la lunghezza del pendolo\t");
    variabili.alpha=read("Inserire l'angolo iniziale in gradi centigradi\t")/360*2*M_PI;
    variabili.v=read("Inserire la velocita' iniziale del pendolo in metri al secondo\t");
    variabili.t=0;
    variabili.intervallo=read("Inserire l'intervallo in cui studiamo il moto\t");
    variabili.energia=energy(variabili);
    omega=sqrt(g/variabili.L);
    teta=-atan(variabili.v/omega/variabili.alpha);
    A=variabili.alpha/cos(teta);
    verlet(variabili);
//Acos(wt+fi) x0=Acos(fi) v0=-Awsin(fi)

    printf("\n\n\talpha=%lfcos(%lf*t+%lf)\n\n",A,omega,teta);
    return 0;
}
double read(char *messaggio){
    double x;
    do{
        printf("%s",messaggio);
        scanf("%lf",&x);
        if(x<=0) printf("Inserire valori positivi!\n");
    } while(x<=0);
    return x;
}
void verlet(spazio x){
    int i=0;
    double temp;
    FILE *posizione,*velocita,*ene;
    posizione=fopen("Posizone vs t.dat","w");
    velocita=fopen("Velocita vs t.dat","w");
    ene=fopen("Energia vs t.dat","w");
    while(i*passo!=x.intervallo){
        fprintf(posizione,"%lf\t%lf\n",i*passo,x.alpha);
        fprintf(velocita,"%lf\t%lf\n",i*passo,x.v);
        fprintf(ene,"%lf\t%lf\n",i*passo,x.energia);
    	x.accelerazione=acc(x);
		x.alpha=x.alpha+x.v*passo+x.accelerazione*pow(passo,2)/2;
		temp=acc(x);
		x.v=x.v+(temp+x.accelerazione)*passo/2;
		x.energia=energy(x);
		i++;
	}
	fclose(posizione);
	fclose(velocita);
	fclose(ene);
}

double energy(spazio x){
    return x.massa*pow(x.L*x.v,2)/2+x.massa*g*x.L*(1-cos(x.alpha));
}

double acc(spazio x){
    return -g/x.L*sin(x.alpha);
}
