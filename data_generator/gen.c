#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include <math.h>

int main()
{
	const int DataNum=pow(2,16)+1234;
	const double Ts=1e-5;
	double t=0.0;
	double ref=0.0;

	FILE *fp=fopen("data.dat","w");
	
	for (int i = 0 ; i < DataNum ; i ++) {
		ref=sin(2.0*M_PI*20*t)+3.0;
		fprintf(fp, "%lf %lf\n", t, ref);
		t+=Ts;
	}
		
	return 0;
}
