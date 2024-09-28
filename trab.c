#include <stdio.h>
#include <math.h>
#include <fenv.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

struct fat{	//Guarda os valores dos fatoriais para evitar contas desnecessárias
	double value;		//guarda o valor do ultimo fatorial
	double last;		//guarda o valor de k+1 do ultimo k!
};

void fatorial (struct fat *num, double *count, double value){
	if (value - 1 <= 1){	//se o valor alvo (k de k!) e 1 ou 0 retorna 1
		num->last++;
		return;
	}
	
	for (long i = num->last; i < value; i++){	//calcula o fatorial a partir de onde parou ate onde quer chegar
		num->value *= i;
		(*count)++;
		num->last++;
	}

	return;
}

double formula (struct fat *num, struct fat *den, double *cont){
	*cont += num->last + 3;	//leva em consideração as multiplicacoes, adicoes e subtracoes no restante da funcao
				//Somente as que afetam o valor gerado no retorno!
	fatorial (den, cont, (2*num->last) + 2);
	fatorial (num, cont, num->last + 1);

	return (pow(2, num->last - 1) * pow (num->value, 2))/den->value;
}

double calcula_pi (double tolerancia, double *count, int mode, double *safe, double *op){
	double current, prev;
	struct fat num, den;
	
	num.value = 1;
	num.last = 0;

	den.value = 1;
	den.last = 0;

	if (mode)
		fesetround(FE_UPWARD);
	else
		fesetround(FE_DOWNWARD);

	prev = formula(&num, &den, op);
	current = formula(&num, &den, op) + prev;
	*op+= 1;
	int i = 2;
	while ((fabs(current - prev) * 2) > tolerancia){
		prev = current;
		current = formula(&num, &den, op) + prev;
		i++;
		*op+=1;
	}
	*count = i;
	*safe = 2*prev;	//guarda o penultimo valor
	*op+=1;	
	return current;
}

void printa(double f1, uint64_t f2){
	printf ("%.15e %016llX\n", f1, (unsigned long long) f2);
}

uint64_t hexify (double *aux){	//representação em hexadecimal
	return *((uint64_t*)aux);
}

int64_t transform (double val){	//torna o double em inteiro para consideração dos bits
	int64_t res;
	memcpy(&res, &val, sizeof(val));
	return res;
}

int main(){
	double tol = 0;
	double count = 0;
	double current, prev, less, aux;
	double operations = 0;

	scanf ("%lf",&tol);

	less = 2*calcula_pi (tol, &count, 0, &prev, &operations);	//arrendondamento pra baixo
	operations = 0;	//reseta a quantidade de operacoes para considerar somente as arredondadas para cima
	current = 2*calcula_pi (tol, &count, 1, &prev, &operations);

	printf ("%.0lf\n", count);	//numero de interações

	aux = current - prev;
	printa (aux, hexify(&aux));	//erro absoluto aproximado
	
	aux = fabs(current - M_PI);
	printa (aux, hexify(&aux));	//erro absoluto exato

	printa (less, hexify(&less));	//valor arredondado para baixo
	printa (current, hexify(&current));	//arredondado para cima

	printf ("%lld\n", llabs(transform(current) - transform(less) - 1));	//diferenca de ULP
	printf ("%.0f\n", operations);	//quantidade de operacoes em ponto flutuante no calculo de pi

	return 0;	
}
