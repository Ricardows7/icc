#include <stdio.h>
#include <math.h>
#include <fenv.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

struct fat{
	long double value;
	long last;
};

long double fatorial (struct fat *num, double *count, int mode, long double value){
	long double n = 1;

	if ((mode && num->last <= 1) || value <= 1)
		return 1;
	
	if (mode){
		num->value *= num->last;
		(*count)++;
		//printf ("O FATORIAL MODO 1 E: %Lf, %ld\n", num->value, num->last);
	}
	else{
		for (int i = 2; i <= value; i++){
			n *= i;
			(*count)++;
		}
		//printf ("O FATORIAL MODO 2 E: %Lf\n", n);
	}	
	return n;
}

long double formula (struct fat *num, double *cont){
	long double den = fatorial (NULL, cont, 0, (2*(num->last))+1); 
	*cont += num->last + 4;
	fatorial (num, cont, 1, 100);

	return (pow(2, num->last) * pow (num->value, 2))/den;
}

long double calcula_pi (double tolerancia, double *count, int mode, double *safe, double *op){
	double current, prev;
	struct fat molde;
	
	molde.value = 1;
	molde.last = 0;

	if (mode)
		fesetround(FE_UPWARD);
	else
		fesetround(FE_DOWNWARD);

	prev = formula(&molde, op);
	molde.last++;
	current = formula(&molde, op) + prev;
	*op+= 1;
	//printf ("Comecamos com %lf e %lf!\n", prev, current);
	int i = 2;
	while ((fabs(current - prev) * 2) > tolerancia){
		molde.last++;
		prev = current;
		current = formula(&molde, op) + prev;
		//printf ("O valor por enquanto e: %.15lf\n", current);
		i++;
		*op+=1;
	}
	*count = i;
	*safe = 2*prev;
	*op+=1;	//multiplicacao por 2 no final	
	return current;
}

void printa(double f1, uint64_t f2){
	printf ("%.15e %016llX\n", f1, (unsigned long long) f2);
}

uint64_t hexify (double *aux){
	return *((uint64_t*)aux);
}

int64_t transform (double val){
	int64_t res;
	memcpy(&res, &val, sizeof(val));
	//printf ("o valor no transform e: %ld\n", res);
	return res;
}

int main(){
	double tol = 0;
	double count = 0;
	double current, prev, less, aux;
	double operations = 0;
	scanf ("%lf",&tol);

	less = 2*calcula_pi (tol, &count, 0, &prev, &operations);
	fesetround(FE_UPWARD);
	operations = 0;
	current = 2*calcula_pi (tol, &count, 1, &prev, &operations);

	printf ("%.0lf\n", count);

	aux = current - prev;
	printa (aux, hexify(&aux));
	
	/*printf ("%.15lf %.15lf\n", current, M_PI);
	double holder = M_PI;
	printf("%016llX\n", (unsigned long long) hexify(&holder));*/

	aux = fabs(current - M_PI);
	printa (aux, hexify(&aux));

	printa (less, hexify(&less));
	printa (current, hexify(&current));

	printf ("%lld\n", llabs(transform(current) - transform(less) - 1));
	printf ("%.0f\n", operations);
	return 0;	
}
