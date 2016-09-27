#include <stdio.h>

struct point{
    double spazio.x;
    double y;
};
typedef struct point punto;
punto *somma(punto,punto);
punto *scrittura();
void main(){
    punto a,b,c,*d;
    a=*scrittura();
    b=*scrittura();
    c=*somma(a,b);
    printf("\nIl punto somma ha ascissa %lf e ordinata %lf\n",c.spazio.x,c.y);
    d=&a;
    printf("Il primo punto inserito ha ascissa %lf e ordinata %lf",(*d).spazio.x,(*d).y);
}
punto *scrittura(){
	static punto a;
    printf("Inserire ascissa punto:\t");
    scanf("%lf",&a.spazio.x);
    printf("Inserire ordinata punto:\t");
    scanf("%lf",&a.y);
    return &a;
}
punto *somma(punto a,punto b){
    static punto c;
    c.spazio.x=a.spazio.x+b.spazio.x;
    c.y=a.y+b.y;
    return &c;
}

