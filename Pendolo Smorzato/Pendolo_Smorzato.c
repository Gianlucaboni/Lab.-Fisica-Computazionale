#include <stdio.h>
#include <math.h>
#define passo 0.001
#define epsilon 0.01
struct spazio{
    double x;
    double omega2;
    double v;
    double gamma;
    double intervallo;
};
typedef struct spazio set;

void kurge(set,FILE *);
double read(char *);
double acc_smorzato(set);
void find_gamma(set);
int main(){
    set variabili;
    FILE *dati1,*dati2;
    dati1=fopen("Oscillatore_Smorzato.dat","w");
    dati2=fopen("Oscillatore.dat","w");
    fprintf(dati1,"Tempo\tPosizione\tVelocita\n");
    fprintf(dati2,"Tempo\tPosizione\tVelocita\n");
    variabili.v=0;
    variabili.x=1;
    variabili.omega2=9;
    variabili.gamma=read("Inserire valore del coefficiente di smorzamento gamma");
    variabili.intervallo=read("Inserire la durata del tempo d'integrazione");
    kurge(variabili,dati1);
    find_gamma(variabili);
    variabili.v=0;
    variabili.x=1;
    variabili.omega2=9;
    variabili.gamma=0;
    kurge(variabili,dati2);

    return 0;
}
double read(char *message){
    double x;
    do{
        printf("\n%s\t",message);
        scanf("%lf",&x);
        if(x<=0) printf("\nIl coefficiente e' definito positivo!\n");
    } while(x<=0);
    return x;
}
void kurge(set data,FILE *dati){
    int i=0;
    set *temp;
while(i*passo<data.intervallo){
        fprintf(dati,"%lf\t%lf\t%lf\n",i*passo,data.x,data.v);
        temp=&data;
        temp->x=data.x+data.v*passo/2;
        temp->v=data.v+acc_smorzato(data)/2*passo;
        data.x=data.x+(data.v+acc_smorzato(data)/2*passo)*passo;
        data.v=data.v+acc_smorzato(*temp)*passo;
        i++;
    }
    fclose(dati);
}
double acc_smorzato(set data){
    return -data.omega2*data.x-data.gamma*data.v;
}
void find_gamma(set data){
    set *temp;
    int i=0;
    double pos=data.x;
    double vel=data.v;
    do{
        temp=&data;
        temp->x=data.x+data.v*passo/2;
        temp->v=data.v+acc_smorzato(data)/2*passo;
        data.x=data.x+(data.v+acc_smorzato(data)/2*passo)*passo;
        data.v=data.v+acc_smorzato(*temp)*passo;
        i++;
        if(data.x<(double) 0.001){
            i=0;
            data.x=pos;
            data.v=vel;
            data.gamma=data.gamma+0.01;
        }
    }while(i*passo<data.intervallo);
    printf("\n\n\tIl coefficiente cercato e':%lf\n\n",data.gamma);

}
