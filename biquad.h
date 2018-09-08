#define FLT_MIN_PLUS 1
#define FLT_MIN_MINUS -1
typedef struct {
  float m_f_Xz_1; // x z-1 delay element
  	float m_f_Xz_2; // x z-2 delay element
  	float m_f_Yz_1; // y z-1 delay element
  	float m_f_Yz_2;

  float a0;
  float a1;
  float a2;
  float b1;
  float b2;

} Biquad;

void biquadclear(Biquad* bi) {
  bi->a0 =0;
  bi->a1 = 0;
  bi->a2 = 0;
  bi->b1 = 0;
  bi->b2 = 0;
  bi->m_f_Xz_1=0; // x z-1 delay element
  	bi->m_f_Xz_2=0; // x z-2 delay element
  	bi->m_f_Yz_1=0; // y z-1 delay element
  	bi->m_f_Yz_2=0;

}
float dobiquad(float f_xn, Biquad* bi) {

//   float out = in * bi->a0 + bi->z1;
//     bi->z1 = in * bi->a1 + bi->z2 - bi->b1 * out;
//     bi->z2 = in * bi->a2 - bi->b2 * out;
// return out;
float yn = bi->a0*f_xn + bi->a1*bi->m_f_Xz_1 +bi->a2*bi->m_f_Xz_2 - bi->b1*bi->m_f_Yz_1 - bi->b2*bi->m_f_Yz_2;

bi->m_f_Yz_2 = bi->m_f_Yz_1;
		bi->m_f_Yz_1 = yn;

		// X delays
		bi->m_f_Xz_2 = bi->m_f_Xz_1;
		bi->m_f_Xz_1 = f_xn;

		return  yn;
}
calculateCutoffMassberg(Biquad* bi1,Biquad* bi2,float m_nSampleRate,float fCutoffFreq,float fQ) {
  float theta_c = 2.0 * pi*fCutoffFreq /(float) m_nSampleRate;
	//printf("testing");
	float sigma_s = 0;
	float g1 = 2.0/(sqrt(pow((2.0-(pow((sqrt(2.0)*pi)/theta_c,2))),2)+(pow((2.0*pi)/(fQ*theta_c),2))));

	if (fQ > sqrt(0.5)) {
		float gr = (2.0 * pow(fQ, 2))/sqrt(4*pow(fQ,2)-1);
		float wr = theta_c * sqrt(1.0 - (1.0 / (2.0 * pow(fQ, 2))));
		float sigmar = tan(wr / 2.0);
		sigma_s = sigmar * (pow((pow(gr, 2) - pow(g1, 2)) / (pow(gr, 2) - 1),1/4));

	}
	else {
		float wm = (theta_c*(sqrt(((2.0 - (1.0 / pow(fQ, 2))+  sqrt((1.0 - (4.0 * pow(fQ, 2))) / pow(fQ, 4) + (4.0 / g1))) / 2.0))));

		float sigma_m = tan((wm / 2.0));

		sigma_s = theta_c * pow((1.0 - pow(g1, 2)), 1.0 / 4.0) / 2;

		if (sigma_m < sigma_s)
			sigma_s = sigma_m;

	}

	float wp = 2.0 * atan(sigma_s);
	float wz = 2.0 * atan(sigma_s / sqrt(g1));
	float gp = 1.0 / sqrt(pow((1.0 - pow((wp / theta_c), 3)),2) + (pow(wp / (fQ*theta_c), 2)));
	float gz = 1.0 / sqrt(pow((1.0 - pow((wz / theta_c), 2)),2) + pow(wz / (fQ*theta_c), 2));
	float qp = sqrt((g1*(pow(gp, 2) - pow(gz, 2))) / ((g1 + pow(gz, 2))*pow(g1 - 1.0, 2)));
	float qz = sqrt((g1*(pow(gp, 2) - pow(gz, 2))) / (pow(gz,2)*(g1 + pow(gp, 2))*pow(g1 - 1.0, 2)));

	float y0 = pow(sigma_s, 2) + ((1.0 / qp)*sigma_s) + 1.0;
	float a0 = pow(sigma_s, 2) + (sqrt(g1) / qz)*sigma_s + g1;
	float a1 = 2.0 * (pow(sigma_s, 2) - g1);
	float b1 = 2.0 * (pow(sigma_s, 2) - 1);
	float a2 = pow(sigma_s, 2) - (sqrt(g1) / qz)*sigma_s + g1;
	float b2 = pow(sigma_s, 2) - (1.0 / qp)*sigma_s + 1.0;

	bi1->a0 = a0/y0;
	bi1->a1 =a1/y0;
	bi1->a2 = a2/y0;
	bi1->b1 = b1/y0;
	bi1->b2 = b2/y0;
	bi2->a0 = a0/y0;
	bi2->a1 =a1/y0;
	bi2->a2 =a2/y0;
	bi2->b1 = b1/y0;
	bi2->b2 = b2/y0;
}
float calculateCutoffs(Biquad* bi1,Biquad* bi2, float m_nSampleRate, float fCutoffFreq, float fQ) {
float theta_c = 2.0*pi*fCutoffFreq / (float)m_nSampleRate;
float d = 1.0 / fQ;

float fBetaNumerator = 1.0 - ((d / 2.0)*(sin(theta_c)));
float fBetaDenominator = 1.0 + ((d / 2.0)*(sin(theta_c)));

float fBeta = 0.5*(fBetaNumerator / fBetaDenominator);

float fGamma = (0.5 + fBeta)*(cos(theta_c));
float fAlpha = (0.5 + fBeta - fGamma) / 2.0;

bi1->a0 = fAlpha;
bi1->a1 = 2.0*fAlpha;
bi1->a2 = fAlpha;
bi1->b1 = -2.0*fGamma;
bi1->b2 = 2.0*fBeta;
bi2->a0 = fAlpha;
bi2->a1 = 2.0*fAlpha;
bi2->a2 = fAlpha;
bi2->b1 = -2.0*fGamma;
bi2->b2 = 2.0*fBeta;
}
