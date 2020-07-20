#include "mcu_init.h"

//////////////////////////////////////////////////////////////////
//InitIO()
//Initialize Input & Output of Port
// Input : None
// Output : None
//////////////////////////////////////////////////////////////////
void InitIO(){
	//TO DO
	DDRA = 0xFF;	//0b 1111 1111	//LED
	DDRD = 0x00;	//0b 0000 1000	//SWITCH
	DDRB = 0x60;	//0b 0110 0000  //PWM
	DDRC = 0x03;	//0b 0001 1010	//Direction
	DDRF = 0x00;	//0b 0000 0000	//ADC
	
	PORTA = 0xff;
	PORTB = 0x07;
	PORTC = 0x03;
}

//////////////////////////////////////////////////////////////////
//InitExtInt()
//Initialize External Interrupt
// Input : None
// Output : None
//////////////////////////////////////////////////////////////////
void InitExtInt(){
	//TO DO
	cli();
	EICRA = INT1_FALLING | INT0_FALLING;
	EIMSK = TIMER3_OVF_ENABLE;
	sei();
}

//////////////////////////////////////////////////////////////////
//InitTimer0()
//Initialize Timer0
// Input : None
// Output : None
//////////////////////////////////////////////////////////////////
void InitTimer0(){
	//TO DO
}

//////////////////////////////////////////////////////////////////
//InitTimer1()
//Initialize Timer1 20kHz
// Input : None
// Output : None
//////////////////////////////////////////////////////////////////
void InitTimer1(){
	//TO DO
	TCCR1A = TIMER13_COMA_CLEAR_OC0 | TIMER13_COMB_CLEAR_OC0 | (1 << WGM11) | (0 << WGM10);	
	TCCR1B = (1 << WGM13) | (1 << WGM12) | TIMER13_CS_PRESCALER_1;
	ICR1 = ICR_TOP;
	OCR1C = 0;
	OCR1A = 0;
	OCR1B = 0;
	TCNT1 = 0;
}

//////////////////////////////////////////////////////////////////
//InitTimer2()
//Initialize Timer2
// Input : None
// Output : None
//////////////////////////////////////////////////////////////////
void InitTimer2(){
	//TO DO
}

//////////////////////////////////////////////////////////////////
//InitTimer3()
//Initialize Timer3
// Input : None
// Output : None
//////////////////////////////////////////////////////////////////
void InitTimer3(){
	//TO DO
	TCCR3B = TIMER13_CS_PRESCALER_256;
	ETIMSK = TIMER3_OVF_ENABLE;
	TCNT3 = 65036;
}

//////////////////////////////////////////////////////////////////
//InitADC()
//InitADC
// Input : None
// Output : None
//////////////////////////////////////////////////////////////////
void InitADC(){
	//TO DO
	ADMUX = ADC_REF_SELECTION_INTERNAL | ADC_ADJUST_RIGHT;
	ADCSRA = ADC_ENABLE | ADC_PRESCALER_128;
}

//////////////////////////////////////////////////////////////////
//GetADC()
//GetADC
// Input : adc chanel
// Output : ADC
//////////////////////////////////////////////////////////////////
int GetADC(char Channel){
	ADMUX = (0xF0) | Channel;
	ADCSRA |= 0x40;
	while(!(ADCSRA & 0x10));
	return ADC;
}

//////////////////////////////////////////////////////////////////
//InitUart0()
//InitUart0
// Input : None
// Output : None
//////////////////////////////////////////////////////////////////
void InitUart0(){
	//TO DO
}

//////////////////////////////////////////////////////////////////
//InitUart1()
//InitUart1
// Input : None
// Output : None
//////////////////////////////////////////////////////////////////
void InitUart1(){
	//TO DO
	UCSR1A = 0x00;
	UCSR1B = USART_RECV_ENABLE | USART_TRANS_ENABLE;
	UCSR1C = USART_CHAR_SIZE_8BIT;
	UBRR1L = USART_9600BPS;
}
//////////////////////////////////////////////////////////////////
//InitSPI()
//InitSPI
// Input : None
// Output : None
//////////////////////////////////////////////////////////////////
void InitSPI(){
	SPCR = 0x50;
	SPSR = 0x01;
}

//////////////////////////////////////////////////////////////////
//TransUart0()
//TransUart0
// Input : Transmit data
// Output : None
//////////////////////////////////////////////////////////////////
void TransUart0(unsigned char data){
	//TO DO
	while(!(UCSR0A & 0x20));
	UDR0 = data;
}

//////////////////////////////////////////////////////////////////
//TransUart1()
//TransUart1
// Input : Transmit data
// Output : None
//////////////////////////////////////////////////////////////////
void TransUart1(unsigned char data){
	//TO DO
	while(!(UCSR1A & 0x20));
	UDR1 = data;
}

//////////////////////////////////////////////////////////////////
//RecvUart0()
//RecvUart0
// Input : None
// Output : Recved Data
//////////////////////////////////////////////////////////////////
unsigned char RecvUart0(){
	//TO DO
	return UDR0;
}

//////////////////////////////////////////////////////////////////
//RecvUart1()
//RecvUart1
// Input : None
// Output : Recved Data
//////////////////////////////////////////////////////////////////
unsigned char RecvUart1(){
	//TO DO
	return UDR1;
}

//////////////////////////////////////////////////////////////////
//TransNumUart0()
//TransNumUart0
// Input : Number data
// Output : None
//////////////////////////////////////////////////////////////////
void TransNumUart0(int num){
	//TO DO
	if(num < 0){
		TransUart0('-');
		num *= -1;
	}
	TransUart0( ((num%10000000) / 1000000) + 48);
	TransUart0( ((num%1000000) / 100000) + 48);
	TransUart0( ((num%100000) / 10000) + 48);
	TransUart0( ((num%10000) / 1000) + 48);
	TransUart0( ((num%1000) / 100) + 48);
	TransUart0( ((num%100) / 10) + 48);
	TransUart0(   num%10 + 48 );
}

//////////////////////////////////////////////////////////////////
//SendShortUART0()
//SendShortUART0
// Input : Number data
// Output : None
//////////////////////////////////////////////////////////////////
void SendShortUART0(int16_t num){
	if(num < 0){
		TransUart0('-');
		num *= -1;
	}
	TransUart0( ((num%100000) / 10000) + 48);
	TransUart0( ((num%10000) / 1000) + 48);
	TransUart0( ((num%1000) / 100) + 48);
	TransUart0( ((num%100) / 10) + 48);
	TransUart0(   num%10 + 48 );
}

//////////////////////////////////////////////////////////////////
//TransNumUart1()
//TransNumUart1
// Input : Number data
// Output : None
//////////////////////////////////////////////////////////////////
void TransNumUart1(int num){
	//TO DO
	if(num < 0){
		TransUart1('-');
		num *= -1;
	}
	TransUart1( ((num%10000000) / 1000000) + 48);
	TransUart1( ((num%1000000) / 100000) + 48);
	TransUart1( ((num%100000) / 10000) + 48);
	TransUart1( ((num%10000) / 1000) + 48);
	TransUart1( ((num%1000) / 100) + 48);
	TransUart1( ((num%100) / 10) + 48);
	TransUart1(   num%10 + 48 );
}

//////////////////////////////////////////////////////////////////
//SendShortUART1()
//SendShortUART1
// Input : Number data
// Output : None
//////////////////////////////////////////////////////////////////
void SendShortUART1(int16_t num){
	if(num < 0){
		TransUart1('-');
		num *= -1;
	}
	TransUart1( ((num%100000) / 10000) + 48);
	TransUart1( ((num%10000) / 1000) + 48);
	TransUart1( ((num%1000) / 100) + 48);
	TransUart1( ((num%100) / 10) + 48);
	TransUart1(   num%10 + 48 );
}

//////////////////////////////////////////////////////////////////
//SPI_MasterSend()
//SPI_MasterSend
// Input : data
// Output : None
//////////////////////////////////////////////////////////////////
void SPI_MasterSend(unsigned char data){
	SPDR = data;
	while (!(SPSR & 0x80));
	data = SPDR;
}

//////////////////////////////////////////////////////////////////
//SPI_MasterSend()
//SPI_MasterSend
// Input : None
// Output : data
//////////////////////////////////////////////////////////////////
unsigned char SPI_MasterRecv(void)
{
	SPDR = 0x00;
	while (!(SPSR & 0x80));
	return SPDR;
}