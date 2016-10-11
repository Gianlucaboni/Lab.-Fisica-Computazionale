#include<stdio.h>
#include<math.h>
#define g 9.81
#define passo 0.001
struct set {
  double alpha;
  double v;
  double L;
  double gamma;
  double massa;
  double tmax;
};
double lettura(char *);
double energia(struct set);
double accelerazione(struct set);
void leapfrog(struct set);
int main(){
	int scelta;
	struct set variabili;
	do{
		variabili.L=lettura("Inserire la lunghezza del pendolo\n");
		if(variabili.L<=0) printf("Inserire un valore strettamente positivo!\n\n");
	} while(variabili.L<=0);
	variabili.alpha=lettura("Inserire angolo per t=0 in radianti\n");
	variabili.v=lettura("Inserire la velocita' per t=0\n");
	do{
		variabili.massa=lettura("Inserire la massa del pendolo\n");
		if(variabili.massa<=0) printf("La massa deve essere strettamente positiva!\n");
	}while(variabili.massa<=0);
	do{
		variabili.tmax=lettura("Inserire estremo superiore d'integrazione\n");
		if(variabili.tmax<=0) printf("Il valore deve essere strettamente positivo\n");
	}while(variabili.tmax<=0);
	do{
		printf("Digitare:\n0 per lo studio del pendolo senza attrito;\n1 per lo studio del pendolo con attrito..,\n\n");
		scanf("%d",&scelta);
	}while (scelta!=0 && scelta!=1);
	if(scelta==0) variabili.gamma=0;
	if(scelta==1){
		do{
			variabili.gamma=lettura("Inserire il coefficiente di smorzamento gamma\n");
			if(variabili.gamma<0) printf("Il coefficiente e' definito positivo!\n");
		 }while(variabili.gamma<0);
	 }
	if(scelta==0) printf("Nel regime delle piccole oscillazioni si ha:\nA=%.10lf,\nfi=%.10lf\nomega=%.10lf\n",sqrt(pow(variabili.alpha,2)+pow(variabili.v,2)*variabili.L/g),atan(-variabili.v/(variabili.alpha*sqrt(g/variabili.L))),sqrt(g/variabili.L));
	leapfrog(variabili);
	return 0;	
}
double lettura(char *message){
  double x;
  printf("%s",message);
  scanf("%lf",&x);
  return x;
}
double accelerazione(struct set parametri){
	return -sin(parametri.alpha)*g/parametri.L-parametri.gamma*parametri.v;
}
double energia(struct set parametri){
	return parametri.massa*parametri.L*(parametri.L/2*pow(parametri.v,2)+g*(1-cos(parametri.alpha)));
}
void leapfrog(struct set parametri){
  int i=0;
  double E0,temp,temp2;
  char titolo[100];
  FILE *p;
  if(parametri.gamma==0) sprintf(titolo, "pendolo_semplice(x0=%.2lf;v0=%.2lf;massa=%.2lf).dat", parametri.alpha, parametri.v, parametri.massa);
	else  sprintf(titolo, "pendolo_smorzato(x0=%.2lf;v0=%.2lf;massa=%.2lf;gamma=%.2lf).dat", parametri.alpha, parametri.v, parametri.massa,parametri.gamma);
  p=fopen(titolo, "w");
  E0=energia(parametri);
  temp=parametri.v+accelerazione(parametri)*passo/2;
  while (i*passo<parametri.tmax) {
    fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*passo,parametri.alpha, parametri.v, (energia(parametri)-E0)/E0);
    temp2=temp+accelerazione(parametri)*passo;
    parametri.alpha=parametri.alpha+temp2*passo;
    parametri.v=(temp2+temp)/2;
    temp=temp2;
    i++;
  }
  fclose(p);
}








