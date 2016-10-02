#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define g 9.8171
#define passo 0.001
struct pendolo{
    double L;
    double alpha; // eliminato il tempo
    double v;
    double intervallo;
    double energia;
    double accelerazione;
    double massa;
    double A;
    double omega;
    double teta;
};
typedef struct pendolo spazio;
double read(char *);
double acc(spazio);
double energy(spazio);
void verlet(spazio);
double traiettoria(spazio,double);
int main(){
    spazio variabili;
    variabili.L=read("Inserire la lunghezza del pendolo\t");
    variabili.alpha=read("Inserire l'angolo iniziale in gradi centigradi\t")/360*2*M_PI;
    variabili.v=read("Inserire la velocita' iniziale del pendolo in metri al secondo\t");
    //variabili.t=0;
    variabili.intervallo=read("Inserire l'intervallo in cui studiamo il moto\t");
    variabili.energia=energy(variabili);
    variabili.omega=sqrt(g/variabili.L);
    variabili.teta=-atan(variabili.v/variabili.omega/variabili.alpha);
    variabili.A=variabili.alpha/cos(variabili.teta);
    verlet(variabili);
//Acos(wt+fi) x0=Acos(fi) v0=-Awsin(fi)

    printf("\n\n\tf=function(x){%lf*cos(%lf*x+%lf)}\n\n",variabili.A,variabili.omega,variabili.teta);
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
    FILE *dati;
    dati=fopen("Data Frame.dat","w");
    fprintf(dati,"Tempo\tAngolo\tVelocita\tEnergia\tSfasamento\n");
    while(i*passo!=x.intervallo){
        fprintf(dati,"%lf\t%lf\t%lf\t%lf\t%lf\n",i*passo,x.alpha,x.v,x.energia,x.alpha-traiettoria(x,i*passo));
    	x.accelerazione=acc(x);
		x.alpha=x.alpha+x.v*passo+x.accelerazione*pow(passo,2)/2;
		temp=acc(x);
		x.v=x.v+(temp+x.accelerazione)*passo/2;
		x.energia=energy(x);
		i++;
	}
	fclose(dati);
}

double energy(spazio x){
    return x.massa*pow(x.L*x.v,2)/2+x.massa*g*x.L*(1-cos(x.alpha));
}

double traiettoria(spazio x,double t){
        return x.A*cos(x.omega*t+x.teta);
}
double acc(spazio x){
    return -g/x.L*sin(x.alpha);
}
