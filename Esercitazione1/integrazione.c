#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#define g 9.81
#define deltaT 0.01
struct set{
  double x;
  double v;
  double a;
  double e;
  double m;
  double k; //costante elastica o coefficiente d'ATTRITO
  double tmax;
  double L;
  double t;
  double F;
  double omega;
};

double energia_oscillatore(struct set);
double accelerazione_oscillatore(struct set);
double energia_pendolo(struct set);
double accelerazione_pendolo(struct set);
void eulero(struct set ,double(*)(struct set),double(*)(struct set),char *,int,FILE*); //da modificare i prototipi
void eulerocromer(struct set,double(*)(struct set),double(*)(struct set),char *,int,FILE*);
void midpoint(struct set,double(*)(struct set),double(*)(struct set),char *,int,FILE*);
void leapfrog(struct set,double(*)(struct set),double(*)(struct set),char *,int,FILE*);
double lettura(char *);
void periodo(int,int,FILE *,double *);
int  ampiezze(struct set,double,int, FILE *);
void plot(char *);
/*double fit(double, struct set);*/

int main(){
	struct set variabili;
	int scelta,cont=1;
	char titolo[80],titolo2[80];
	double (*f_accelerazione)(struct set);
	double (*f_energia)(struct set);
	FILE *p3;
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
				cont=1;
				sprintf(titolo,"oscillatore_eulero_(x0=%.3lf_v0=%lf.3).dat", variabili.x, variabili.v);
				strcpy(titolo2,"CONSERVAZIONE_ENERGIA_vs_DELTA_T-");
				strcat(titolo2,titolo);
				p3=fopen(titolo2,"w");
			while(cont<=20){
				eulero(variabili,f_accelerazione,f_energia,titolo,cont,p3);
				cont++;
			}
		}
		if(scelta==2){
				cont=1;
				sprintf(titolo, "oscillatore_eulero-cromer_(x0=%.3lf_v0=%.3lf).dat", variabili.x, variabili.v);
				strcpy(titolo2,"CONSERVAZIONE_ENERGIA_vs_DELTA_T-");
				strcat(titolo2,titolo);
				p3=fopen(titolo2,"w");
			while(cont<=20){
				eulerocromer(variabili,f_accelerazione,f_energia,titolo,cont,p3);
				cont++;
			}
		}
		if(scelta==3){
				cont=1;
				sprintf(titolo, "oscillatore_midpoint_(x0=%.3lf_v0=%.3lf).dat", variabili.x, variabili.v);
				strcpy(titolo2,"CONSERVAZIONE_ENERGIA_vs_DELTA_T-");
				strcat(titolo2,titolo);
				p3=fopen(titolo2,"w");
			while(cont<=20){
				midpoint(variabili,f_accelerazione,f_energia,titolo,cont,p3);
				cont++;
			}
		}
		if(scelta==4){
				cont=1;
				sprintf(titolo, "oscillatore_leapfrog_(x0=%.3lf_v0=%.3lf).dat", variabili.x, variabili.v);
				strcpy(titolo2,"CONSERVAZIONE_ENERGIA_vs_DELTA T-");
				strcat(titolo2,titolo);
				p3=fopen(titolo2,"w");
			while(cont<=20){
			 leapfrog(variabili,f_accelerazione,f_energia,titolo,cont,p3);
			 cont++;
			}
		}
		scelta=1;
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
			variabili.k=lettura("Inserisci coefficiente d'attrito. Digitare 0 per studiare pendolo senza attrito\n");
			if(variabili.k<0) printf("Il parametro inserito non deve essere minore di zero, riprova\n");
		}while (variabili.k<0);
	    variabili.F=lettura("Inserisci forzante...inserire 0 per studiare il pendolo senza forzante\n");
		if(variabili.F!=0){
			do{
				variabili.omega=lettura("Inserisci pulsazione forzante\n");
				if(variabili.omega<=0) printf("Il parametro inserito deve essere maggiore di zero, riprovare\n");
			}while(variabili.omega<=0);   
		}
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
			cont=1;
			sprintf(titolo, "pendolo_eulero(x0=%.3lf_v0=%.3lf_gamma=%.3lf).dat", variabili.x, variabili.v, variabili.k);
			strcpy(titolo2,"CONSERVAZIONE_ENERGIA_vs_DELTA_T-");
			strcat(titolo2,titolo);
			p3=fopen(titolo2,"w");
			while(cont<=20){
				eulero(variabili,f_accelerazione,f_energia,titolo,cont,p3);	
				cont++;
			}
		}
		if(scelta==2){
			cont=1;
			sprintf(titolo, "pendolo_eulero-cromer(x0=%.3lf_v0=%.3lf_gamma=%.3lf).dat", variabili.x, variabili.v, variabili.k);
			strcpy(titolo2,"CONSERVAZIONE_ENERGIA_vs DELTA_T-");
			strcat(titolo2,titolo);
			p3=fopen(titolo2,"w");
			while(cont<=20){
				eulerocromer(variabili,f_accelerazione,f_energia,titolo,cont,p3);
				cont++;
			}
		}
		if(scelta==3){
			cont=1;
			sprintf(titolo, "pendolo_midpoint(x0=%.3lf_v0=%.3lf_gamma=%.3lf).dat", variabili.x, variabili.v, variabili.k);
			strcpy(titolo2,"CONSERVAZIONE_ENERGIA_vs_DELTA_T-");
			strcat(titolo2,titolo);
			p3=fopen(titolo2,"w");
			while(cont<=20){
				midpoint(variabili,f_accelerazione,f_energia,titolo,cont,p3);
				cont++;
			}
		}
		if(scelta==4){
			sprintf(titolo, "pendolo_leapfrog(x0=%.3lf_v0=%.3lf_gamma=%.3lf).dat", variabili.x, variabili.v, variabili.k);
			strcpy(titolo2,"CONSERVAZIONE_ENERGIA_vs_DELTA_T-");
			strcat(titolo2,titolo);
			p3=fopen(titolo2,"w");
			while(cont<=20){
				leapfrog(variabili,f_accelerazione,f_energia,titolo,cont,p3);
				cont++;
			}
		}
	}
	return 0;
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
  return -sin(parametri.x)*g/parametri.L-parametri.k*parametri.v+parametri.F*cos(parametri.omega*parametri.t);
}
double energia_pendolo(struct set parametri){
	return parametri.m*parametri.L*(parametri.L/2*pow(parametri.v,2)+g*(1-cos(parametri.x)));
}
void eulero(struct set parametri,double (*accelerazione)(struct set),double (*energia)(struct set),char *titolo,int cont,FILE*p3){ //calcola il periodo
	int i=0,j=1, maxmin=0, z;
	double E0,temp,prima,passo;
	double *t;
	char titolo2[100];
	FILE *p,*p2,*f;
	t=malloc(1*sizeof(double));
	strcpy(titolo2,"PERIODO-");
	p=fopen(titolo, "w");
	strcat(titolo2,titolo);
	p2=fopen(titolo2,"w");
	strcpy(titolo2,"AMPIEZZE-");
	strcat(titolo2,titolo);
	f=fopen(titolo2,"w");
	E0=energia(parametri);
	passo=cont*deltaT/20;
	while (i*passo<=parametri.tmax){
		if(cont==20)	fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*deltaT, parametri.x, parametri.v, (energia(parametri)-E0)/E0);
		parametri.t=i*passo;
		prima=parametri.x; //la posizione prima del deltaT
		temp=accelerazione(parametri);
		parametri.x=parametri.x+parametri.v*passo;
		parametri.v=parametri.v+temp*passo;
		if(cont==20){
			z=maxmin;
			maxmin=ampiezze(parametri,prima,z,f);
			if(prima*parametri.x<0){
				t=(double *)realloc(t,(j+1)*sizeof(double));
				periodo(i,j,p2,t);
				j++;
			}
		}
		if((parametri.tmax/2-parametri.t)<=passo && (parametri.tmax/2-parametri.t)>0) fprintf(p3,"%.10lf\t%.10lf\n",passo,(energia(parametri)-E0)/E0);
		i++;
	}
	if(cont==20){
		fclose(p);
		fclose(p2);
		fclose(f);
		fclose(p3);
		plot(titolo);
	}
}
void eulerocromer(struct set parametri,double (*accelerazione)(struct set),double (*energia)(struct set),char *titolo,int cont, FILE *p3){
	int i=0,j=1, maxmin=0,z;
	double E0,prima,passo;
	double *t;
	char titolo2[100];
	FILE *p,*p2, *f;
	t=malloc(1*sizeof(double));
	strcpy(titolo2,"PERIODO-");
	p=fopen(titolo, "w");
	strcat(titolo2,titolo);
	p2=fopen(titolo2,"w");
	strcpy(titolo2,"AMPIEZZE-");
	strcat(titolo2,titolo);
	f=fopen(titolo2,"w");
	E0=energia(parametri);
	passo=cont*deltaT/20;
	while (i*passo<parametri.tmax) {
		if(cont==20)fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*passo, parametri.x, parametri.v, (energia(parametri)-E0)/E0);
		parametri.t=i*passo;
		prima=parametri.x;
		parametri.v=parametri.v+accelerazione(parametri)*passo;
		parametri.x=parametri.x+parametri.v*passo;
		if(cont==20){
			z=maxmin;
			maxmin=ampiezze(parametri,prima,z,f);
			if(prima*parametri.x<0){
				t=(double *)realloc(t,(j+1)*sizeof(double));
				periodo(i,j,p2,t);
				j++;
			}
		}
		if((parametri.tmax/2-parametri.t)<=passo && (parametri.tmax/2-parametri.t)>0) fprintf(p3,"%.10lf\t%.10lf\n",passo,(energia(parametri)-E0)/E0);
		i++;
	}
	if(cont==20){
		fclose(p);
		fclose(p2);
		fclose(f);
		fclose(p3);
		plot(titolo);
	}
}
void leapfrog(struct set parametri,double (*accelerazione)(struct set),double (*energia)(struct set),char *titolo,int cont,FILE *p3){
	int i=0,j=1, maxmin=0, z;
	double E0,temp,temp2,prima,*t,passo;
	char titolo2[80];
	FILE *p,*p2, *f;
	t=malloc(1*sizeof(double));
	strcpy(titolo2,"PERIODO-");
	p=fopen(titolo, "w");
	strcat(titolo2,titolo);
	p2=fopen(titolo2,"w");
	strcpy(titolo2,"AMPIEZZE-");
	strcat(titolo2,titolo);
	f=fopen(titolo2,"w");
	E0=energia(parametri);
	temp=parametri.v+accelerazione(parametri)*deltaT/2;
	passo=cont*deltaT/20;
	while (i*passo<parametri.tmax) {
		if(cont==20)fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*passo,parametri.x, parametri.v, (energia(parametri)-E0)/E0);
		parametri.t=i*passo;
		prima=parametri.x;
		temp2=temp+accelerazione(parametri)*passo;
		parametri.x=parametri.x+temp2*passo;
		parametri.v=(temp2+temp)/2;
		temp=temp2;
		if(cont==20){
			z=maxmin;
			maxmin=ampiezze(parametri,prima,z,f);
			if(prima*parametri.x<0) {
				t=(double *)realloc(t,(j+1)*sizeof(double));
				periodo(i,j,p2,t);
			j++;
			}
		}
		if((parametri.tmax/2-parametri.t)<=passo && (parametri.tmax/2-parametri.t)>0) fprintf(p3,"%.10lf\t%.10lf\n",passo,(energia(parametri)-E0)/E0);
		i++;
	}
	if(cont==20){
		fclose(p);
		fclose(p2);
		fclose(f);
		fclose(p3);
		plot(titolo);
	}
}
void midpoint(struct set parametri,double (*accelerazione)(struct set),double (*energia)(struct set),char *titolo,int cont,FILE *p3){
	int i=0,j=1,maxmin=0,z;
	double E0, temp,prima,*t,passo;
	char titolo2[80];
	t=malloc(1*sizeof(double));
	strcpy(titolo2,"PERIODO-");
	FILE *p,*p2,*f;
	p=fopen(titolo, "w");
	strcat(titolo2,titolo);
	p2=fopen(titolo2,"w");
	strcpy(titolo2,"AMPIEZZE-");
	strcat(titolo2,titolo);
	f=fopen(titolo2,"w");
	E0=energia(parametri);
	temp=parametri.v;
	passo=cont*deltaT/20;
	while (i*deltaT<parametri.tmax) {
		if(cont==20)fprintf(p, "%.10lf\t %.10lf\t %.10lf\t %.10lf\t\n", i*passo, parametri.x, parametri.v, (energia(parametri)-E0)/E0);
		parametri.t=i*passo;
		prima=parametri.x;
		parametri.v=parametri.v+accelerazione(parametri)*passo;
		parametri.x=parametri.x+(parametri.v+temp)/2*passo;
		temp=parametri.v;
		if(cont==20){
			z=maxmin;
			maxmin=ampiezze(parametri,prima,z,f);
			if(prima*parametri.x<0) {
				t=(double *)realloc(t,(j+1)*sizeof(double));
				periodo(i,j,p2,t);
				j++;
			}
		}
		if((parametri.tmax/2-parametri.t)<=passo && (parametri.tmax/2-parametri.t)>0) fprintf(p3,"%.10lf\t%.10lf\n",passo,(energia(parametri)-E0)/E0);
		i++;
	}
	if(cont==20){
		fclose(p);
		fclose(p2);
		fclose(f);
		fclose(p3);
		plot(titolo);
	}
}
void periodo(int i,int j,FILE *p2,double *t){
	*(t+j-1)=(i*deltaT+(i-1)*deltaT)/2;
	if(j>2) fprintf(p2,"%d\t%.10lf\n",j-3,*(t+j-1)-(*(t+j-3)));
}
int ampiezze(struct set parametri,double prima,int z,FILE *f){
  int temp;
  if(z==0) { //cerca massimi
    if(prima>parametri.x) {
      fprintf(f,"%.10lf\t%.10lf\n",parametri.t-deltaT/2,fabs(prima));
      temp=1;
      return temp;
    }
  }
  if(z==1){ //cerca minimi
    if(prima<parametri.x) {//trovato minimo
      fprintf(f,"%.10lf\t%.10lf\n",parametri.t-deltaT/2,fabs(prima)); //fit(prima, parametri)
      temp=0;
      return temp;
    }
  }
  return z;
}
/*double fit(double prima, struct set parametri){
	double x1,x2,x3,t1,t2,t3,D,D1,D2,D3,a,b,c; //D1=Da,D2=Db,D3=Dc
  	x1=prima;
  	x2=parametri.x;
  	x3=parametri.x+parametri.v*deltaT/2;
  	t1=parametri.t-deltaT;
  	t2=parametri.t;
  	t3=parametri.t+deltaT/2;
	//printf("\n\n\n%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n\n\n",x1,x2,x3,t1,t2,t3);	
  	D=pow(t2,2)*t3-pow(t3,2)*t2-pow(t1,2)*t3+pow(t3,2)*t1+pow(t1,2)*t2-t1*pow(t2,2);
  	D1=pow(x2,2)*t3-pow(x3,2)*t2-pow(x1,2)*t3+pow(x3,2)*t1+pow(x1,2)*t2-t1*pow(x2,2);
  	D2=pow(t2,2)*x3-pow(t3,2)*x2-pow(t1,2)*x3+pow(t3,2)*x1+pow(t1,2)*x2-x1*pow(t2,2);
  	D3=x1*(pow(t2,2)*t3-pow(t3,2)*t2)-x2*(pow(t1,2)*t3-pow(t3,2)*t1)+x3*(pow(t1,2)*t2-t1*pow(t2,2));
  	a=D1/D;
  	b=D2/D;
  	c=D3/D;
	//printf("\n\n\n%lf\t%lf\t%lf\t%lf\n\n\n",D, Da, Db, Dc);
	printf("\n\n\t%.10lf\t%.10lf\t%.10lf\tmax=%.10lf\n\n\n",a,b,c,(-pow(b,2)+4*a*c)/4*a);	
	return (-pow(b,2)+4*a*c)/4*a;
}*/
void plot(char *file){
	FILE * gnuplotPipe=popen("gnuplot -persistent","w");
	if (gnuplotPipe==NULL){
		printf("Error opening pipe to GNU plot. Check if you have it! \n");
		exit(0);
    }
	fprintf(gnuplotPipe,"set term X11 0\n set title 'Traiettoria'\n set xlabel 'Tempo'\n set ylabel 'Posizione'\n plot '%s' w l\n",file);
	fprintf(gnuplotPipe,"set term png\n set out 'TRAIETTORIA.png'\n replot\n");
	fprintf(gnuplotPipe,"set term X11 1\n set title 'Spazio delle Fasi'\n set xlabel 'Posizione'\n set ylabel 'Velocita'\n plot '%s' u 2:3 w l\n",file);
	fprintf(gnuplotPipe,"set term png\n set out 'SPAZIO_DELLE_FASI.png'\n replot\n");
	fprintf(gnuplotPipe,"set term x11 2\n set title 'Andamento dei massimi'\n set xlabel 'Tempo'\n set ylabel 'Ampiezze'\n plot 'AMPIEZZE-%s' \n",file);
	fprintf(gnuplotPipe,"set term png\n set out 'MASSIMI&MINIMI.png'\n replot\n");
	fprintf(gnuplotPipe,"set term X11 3\n set title 'Periodo'\n set xlabel 'Numero di periodo calcolato'\n set ylabel 'Periodo' \n plot 'PERIODO-%s' w l\n",file);
	fprintf(gnuplotPipe,"set term png\n set out 'PERIODO.png'\n replot\n");
	fprintf(gnuplotPipe,"set term X11 3\n set title 'Conservazione Energia'\n set xlabel 'Tempo'\n set ylabel 'Delta{E}/E0' \n plot '%s' u 1:4 w l\n",file);
	fprintf(gnuplotPipe,"set term png\n set out 'CONSERVAZIONE_ENERGIA.png'\n replot\n");
	fprintf(gnuplotPipe,"set term X11 3\n set title 'Conservazione Energia VS passo'\n set xlabel 'Passo'\n set ylabel 'Delta{E}/E0' \n plot 'CONSERVAZIONE_ENERGIA_vs_DELTA_T-%s' \n",file);
	fprintf(gnuplotPipe,"set term png\n set out 'CONSERVAZIONE_ENERGIA_VS_PASSO.png'\n replot\n");
	fclose(gnuplotPipe);
	}
