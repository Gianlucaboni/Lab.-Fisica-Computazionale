#include<stdio.h>
#include<math.h>
#define massa 1
#define passo (double) 0.02
#define intervallo (double) 100
struct spazio{
	double x;
	double y;
	double vx;
	double vy;
	};
void verlet(struct spazio);
double accX(struct spazio);
double accY(struct spazio);
double read(char*);
int main(){
	struct spazio campo;
	campo.x=read("Inserire la posizione iniziale lungo l'asse x\n");
	campo.y=read("Inserire la posizione iniziale lungo l'asse y\n");
	campo.vx=read("Inserire la velocita iniziale lungo l'asse x\n");
	campo.vy=read("Inserire la velocita iniziale lungo l'asse y\n");
	verlet(campo);
	return 0;	
}
void verlet(struct spazio campo){
	double tempx,tempy;
	int i=0;
	FILE *fp;
	fp=fopen("Dati.dat","w");
	//fprintf(fp,"TEMPO\tPOSIZIONE(X)\tPOSIZIONE(Y)\tVELOCITA(X)\tVELOCITA(Y)\n");
	while(i*passo<=intervallo){
		fprintf(fp,"%lf\t%lf\t%lf\t%lf\t%lf\n",i*passo,campo.x,campo.y,campo.vx,campo.vx);
		tempx=accX(campo);
		tempy=accY(campo);
		campo.x=campo.x+campo.vx*passo+accX(campo)*pow(passo,2)/2;
		campo.y=campo.y+campo.vy*passo+accY(campo)*pow(passo,2)/2;
		campo.vx=campo.vx+(accX(campo)+tempx)/2*passo;
		campo.vy=campo.vy+(accY(campo)+tempy)/2*passo;
		i++;
	}
}
double accX(struct spazio campo){
	return cos(campo.x);
}
double accY(struct spazio campo){
	return sin(campo.y);
}
double read(char *message){
	double x;
	printf("%s",message);
	scanf("%lf",&x);
	return x;
}
