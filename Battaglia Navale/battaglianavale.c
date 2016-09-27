#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define A 10 //righe
#define B 10 //colonne
#define C 4 //singole
#define D 3 //doppie
#define E 2 //triple
#define F 1 //quadruple

void compilazione(int [][B+4],void(*)(),void (*)(int [][B+4],int,int *,int*,int*,int*,int,int));
void posizionamentoplus(int [][B+4],int,int,void (*) (int a[]),void(*)(int [][B+4],int [][B+4]),void (*)(int [][B+4],int,int *,int*,int*,int*,int,int)); //AVENDO SOMMATO 2 POSSO FARE UNO STRATO CON DEI 2 e UNO CON 0 POI STAMPO DA 2 A B+1
void attacco(void (*) (int a[]),int [][B+4]);
void difesa(int [][B+4],int*,int*,int*,int*);
void selezione(int a[]);
int check(int[][B+4],int[][B+4]);
void sonda(int [][B+4],int,int,void (*)(int [][B+4],int,int *,int*,int*,int*,int,int));
void stampa(int [][B+4],int [][B+4]);
void ispezioneintorno(int [][B+4],int,int *,int*,int*,int*,int,int);
void recinto(int [][B+4],int [][B+4],int);

int main (){
	printf("\n\n");
	
	void (*griglia) (int [][B+4],int [][B+4]);
	void (*fun)(int a[]);	
	void (*intorno)(int [][B+4],int,int *,int*,int*,int*,int,int);
	void (*sonar)(int [][B+4],int,int,void (*)(int [][B+4],int,int *,int*,int*,int*,int,int));
	int flotta1[A+4][B+4]={0}, flotta2[A+4][A+4]={0},fine=0;
	int a=-1,b=-1,v=0,o=0,temp;
	

	
	srand48(time(0)); //inizializzazione rand
	fun=&selezione;
	sonar=&sonda;
	griglia=&stampa;
	intorno=&ispezioneintorno;
	
	printf("Benvenuto e che la battaglia navale abbia inizio!\n\n\n");
	
	recinto(flotta1,flotta2,3);
	compilazione(flotta2,sonar,intorno);
	//stampa(flotta2,flotta2);
	printf("Inserisci le tue navi in un rettangolo di dimensioni %dX%d. Per ogni nave devi assegnare due numeri: il primo indica la riga, il secondo la colonna. Buona fortuna!\n\n\n",A,B);
	printf("Premere 0 per compilazione automatica, 1 per quella manuale...\n\n");
	scanf("%d",&temp);
	if(temp==0) compilazione(flotta1,sonar,intorno);
	else {
		posizionamentoplus(flotta1,C,1,fun,griglia,intorno); // numero di navi e lunghezza
		posizionamentoplus(flotta1,D,2,fun,griglia,intorno);
		posizionamentoplus(flotta1,E,3,fun,griglia,intorno);
		posizionamentoplus(flotta1,F,4,fun,griglia,intorno);
	}
	recinto(flotta1,flotta2,2);
	while(fine==0){
		stampa(flotta1,flotta2);
		attacco(fun,flotta2);
		fine=check(flotta1,flotta2);
		if(fine==0) difesa(flotta1,&a,&b,&o,&v);
	}
	if(fine==1) printf("Congratulazioni, hai vinto!\n");
		else printf("Mi dispiace ma stai affondandooo... glu glu\n"); 
	return 0;
}
void compilazione(int data[][B+4],void (*sonar)(int [][B+4],int,int,void (*)(int [][B+4],int,int *,int*,int*,int*,int,int)),void (*intorno)(int [][B+4],int,int *,int*,int*,int*,int,int)){// numero di navi e lunghezza
		sonar(data,F,4,intorno);
		sonar(data,E,3,intorno);
		sonar(data,D,2,intorno);
		sonar(data,C,1,intorno);
}
void selezione(int a[]){
	int x,y;
			do{
				printf("Riga \t");
				scanf("%d",&x);
				if( x>A || x<=0) printf("Hai a disposizione solo %d file! Il numero della riga da assegnare va da 1 a %d,riprova...\n\n\n",A,A);
			} while(x>A || x<=0);
			do{
				printf("Colonna \t");
				scanf("%d",&y);
				printf("\n");
				if( y>B || y<=0) printf("Hai a disposizione solo %d file! Il numero della riga da assegnare va da 1 a %d,riprova...\n\n\n",B,B);
			} while(y>B || x<=0);
	a[0]=x+1;
	a[1]=y+1;
}
void attacco(void (*f) (int a[]),int data[][B+4]){
	int flag=0,xy[2];
	printf("\nE' il tuo turno! Scegli il punto da bombardare inserendo riga e colonna\n");
	while(flag==0){
		f(xy);
		if (data[xy[0]][xy[1]]==2){
			printf("Hai bombardato un punto gia' colpito... ti stai facendo vecchio eh!? Su, riprova...\n\n\n");
		}
		if (data[xy[0]][xy[1]]==0) {
			printf("E' stato semplicemente un... buco nell'acqua :(\n\n\n");
			data[xy[0]][xy[1]]=2;
			flag=1;
		}
		if (data[xy[0]][xy[1]]==1) {
				if(data[xy[0]+1][(xy[1])]==1 || data[xy[0]-1][xy[1]]==1 || data[xy[0]][(xy[1])+1]==1 || data[xy[0]][(xy[1])-1]==1) printf("COLPITO! C'e' ancora qualcosa qui intorno...\n\n\n");
			 
			else printf("COLPITO E AFFONDATO!! Ottimo lavoro! Ancora qualche colpo ben assestato e potrai vantarti di essere il vincitore\n\n\n");
			data[xy[0]][xy[1]]=2;
			flag=1;		
		}
		
	}
}
void difesa(int data[][B+4],int*a,int*b,int*o,int*v){
	int x,y,flag=0,rand,temp=0;
	while(flag==0){
		if(*a<0 && *b<0){ // non ha idea  di dove sia: va a cazzo
			x=lrand48()%A+2; 
			y=lrand48()%B+2;
			if(data[x][y]==1) {
				printf("Mi dispiace mio caro... sei stato colpito\n\n\n");
				flag=1;
				data[x][y]=2;
				if(data[x+1][y]==1 || data[x-1][y]==1 || data[x][y+1]==1 || data[x][y-1]==1){ //questo if mi permette di aggiungere suggerimenti se intorno c'e qualcosa
					*a=x; //se sei stato semplicemente colpito
					*b=y;	
				}		
			}
			if(data[x][y]==0){
				printf("Il tuo avversario ha fatto flop: resisti e vincerai!\n\n\n");
				flag=1;
				data[x][y]=2;
			}
		} 
		else { // se hai dei suggerimenti
			if(*o==0 && *v==0){ //se non hai info su verticale o orizzontale
			rand=lrand48()%4+1;
				if(rand==1 && data[*a-1][*b]!=2){ // vai su
					if(data[*a-1][*b]==1){
						printf("Mi dispiace mio caro... sei stato colpito\n\n\n");
						data[*a-1][*b]=2;
						*a=*a-1;
						*v=1;
					}	
					else{ 
						printf("Il computer ha fatto cilecca!\n\n\n");
						data[*a-1][*b]=2;
					}		
					flag=1;		
				}
				if(rand==2 && data[*a+1][*b]!=2){ //vaai a giu
					if(data[*a+1][*b]==1){
						printf("Mi dispiace mio caro... sei stato colpito\n\n\n");
						data[*a+1][*b]=2;
						*a=*a+1;
						*v=1;
					}
					else{ 
						printf("Il computer ha fatto cilecca!\n\n\n");
						data[*a+1][*b]=2;
					}
					flag=1;
				}		
				if(rand==3 && data[*a][*b+1]!=2){ //vaai dx
					if(data[*a][*b+1]==1){
						printf("Mi dispiace mio caro... sei stato colpito\n\n\n");
						data[*a][*b+1]=2;
						*b=*b+1;
						*o=1;
					}
					else{ 
						printf("Il computer ha fatto cilecca!\n\n\n");
						data[*a][*b+1]=2;
					}
				flag=1;
				}
				if(rand==4 && data[*a][*b-1]!=2){ // vai a sx
					if(data[*a][*b-1]==1){
						printf("Mi dispiace mio caro... sei stato colpito\n\n\n");
						data[*a][*b-1]=2;
						*b=*b-1;
						*o=1;
					}
					else{ 
						printf("Il computer ha fatto cilecca!\n\n\n");
						data[*a-1][*b]=2;
					}		
				flag=1;	
				}
			}
			if(*o!=0 && flag==0){        // se il computer sa di doversi muovere ORIZZONTALE
				rand=lrand48()%2;
				if(rand==0 && data[*a][*b+1]!=2){ //vaai dx
					if(data[*a][*b+1]==1){
						printf("Mi dispiace mio caro... sei stato colpito\n\n\n");
						data[*a][*b+1]=2;
						*b=*b+1;
					}
					else{ 
						printf("Il computer ha fatto cilecca!\n\n\n");
						data[*a][*b+1]=2;
						*b=*b-1;
						if(data[*a][*b-2]==1) {
							*b=*b-1;
							temp=1;
						}
					}
					flag=1;
				}
				if(rand==1 && data[*a][*b-1]!=2){ // vai a sx
					if(data[*a][*b-1]==1){
						printf("Mi dispiace mio caro... sei stato colpito\n\n\n");
						data[*a][*b-1]=2;
						*b=*b-1;
					}
					else{ 
						printf("Il computer ha fatto cilecca!\n\n\n");
						data[*a][*b-1]=2;
						*b=*b+1;
						if(data[*a][*b+2]==1) {
							*b=*b+1;
							temp=1;
						}
					}		
					flag=1;	
				}
				if(data[*a][*b+1]==2 && data[*a][*b-1]==2){
					*a=-1;
					*b=-1;
					*o=0;
					printf("COLPITO E AFFONDATO! Stai perdendo navi mio caro...\n\n\n");
				}
			}
			if(*v!=0 && flag==0){ // se il computer sa di doversi muovere verticale
				rand=lrand48()%2;
				if(rand==0 && data[*a-1][*b]!=2){ //vaai su
					if(data[*a-1][*b]==1){
						printf("Mi dispiace mio caro... sei stato colpito\n\n\n");
						data[*a-1][*b]=2;
						*a=*a-1;
					}
					else{ 
						printf("Il computer ha fatto cilecca!\n\n\n");
						data[*a-1][*b]=2; 
						*a=*a+1;
						if(data[*a+2][*b]==1) {
							*a=*a+1;
							temp=1;
						}
					}
				flag=1;
				}
				if(rand==1 && data[*a+1][*b]!=2){ // vai a giu
					if(data[*a+1][*b]==1){
						printf("Mi dispiace mio caro... sei stato colpito\n\n\n");
						data[*a+1][*b]=2;
						*a=*a+1;
					}
					else{ 
						printf("Il computer ha fatto cilecca!\n\n\n");
						data[*a+1][*b]=2;
						*a=*a-1;
						if(data[*a-2][*b]==1) {
							*a=*a-1;		
							temp=1;
						}
					}		
				flag=1;	
				}
				if(data[*a+1][*b]==2 && data[*a-1][*b]==2){
					*a=-1;
					*b=-1;
					*v=0;		
					printf("COLPITO E AFFONDATO!!\n\n");
				}	
			}
		}
		if(*a!=-1 && temp==0 && data[*a][*b+1]!=1 && data[*a][*b-1]!=1 && data[*a+1][*b]!=1 && data[*a-1][*b]!=1){ // questo if si attiva quanda intorno non hai nessuna nave
			*a=-1;
			*b=-1;
			*v=0;
			*o=0;
			printf("COLPITO E AFFONDATO!!\n\n");
		}
	}	
}	
int check(int data1[][B+4],int data2[][B+4]){
	int i,j,somma1=0,somma2=0;
		for(i=0;i<A;i++){
			for(j=0;j<B;j++){
				if(*(*(data1+i)+j)==1) somma1=1;
			}
		}
		for(i=0;i<A;i++){
			for(j=0;j<B;j++){
				if(*(*(data2+i)+j)==1) somma2=1;
			}
		}
	if (somma1==0) return 2;
	if (somma2==0) return 1;
	return 0;
}
void sonda (int data[][B+4],int a1,int b1,void (*h)(int [][B+4],int,int *,int*,int*,int*,int,int)){ //a1 sono il numero di navi b1 la lunghezza
	int x,y,direzione,i,p=0,su,giu,dx,sx,min,*a,*b,*c,*d;	
	while(p!=a1){
			x=(lrand48()%A)+2; //A=10=B da 2 a 11 
			y=(lrand48()%B)+2;
			direzione=lrand48()%4; //un numero casuale per decidere che direzione prendere
			a=&su;
			b=&giu;
			c=&dx;
			d=&sx;
			min=data[x][y+1]+data[x][y-1]+data[x+1][y]+data[x-1][y]+data[x][y];
			if(min==0 && b1>1){
				h(data,b1,a,b,c,d,x,y); //fa ispezione
				if(direzione==3 && su==0){
					for(i=0;i<b1;i++){
						*(*(data+x-i)+y)=1;
					}
					p++;
				}
				if(direzione==2 && giu==0 ){
					for(i=0;i<b1;i++){
						*(*(data+x+i)+y)=1;
					}
					p++;
				}
				if(direzione==1 && dx==0){
					for(i=0;i<b1;i++){
						*(*(data+x)+y+i)=1;
					}
					p++;
				}
				if(direzione==0 && sx==0){
					for(i=0;i<b1;i++){
						*(*(data+x)+y-i)=1;
					}
					p++;
				}
			}
			if(b1==1 && min==0){
				data[x][y]=1;
				p++;
			}	
				
	}
}
void posizionamentoplus(int data[][B+4],int a1,int b1,void(*f)(int a[]),void(*g)(int [][B+4],int [][B+4]),void (*h)(int [][B+4],int,int *,int*,int*,int*,int,int)){ 
	int su,giu,dx,sx,pos,cont=0,i,x,y,xy[2],min,*a,*b,*c,*d,fake[A+4][B+4]={0};
	printf("Stai posiozionado le %d navi di lunghezza %d\n\n",a1,b1);
	while(cont!=a1){
		pos=0;
		printf("Nave numero %d di lunghezza %d\n\n",cont+1,b1);
		g(data,fake);
		f(xy); // da 2 a 11
		x=xy[0]; // VA DA 2 a 11
		y=xy[1];
		a=&su;
		b=&giu;
		c=&dx;
		d=&sx;
		min=data[x][y+1]+data[x][y-1]+data[x+1][y]+data[x-1][y]+data[x][y];
		h(data,b1,a,b,c,d,x,y); //fa ipezione
		if(b1>1 && min==0){	
				
			if(su==0) printf("C'e' dello spazio per la tua nave di lunghezza %d sopra la casella da te selezionata! Per posizionare la nave verticalmente, verso l'alto, premere 1!\n\n",b1);
			if(giu==0) printf("C'e' dello spazio per la tua nave di lunghezza %d sotto la casella da te selezionata! Per posizionare la nave verticalmente, verso il basso, premere 5!\n\n",b1);
			if(dx==0) printf("C'e' dello spazio per la tua nave di lunghezza %d a desta della casella da te selezionata! Per posizionare la nave orizzontalmente, verso destra, premere 3!\n\n",b1);
			if(sx==0) printf("C'e' dello spazio per la tua nave di lunghezza %d a sinistra della casella da te selezionata! Per posizionare la nave orizzontalmente, verso sinistra, premere 7!\n\n",b1);
			
			if(su==0 || giu==0 || dx==0 || sx==0){
				scanf("%d",&pos);
				if(pos==1){
					for(i=0;i<b1;i++){
						*(*(data+x-i)+y)=1;
					}
					cont++;
				}
				if(pos==5){
					for(i=0;i<b1;i++){
						*(*(data+x+i)+y)=1;
					}
					cont++;
				}
				if(pos==3){
					for(i=0;i<b1;i++){
						*(*(data+x)+(y+i))=1;
					}
					cont++;
				}
				if(pos==7){
					for(i=0;i<b1;i++){
						*(*(data+x)+(y-i))=1;
					}
					cont++;
				}
				printf("Posizionamento effettuato!\n\n\n");	
			}
			if(pos==0 && min==0) printf("Non c'e' abbastanza spazio nelle vicinanze per una nave cosi' grande!\n\n");
		}
		if(b1==1 && min==0){
			data[x][y]=1;
			printf("Posizionamento effettuato!\n\n\n");	
			cont++;
		}
		if(min!=0) printf("Non c'e' abbastanza spazio nelle vicinanze per una nave cosi' grande!\n\n");
		if(data[x][y]!=0) printf("Hai gia' scelto questa posizione, scegline un'altra \n\n");
	}
}		
void stampa(int data1[][B+4],int data2[][B+4]){
	int v,j;
	printf("\n\n");
	printf("\t\t\t\tLA TUA FLOTTA \t\t\t\t\t\t\t AREA NAVALE NEMICA\n\n");
	for(j=2;j<B+2;j++){
			if(j==2) printf("\t\t\t");
			printf(" %d ",j-1);
	}
	printf("\t\t");
	for(j=2;j<B+2;j++){
			if(j==2) printf("\t\t\t");
			printf(" %d ",j-1);
	}
	printf("\n\n");
	for(v=2;v<A+2;v++){ //STAMPA 10 RIGHE E 1O COLONNE!! dA 2 A 11
		for(j=2;j<B+2;j++){
			if(j==2) printf("\t\t%d\t|",v-1);
			if(data1[v][j]==1) printf("# |");
			if(data1[v][j]==2) printf("X |");
			if(data1[v][j]==0) printf("  |");
		} 
	printf("\t\t");
		for(j=2;j<B+2;j++){
			if(j==2) printf("\t\t%d\t|",v-1);
			if(data2[v][j]==2) printf("X |");
			else printf("  |");
		}
	printf("\n");
	}
}
void ispezioneintorno(int data [][B+4],int length,int *a,int*b,int*c,int*d,int x,int y){
	int i;
	*a=0; //su
	*b=0; //giu
	*c=0; //dx
	*d=0; //sx
	for(i=0;i<length+1;i++){
		*c=*c+(*(*(data+x)+(y+i))+*(*(data+x+1)+(y+i))+*(*(data+x-1)+(y+i))); 
		*d=*d+(*(*(data+x)+(y-i))+*(*(data+x+1)+(y-i))+*(*(data+x-1)+(y-i)));
		*b=*b+(*(*(data+x+i)+y)+*(*(data+x+i)+y+1)+*(*(data+x+i)+y-1));
		*a=*a+(*(*(data+x-i)+y)+*(*(data+x-i)+y+1)+*(*(data+x-i)+y-1));
	}
}
void recinto(int data1 [][B+4], int data2 [][B+4],int x){
	int i;
	for(i=0;i<A+4;i++){
		data1[i][abs(abs(-3+x))]=2;
		data2[i][abs(-3+x)]=2;
		data1[i][B+x]=2;
		data2[i][B+x]=2;
		
	}
	for(i=0;i<B+4;i++){
		data1[abs(-3+x)][i]=2;
		data2[abs(-3+x)][i]=2;
		data1[A+x][i]=2;
		data2[A+x][i]=2;
	}
	
}			
	
	
