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
/*
#define F_CPU 16000000UL

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

//err 잡는 변수들
#define Kp 0.4
#define Kd 0
#define MOTER_SPEED 800 // 초기 모터 스피드
int pre_err = 0; // 이전 에러
int sum = 0;
int mode = 1; // 모드설정 - 스위치로 조절할 수있다.

              //센서 정규화를 위한 배열들
volatile double senserMin[8] = { 0, }; // 최소값 배열
volatile double senserMax[8] = { 0, }; // 최대값 배열
volatile double senseGeneral[8] = { 0, }; // 정규화 배열
volatile double weight[8] = { -8, -4, -2.142, -1, 1.1, 2.25, 4.2, 8 }; // 가중치 배열

                                                                       //초기값 설정 함수들(LED, INTERRUPT, ADC, UART, TIMER/COUNTER
void io_Init();
void interrupt_Init();
void adc_Init();
void uart_Init();
void timerCounter_Init();

//제어를 위한 함수들(uart, adc, motor)
void Uart_Transmit(unsigned char data);
//unsigned char Uart_Receive(); //사용 안함
void Uart_TransNum(int NumData);// 숫자를 문자로 변형후 Uart_Transmit 함수 실행
                                //void uart_Control(); //uart제어 함수
void motor_Control(); //모터제어 함수

ISR(INT0_vect) // 최대 최소 배열 생성, 모드 : 대기
{
    mode = 1;
}

ISR(INT1_vect) // 모드 : 작동
{
    mode = 2;
}

SIGNAL(SIG_OVERFLOW3) // 제어주기를 위한 오버플로 인터럽트
{
    TCNT3 = 64285; // 초기값(bottom비스무리) 설정
    if (mode == 1)
    {

        for (int Adc_input = 0; Adc_input<8; Adc_input++) // adc 0~7번 다 사용
        {
            ADMUX = (Adc_input | 0b01000000);

            ADCSRA |= (1 << ADSC);
            while (!(ADCSRA & (1 << ADIF)));

            if (ADC<400)
                PORTA = ~(0x00 | (1 << Adc_input));

            if (senserMax[Adc_input] <ADC)
                senserMax[Adc_input] = ADC;

            if (senserMin[Adc_input] >ADC)
                senserMin[Adc_input] = ADC;
        }

        OCR1A = 0;
        OCR1B = 0;
    }

    else if (mode == 2)
    {
        PORTA = 0xff;

        //정규화 시작
        for (int i = 0; i<8; i++)
        {
            ADMUX = (i | 0b01000000);

            ADCSRA |= (1 << ADSC);
            while (!(ADCSRA & (1 << ADIF)));

            if (ADC<400)
                PORTA = ~(0x00 | (1 << i));

            senseGeneral[i] = (ADC - senserMin[i]) / (senserMax[i] - senserMin[i]) * 100; // 정규화 공식
            senseGeneral[i] = senseGeneral[i] * weight[i]; // 가중치 대입

            Uart_TransNum(senseGeneral[i]);

        }



        for (int i = 0; i<8; i++) // 가중치 총합
        {
            sum += senseGeneral[i];
        }


        int err = sum; // 오차 = 목표치-현재값
        int Kp_term = err * Kp; //p항=Kp*오차


        int D_err = (err - pre_err);
        int Kd_term = Kd * D_err;

        pre_err = err; // 이전 에러 저장(D제어를 위해서)

        Uart_Transmit('/');
        Uart_Transmit('P');
        Uart_Transmit(':');
        Uart_TransNum(Kp_term);

        Uart_Transmit('/');
        Uart_Transmit('D');
        Uart_Transmit(':');
        Uart_TransNum(Kd_term); // 통신용

        int sum_err = Kp_term + Kd_term;

        //OCR 에러를 방지 하기 위한 소스
        if (sum_err > MOTER_SPEED - 30)
        {
            sum_err = MOTER_SPEED - 30;
        }
        else if (-sum_err > MOTER_SPEED - 30)
        {
            sum_err = -(MOTER_SPEED - 30);
        }

        OCR1A = MOTER_SPEED -200 - sum_err;
        OCR1B = MOTER_SPEED + sum_err;

        Uart_Transmit('/');
        Uart_Transmit('L');
        Uart_Transmit(':');
        Uart_TransNum(OCR1A);

        Uart_Transmit('/');
        Uart_Transmit('R');
        Uart_Transmit(':');
        Uart_TransNum(OCR1B);

        Uart_Transmit('\n');
        Uart_Transmit('\r');
    }

    sum = 0;
}


//메인문
int main(void)
{
    io_Init();
    interrupt_Init();
    adc_Init();
    uart_Init();
    timerCounter_Init();

    motor_Control();



    while (1);

    return 0;
}

/////////////////////////////////////////////////////////////////////////

//초기값 설정 함수들(LED, INTERRUPT, ADC, UART, TIMER/COUNTER
void io_Init()
{
    DDRA = 0xff; // LED 활성화
    PORTA = 0xff; // LED 꺼놓기

    DDRD = 0x00; // 스위치 활성화
    DDRF = 0x00; // ADC 활성화

    DDRB = 0b01100000; // PWM설정


    DDRC = 0b00000011; //모터 방향 설정
    PORTC = 0b00000011;

}

void interrupt_Init()
{
    SREG = 0x80; // 전체 인터럽트 활성화

    EIMSK = 0b00000011; // 0번, 1번(스위치) 인터럽트 활성화
    EICRA = 0b00001010; // 0번, 1번(스위치) falling edge로 활성화
}

void adc_Init()
{
    ADMUX = (1 << REFS1) | (1 << REFS0) | (0 << ADLAR); // 내부 2.56V를 기준으로 사용, 우측정렬
    ADCSRA = (1 << ADEN) | (0 << ADFR) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // ADC 활성화, 단일변환모드, 분주비 : 128
}

void uart_Init()
{
    UCSR0A = 0x00;
    UCSR0B = 0b00011000;
    UCSR0C = 0b00000110;

    UBRR0H = 0;
    UBRR0L = 103;
}

void timerCounter_Init()
{
    //1번핀 모터제어로 사용
    TCCR1A = (1 << COM1A1) | (0 << COM1A0) | (1 << COM1B1) | (0 << COM1B0) | (1 << WGM11) | (0 << WGM10); // A,B Channel Clear(Compare Match) & Set(OVerflow) ,fast PWM모트
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (0 << CS11) | (1 << CS10); // fast PWM모드 설정

                                                                                    //3번핀 제어주기로 사용
    TCCR3A = (0 << WGM31) | (0 << WGM30);  // Normal Mode, prescale 256
    TCCR3B = (0 << WGM33) | (0 << WGM32) | (1 << CS32) | (0 << CS31) | (0 << CS30); // 분주비 256
    ETIMSK = (1 << TOIE3);  // 3번 Overflow Interrupt Enable
    TCNT3 = 64285;
}

/////////////////////////////////////////////////////////////////////////

//제어를 위한 함수들(uart, adc, motor)
void Uart_Transmit(unsigned char data) // atmega에서 pc로 전송
{
    while (!(UCSR1A & (1 << UDRE1)));
    UDR1 = data;
}
/*
unsigned char Uart_Receive() // pc에서 atmega로 전송
{
while (!(UCSR1A & (1<<RXC1)));
return UDR1;
}
*/
void Uart_TransNum(int NumData) // 숫자를 문자로 변환해서 출력해주는 함수
{
    int TempData;

    if (NumData < 0)
    {
        Uart_Transmit('-');
        NumData = -NumData;
    }


    TempData = NumData / 10000;
    Uart_Transmit(TempData + 48);

    TempData = (NumData % 10000) / 1000;
    Uart_Transmit(TempData + 48);

    TempData = (NumData % 1000) / 100;
    Uart_Transmit(TempData + 48);

    TempData = (NumData % 100) / 10;
    Uart_Transmit(TempData + 48);

    TempData = NumData % 10;
    Uart_Transmit(TempData + 48);

    Uart_Transmit(',');
    Uart_Transmit(' ');
}
/*
//uart제어 함수 - ADC값을 받는다.//
void uart_Control()
{
while(1)
{
for(int Adc_input= 0; Adc_input<8; Adc_input++) // adc 0~7번 다 사용
{
ADMUX = (Adc_input | 0b01000000);

ADCSRA |= (1<<ADSC);
while(!(ADCSRA & (1<<ADIF)));

if(ADC>500)
PORTA = 0x0f;
else if(ADC<500)
PORTA = 0xf0;

Uart_TransNum(ADC);

_delay_ms(500);
}

Uart_Transmit('\n');
Uart_Transmit('\r');

_delay_ms(500);

}
}
*/
//모터제어 함수//
void motor_Control()
{
    ICR1 = 2000;
    OCR1A = 0;
    OCR1B = 0;
}

/////////////////////////////////////////////////////////////////////////

*/
