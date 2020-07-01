#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include <math.h>

int main()
{
	const int	fftSize = pow(2,16)+653;
	fftw_complex	*in, *out, *restore, *z, *envelope;
	fftw_plan	fft_plan, hilbert_plan;

	in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fftSize);
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fftSize);
	z = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fftSize);
	envelope = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fftSize);

	fft_plan = fftw_plan_dft_1d(fftSize, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	hilbert_plan = fftw_plan_dft_1d(fftSize, z, envelope, FFTW_BACKWARD , FFTW_ESTIMATE);

	const double Ts=1e-5;
	double t=0.0;
	double *ref=(double *)malloc(sizeof(double)*fftSize);

	for (int i = 0 ; i < fftSize ; i ++) {
		ref[i]=sin(2.0*M_PI*20*t)+3.0;//sin(2.0*M_PI*20*t)*exp(-5.0*t)*(1.0-cos(2.0*M_PI*2*t)) + 3.0;
		t+=Ts;
	}
	
	FILE *fa=fopen("average.dat","w");
	FILE *fp=fopen("power.dat","w");
	FILE *fe=fopen("envelope.dat","w");
		
	//Average
	double ref_sum=0;
	double average=0.0;
	for (int i = 0 ; i < fftSize ; i ++) ref_sum+=ref[i];
	average=ref_sum/(double) fftSize;
	for (int i = 0 ; i < fftSize ; i ++) fprintf(fa, "%lf %lf\n", (double)i*Ts, ref[i]-average);
	
	//RMS value (root+mean+square)
	double *ref_trim=(double *)malloc(sizeof(double)*fftSize);
	double ref_s=0.0;
	double ref_sm=0.0;
	double ref_rms=0.0;
	double ref_trim_s=0.0;
	double ref_trim_sm=0.0;
	double ref_trim_rms=0.0;
	for (int i = 0 ; i < fftSize ; i ++){
		ref_trim[i]=ref[i]-average;
		ref_s+=pow(ref[i], 2.0);
		ref_trim_s+=pow(ref_trim[i], 2.0);
	}
	ref_sm=ref_s/(double) fftSize;
	ref_trim_sm=ref_trim_s/(double) fftSize;
	ref_rms=sqrt(ref_sm);
	ref_trim_rms=sqrt(ref_trim_sm);
	printf("The RMS of a wave: %lf\n", ref_rms);
	printf("The RMS of ripple: %lf\n", ref_trim_rms);
	
	//Power spectrum
	static double power=0.0;
	for (int i = 0 ; i < fftSize ; i ++){
		in[i][0] = ref_trim[i]/fftSize;
		in[i][1] = 0.0;
	}
	
 	fftw_execute(fft_plan); 
	fftw_destroy_plan(fft_plan);
	fftw_cleanup();
	for (int i = 0 ; i < fftSize ; i ++){
		power=sqrt(out[i][0]*out[i][0]+out[i][1]*out[i][1]);
		fprintf(fp, "%lf %lf\n", (double)i/(Ts*fftSize), 20.0*log10(2*power));
	}


	//Envelope --Peak2Peak--
	for (int i = 0 ; i < fftSize ; i ++) {
		if(i==0){
			z[i][0]=out[i][0];
			z[i][1]=out[i][1];
		}
		else if(i<=fftSize/2){
			z[i][0]=2.0*out[i][0];
			z[i][1]=2.0*out[i][1];
		}
		else{
			z[i][0]=0.0;
			z[i][1]=0.0;
		}
	}
	fftw_execute(hilbert_plan); 
	fftw_destroy_plan(hilbert_plan);
	fftw_cleanup();
	
	static double envelope_amp=0.0;
	for (int i = 0 ; i < fftSize ; i ++){
		envelope_amp=sqrt(pow(envelope[i][0],2.0)+pow(envelope[i][1],2.0));
		fprintf(fe, "%lf %lf %lf %lf\n", (double)i*Ts, ref_trim[i], fabs(ref_trim[i]), envelope_amp);
	}
	
	
	free(ref);
	free(ref_trim);
	fftw_free(in);
	fftw_free(out);
	fftw_free(z);
	fftw_free(envelope);
	
	return 0;
}
