#include<stdio.h>
#include<math.h>
#define alpha0 (double)  M_PI/2
#define v0 (double) 0.
#define omega (double) 0.
#define w_forzante (double) 2/3
#define passo (double) 0.001
#define gamma (double) 4
#define intervallo (double) 100

struct fase{
    double alpha;
    double v;
    double tempo;
    double f;
};
double equation(struct fase);
void kurge(struct fase,FILE *);
int main(){
    struct fase parametri;
    FILE *data1,*data2;
    printf("Inserire modulo ampiezza forza esterna:\t");
    scanf("%lf",&parametri.f);
    parametri.alpha=alpha0;
    parametri.v=v0;
    printf("\n\t\t%lf\t%lf\t%lf\t%lf",parametri.f,parametri.alpha,parametri.v,w_forzante);
    data1=fopen("Oscillatore.forzato.dat","w");
    data2=fopen("Oscillatore.forzato*.dat","w");
    fprintf(data1,"Tempo\tPosizione\tVelocita\n");
    fprintf(data2,"Tempo\tPosizione2\tVelocita2\n");
    kurge(parametri,data1);
    parametri.alpha=alpha0+0.03; //piccola variazione
    parametri.v=v0+0.02;
    kurge(parametri,data2);
}
double equation(struct fase x){
    return -pow(omega,2)*sin(x.alpha)-gamma*x.v+x.f*cos(w_forzante*x.tempo);
}
void kurge(struct fase grandezze,FILE *dati){
    int i=0;
    struct fase *temp;
    while(i*passo<intervallo){
        grandezze.tempo=i*passo;
        fprintf(dati,"%lf\t%lf\t%lf\n",i*passo,grandezze.alpha,grandezze.v);
        temp=&grandezze;
        temp->alpha=grandezze.alpha+grandezze.v*passo/2;
        temp->v=grandezze.v+equation(grandezze)/2*passo;
        grandezze.alpha=grandezze.alpha+(grandezze.v+equation(grandezze)/2*passo)*passo;
        grandezze.v=grandezze.v+equation(*temp)*passo;
        i++;
    }
    fclose(dati);
}
