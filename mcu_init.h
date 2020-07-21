#ifndef MCU_INIT_H_
#define MCU_INIT_H_

//////////////////////////////////////////////////////////////////
//include
//////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#define	sbi(PORTX,BitX)	PORTX|= (1<<BitX)
#define	cbi(PORTX,BitX)	PORTX&=~(1<<BitX)

#define ICR_TOP	2000
#define OCR_MAX 790
#define OCR_MIN 10

//////////////////////////////////////////////////////////////////
//Functions
//////////////////////////////////////////////////////////////////
void InitIO();
void InitExtInt();

void InitTimer0();
void InitTimer1();
void InitTimer2();
void InitTimer3();

void InitADC();

int GetADC(char Channel);

void InitUart0();
void InitUart1();

void InitSPI();

void TransUart0(unsigned char data);
void TransUart1(unsigned char data);

void TransNumUart0(int num);
void SendShortUART0(int16_t num);
void TransNumUart1(int num);
void SendShortUART1(int16_t num);

unsigned char RecvUart0();
unsigned char RecvUart1();

void SPI_MasterSend(unsigned char data);
unsigned char SPI_MasterRecv(void);

//////////////////////////////////////////////////////////////////
//enum for external interrupt

//EICRA
enum{
	INT0_LOW_LEVEL = 0x00,
	INT0_RESERVED = 0x01,
	INT0_FALLING = 0x02,
	INT0_RISING = 0x03,

	INT1_LOW_LEVEL = 0x00,
	INT1_RESERVED = 0x04,
	INT1_FALLING = 0x08,
	INT1_RISING = 0x0C,
	
	INT2_LOW_LEVEL = 0x00,
	INT2_RESERVED = 0x10,
	INT2_FALLING = 0x20,
	INT2_RISING = 0x30,
	
	INT3_LOW_LEVEL = 0x00,
	INT3_RESERVED = 0x40,
	INT3_FALLING = 0x80,
	INT3_RISING = 0xC0
};

//EICRB
enum{
	INT4_LOW_LEVEL = 0x00,
	INT4_TOGGLE = 0x01,
	INT4_FALLING = 0x02,
	INT4_RISING = 0x03,
	
	INT5_LOW_LEVEL = 0x00,
	INT5_TOGGLE = 0x04,
	INT5_FALLING = 0x08,
	INT5_RISING = 0x0C,
	
	INT6_LOW_LEVEL = 0x00,
	INT6_TOGGLE = 0x10,
	INT6_FALLING = 0x20,
	INT6_RISING = 0x30,
	
	INT7_LOW_LEVEL = 0x00,
	INT7_TOGGLE = 0x40,
	INT7_FALLING = 0x80,
	INT7_RISING = 0xC0
};

//EIMSK
enum{
	INT0_ENABLE = 0x01,
	TIMER3_OVF_ENABLE = 0x04
};

//////////////////////////////////////////////////////////////////
//enum for timer0
//TCCR0
enum{
	TIMER0_WGM_NORMAL = 0x00,
	TIMER0_WGM_PWM = 0x08,
	TIMER0_WGM_CTC = 0x40,
	TIMER0_WGM_FAST_PWM	= 0x48
};
enum{
	TIMER0_COM_NORMAL = 0x00,
	TIMER0_COM_TOGGLE_OC0 = 0x10,
	TIMER0_COM_RESERVED_OC0 = 0x10,
	TIMER0_COM_CLEAR_OC0 = 0x20,
	TIMER0_COM_SET_OC0 = 0x30
};
enum{
	TIMER0_CS_NO_CLOCK_SOURCE = 0x00,
	TIMER0_CS_PRESCALER_1 = 0x01,
	TIMER0_CS_PRESCALER_8 = 0x02,
	TIMER0_CS_PRESCALER_32 = 0x03,
	TIMER0_CS_PRESCALER_64 = 0x04,
	TIMER0_CS_PRESCALER_128 = 0x05,
	TIMER0_CS_PRESCALER_256 = 0x06,
	TIMER0_CS_PRESCALER_1024 = 0x07
};

//////////////////////////////////////////////////////////////////
//enum for timer1 & timer3
//TCCR1A & TCCR3A
enum{
	TIMER13_COMA_NORMAL = 0x00,
	TIMER13_COMA_TOGGLE_OC0 = 0x40,
	TIMER13_COMA_CLEAR_OC0 = 0x80,
	TIMER13_COMA_SET_OC0 = 0xC0
};
enum{
	TIMER13_COMB_NORMAL = 0x00,
	TIMER13_COMB_TOGGLE_OC0 = 0x10,
	TIMER13_COMB_CLEAR_OC0 = 0x20,
	TIMER13_COMB_SET_OC0 = 0x30
};
enum{
	TIMER13_COMC_NORMAL = 0x00,
	TIMER13_COMC_TOGGLE_OC0 = 0x04,
	TIMER13_COMC_CLEAR_OC0 = 0x08,
	TIMER13_COMC_SET_OC0 = 0x0C
};
//TCCR1B & TCCR3B
enum{
	TIMER13_CS_NO_CLOCK_SOURCE = 0x00,
	TIMER13_CS_PRESCALER_1 = 0x01,
	TIMER13_CS_PRESCALER_8 = 0x02,
	TIMER13_CS_PRESCALER_32 = 0x03,
	TIMER13_CS_PRESCALER_64 = 0x04,
	TIMER13_CS_PRESCALER_128 = 0x05,
	TIMER13_CS_PRESCALER_256 = 0x06,
	TIMER13_CS_PRESCALER_1024 = 0x07
};

//////////////////////////////////////////////////////////////////
//enum for timer2
//TCCR2
enum{
	TIMER2_WGM_NORMAL = 0x00,
	TIMER2_WGM_PWM = 0x08,
	TIMER2_WGM_CTC = 0x40,
	TIMER2_WGM_FAST_PWM	= 0x48
};
enum{
	TIMER2_COM_NORMAL = 0x00,
	TIMER2_COM_TOGGLE_OC0 = 0x10,
	TIMER2_COM_RESERVED_OC0 = 0x10,
	TIMER2_COM_CLEAR_OC0 = 0x20,
	TIMER2_COM_SET_OC0 = 0x30
};
enum{
	TIMER2_CS_NO_CLOCK_SOURCE = 0x00,
	TIMER2_CS_PRESCALER_1 = 0x01,
	TIMER2_CS_PRESCALER_8 = 0x02,
	TIMER2_CS_PRESCALER_64 = 0x03,
	TIMER2_CS_PRESCALER_256 = 0x04,
	TIMER2_CS_PRESCALER_1024 = 0x05,
	TIMER2_CS_EXTERNAL_CLOCK_T2_FALLING = 0x06,
	TIMER2_CS_EXTERNAL_CLOCK_T2_RISING = 0x07
};

//////////////////////////////////////////////////////////////////
//enum for uart
//UCSRA
enum{
	USART_MULTI_PROCESSOR_MODE = 0x01,
	USART_DOUBLE_SPEED = 0x02,
	USART_PARITY_ERROR = 0x04,
	USART_DATA_OVERRUN = 0x08,
	USART_FRAME_ERROR = 0x10,
	USART_UDR_EMPTY = 0x20,
	USART_TRANS_COMPLETE = 0x40,
	USART_RECV_COMPLETE = 0x80
};

//UCSRB
enum{
	USART_CHAR_SIZE_9BIT_B = 0x04,
	USART_TRANS_ENABLE = 0x08,
	USART_RECV_ENABLE = 0x10,
	USART_UDR_EMPTY_INTERRUPT_ENABLE = 0x20,
	USART_TRANS_INTERRUPT_ENABLE = 0x40,
	USART_RECV_INTERRUPT_ENABLE = 0x80
};

//UCSRC
enum{
	USART_ASYNC_MODE = 0x00,
	USART_SYNC_MODE	= 0x40,
	USART_PARITY_NONE = 0x00,
	USART_PARITY_EVEN = 0x20,
	USART_PARITY_ODD = 0x30,
	USART_STOP_1BIT	= 0x00,
	USART_STOP_2BIT = 0x08,
	USART_CHAR_SIZE_5BIT = 0x00,
	USART_CHAR_SIZE_6BIT = 0x02,
	USART_CHAR_SIZE_7BIT = 0x04,
	USART_CHAR_SIZE_8BIT = 0x06,
	USART_CHAR_SIZE_9BIT_C = 0x06,
	USART_SYNC_CLOCK_RISING = 0x00,
	USART_SYNC_CLOCK_FALLING = 0x01
};

//UBRRL
enum{
	USART_9600BPS = 103,
	USART_115200BPS = 8
};


//////////////////////////////////////////////////////////////////
//enum for adc
//ADMUX
enum{
	ADC_REF_SELECTION_AREF = 0x00,
	ADC_REF_SELECTION_AVCC = 0x04,
	ADC_REF_SELECTION_INTERNAL = 0xC,
	ADC_ADJUST_RIGHT = 0x00,
	ADC_ADJUST_LEFT = 0x20
};
enum{
	ADC_MUX_CH0 = 0x00,
	ADC_MUX_CH1 = 0x01,
	ADC_MUX_CH2 = 0x02,
	ADC_MUX_CH3 = 0x03,
	ADC_MUX_CH4 = 0x04,
	ADC_MUX_CH5 = 0x05,
	ADC_MUX_CH6 = 0x06,
	ADC_MUX_CH7 = 0x07
};

//ADCSRA
enum{
	ADC_INTERRUPT_ENABLE = 0x08,
	ADC_FREE_RUNNING = 0x20,
	ADC_CONVERTION_START = 0x40,
	ADC_ENABLE = 0x80
};
enum{
	ADC_PRESCALER_2 = 0x01,
	ADC_PRESCALER_4 = 0x02,
	ADC_PRESCALER_8 = 0x03,
	ADC_PRESCALER_16 = 0x04,
	ADC_PRESCALER_32 = 0x05,
	ADC_PRESCALER_64 = 0x06,
	ADC_PRESCALER_128 = 0x07
};

#endif /* MCU_INIT_H_ */
