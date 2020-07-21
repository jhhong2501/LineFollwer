#include "mcu_init.h"

#define Kp 0.4
#define Ki 0
#define Kd 0

#define VEL_M 700

volatile int err	 = 0;
volatile int pre_err = 0;
volatile int sum_err = 0;
volatile int sum	 = 0;
volatile int t		 = 0.02;

volatile int mode = 1; 
volatile int cnt  = 0;
volatile int Stop_cnt = 0;

volatile double	ADC_Data[8]		 = { 0, };
volatile double ADC_MIN[8]		 = { 1023, };
volatile double ADC_MAX[8]		 = { 0, };
volatile double ADC_Normalize[8] = { 0, };
volatile double ADC_Weight[8]	 = { -8, -4, -2.142, -1, 1.1, 2.25, 4.2, 8 };

void Save_ADC();

ISR(INT0_vect){
	mode = 1;
	PORTA = 0x0F;
}

ISR(INT1_vect){
	mode = 2;
	PORTA = 0xF0;
}

ISR(TIMER3_OVF_vect) {
	TCNT3 = 64285;
	sum = 0;
	if (mode == 1){
		Save_ADC();
		OCR1A = 0;
		OCR1B = 0;
	}
	else if (mode == 2){
		for (int i = 0; i<8; i++){
			ADC_Data[i] = GetADC(i);
			if (ADC_Data[i] < 400)
			PORTA = ~(0x00 | (1 << i));
			ADC_Normalize[i] = (ADC_Data[i] - ADC_MIN[i]) / (ADC_MAX[i] - ADC_MIN[i]) * 100;
			ADC_Normalize[i] = ADC_Normalize[i] * ADC_Weight[i];
			sum += ADC_Normalize[i];
		}

		err = sum;
		pre_err = err;
		sum_err = Kp*err + Ki*err*t + Kd*(err - pre_err)/t;
		
		if (sum_err > VEL_M - 30){
			sum_err = VEL_M - 30;
		}
		else if (-sum_err > VEL_M - 30){
			sum_err = -(VEL_M - 30);
		}
		OCR1A = VEL_M - sum_err;
		OCR1B = VEL_M + sum_err;
		
		if ((ADC_Data[0] == 1) && (ADC_Data[7] == 1)){
			Stop_cnt++;
		}
		if (Stop_cnt == 90){
			OCR1A = 0;
			OCR1B = 0;
			for(int j = 0; j<8;j++){
				PORTA = ~(0x00 | (1 << j));				
			}
		}
	}
}

int main(void){
	void InitIO();
	void InitExtInt();
	void InitTimer1();
	void InitTimer3();
	void InitADC();
	void InitUart1();

	while (1);
	return 0;
}

void Save_ADC(){
	for (int ch=0;ch<8;ch++){
		GetADC(ch);
		if (ADC < 400)
		cbi(PORTA, 1);
		else
		sbi(PORTA, 1);

		if (ADC_MAX[ch] < ADC)
		ADC_MAX[ch] = ADC;

		if (ADC_MIN[ch] > ADC)
		ADC_MIN[ch] = ADC;
	}
}
