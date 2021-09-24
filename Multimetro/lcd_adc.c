#include <16f877a.h>								//Inclui biblioteca do microcontrolador 16f877a
#device ADC = 10//									//Inicia ADC com 2^10 = 1024-1 10 bits
#fuses xt,nowdt,noprotect,put,brownout				//Configura��o dos fus�veis
#fuses nolvp,nocpd,nowrt 							//Configura��o dos fus�veis
#use delay(clock=4000000)							//Usa crystal de 4MHz
#include <lcd2.c>									//Inclui a biblioteca do LCD 
#byte porta = 0x05									//Diz que o port_a ficara no espa�o 0x05 da memoria
#byte portb = 0x06									//Diz que o port_b ficara no espa�o 0x06 da memoria
#byte portc = 0x07									//Diz que o port_c ficara no espa�o 0x07 da memoria
#byte portd = 0x08									//Diz que o port_d ficara no espa�o 0x08 da memoria
#byte porte = 0x09									//Diz que o port_e ficara no espa�o 0x09 da memoria
int ohm [8]={										//Cria��o da Variavel para criar um caractere novo
	0b00000,
	0b00000,
	0b01110,
	0b11011,
	0b10001,
	0b01010,
	0b11011,
	0b00000
};
float a,b,c,d,e,l,inf,sup;							//Declara��o das variaveis globais do tipo ponto flutuante
void conta (void);									//Indica que ir� ter uma fun��o com retorno 0 com o nome conta
void main()
{
	int f;											//Cria uma variavel do tipo inteiro com o nome 'f'
	setup_adc_ports(AN0);							//Diz que ir� utiliar o pino AN0 como ADC (pin_a0)
	setup_adc (ADC_CLOCK_INTERNAL);					//Informa que ir� utilizar clock interno para o adc
	set_adc_channel(0);								//Inicia o Ad no canal 0
	set_tris_a(0xff);								//Configura todos os pinos do port_a como entrada
	set_tris_b(0x00);								//Configura todos os pinos do port_b como entrada
	set_tris_c(0xff);								//Configura todos os pinos do port_c como entrada
	set_tris_d(0x08);								//Configura todos os pinos do port_d como sa�da
	set_tris_e(0xff);								//Configura todos os pinos do port_e como entrada
	for(f=0;f<=6;f++)								//La�o de inicializa��o do programa
	{
		output_toggle(pin_b7);						//Alterna o valor da saida do pino b7
		delay_ms(150);								//Espera 150 miliSegundos
	}
	output_low(pin_b7);								//Deixa 0 o valor da sa�da do pino b7
	lcd_init();										//Inicia LCD
	lcd_set_cgram_char(0,ohm);						//Fun��o do LCD para usar memoria especial CGRAM - caractere generation ram para armazenar at� 8 carcteres especiais (0 a 7)
	//salvo na posi��o 0 da ram, com o nome ohm da variavel
	while (true)
	{	
		conta();									//Vai para fun��o conta

		printf(lcd_putc,"\f%1.2f V",a);				// /f limpa tela /r da um tab, %1.2f mostra valor com duas casas depois da virgula
		lcd_gotoxy(1,2);						  	// vai para linha 2, no espa�o 1
		printf(lcd_putc,"%1.0f %c",e,0);	  		// /r da um tab, %1.2f mostra valor com duas casas depois da virgula
		lcd_gotoxy(21,1);						  	// vai para linha 2, no espa�o 1
		printf(lcd_putc,"%1.2f mA",c);				// /f limpa tela %1.2f mostra valor com duas casas depois da virgula
		lcd_gotoxy(21,2);							// vai para linha 2, no espa�o 1
		printf(lcd_putc,"%1.2f mW",b);				// /r da um tab, %1.2f mostra valor com duas casas depois da virgula
		if(a>2.0)									//Se o valor de tens�o for maior que 2.0
		{
			output_high(pin_b7);					//Ent�o liga a saida do pino b7
		}
		if(a<2.0)									//Se o valor de tens�o for menor que 2.0
		{
			output_low(pin_b7);						//Ent�o desliga a saida do pino b7
		}
		while(l<sup&&l>inf)
		{
			l = read_adc();							//Le o valor do AD enquanto for menor que o sup e maior que o inferior
		}
	}	
}
void conta ()
{
		l = read_adc();								//Le o valor do ADC
		a = 5.0 * (l/1023.0);						//variavel tipo float, tem 1023 possibilidade, 5 volts / 1023 = racional 4,88 mV cada
		d = 5.0 - a;								//5 volts (tens�o total) - (tens�o lida pelo adc, atribuido na variavel  
		e = (10000.0*a)/d;							//10000 valor nominal do resistor * tens�o na variavel / tens�o no outro resistor (regra de 3)	
		c = (5.0 / (e+10000.0)) * 1000.0;			//(valor da tens�o (a) / valor da resistencia (e)) * 1000 para ficar na faixa de mili 10e-�
		b = c * a ;									//valor da corrente (c) * valor da tens�o (a)
		inf = l - 5;								//valor do AD - 5, para ser valor inferior
		sup = l + 5;								//valor do AD + 5, para ser valor superior
}