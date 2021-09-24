#include <16f877a.h>								// Inclui biblioteca do microcontrolador 16F877A
#device ADC = 10//									// Inicia ADC com 2^10 = 1024-1 10 bits
#fuses xt,nowdt,noprotect,put,brownout				// Configuração dos fusíveis
#fuses nolvp,nocpd,nowrt 							// Configuração dos fusíveis
#use delay(clock=4000000)							// Usa crystal de 4MHz
#include <lcd2.c>									// Inclui a biblioteca do LCD 
#byte porta = 0x05									// Diz que o port_a ficara no espaço 0x05 da memoria
#byte portb = 0x06									// Diz que o port_b ficara no espaço 0x06 da memoria
#byte portc = 0x07									// Diz que o port_c ficara no espaço 0x07 da memoria
#byte portd = 0x08									// Diz que o port_d ficara no espaço 0x08 da memoria
#byte porte = 0x09									// Diz que o port_e ficara no espaço 0x09 da memoria
int graus[8]={
0b00000,
0b11100,
0b10100,
0b11100,
0b00000,
0b00000,
0b00000,
0b00000
};
float a,l,inf,sup;									// Declaração das variaveis globais do tipo ponto flutuante
void conta (void);									// Indica que irá ter uma função com retorno 0 com o nome conta
void main()
{
	setup_adc_ports(AN0);							// Diz que irá utiliar o pino AN0 como ADC (pin_a0)
	setup_adc (ADC_CLOCK_INTERNAL);					// Informa que irá utilizar clock interno para o adc
	set_adc_channel(0);								// Inicia o Ad no canal 0
	set_tris_a(0xff);								// Configura todos os pinos do port_a como entrada
	set_tris_b(0xfc);								// Configura todos os pinos do port_b como saída
	set_tris_c(0xff);								// Configura todos os pinos do port_c como entrada
	set_tris_d(0x08);								// Configura todos os pinos do port_d como saída
	set_tris_e(0xff);								// Configura todos os pinos do port_e como entrada
	output_b(0x00);									// Zera saídas do port b
	lcd_init();										// Inicia LCD
	lcd_set_cgram_char(0,graus);					// Armazena a variavel graus no espaço 0 da memoria do LCD
	while (true)
	{	
		conta();									// Vai para função conta
		printf(lcd_putc,"\fControle temperatura");	// /f limpa tela e a seguir mostra o texto que está a frente 
		lcd_gotoxy(1,2);						  	// vai para linha 2, no espaço 1
		printf(lcd_putc,"%1.2f%cC",a,0);	  		// /r da um tab, %1.2f mostra valor com duas casas depois da virgula
		if(a>119.99)								// Se o valor da temperatura for maior que 190°C
		{
			lcd_gotoxy(1,1);						// vai para linha 1, no espaço 1
			printf(lcd_putc,"\fTemperatura elevada");
			lcd_gotoxy(1,2);						// vai para linha 2, no espaço 1
			printf(lcd_putc,"Resfriando sistema");
			lcd_gotoxy(21,1);						// vai para linha 3, no espaço 1
			printf(lcd_putc,"Aguarde um momento");
			output_b(0x00);							// Zera o port b 
			output_high(pin_b1);					// Liga o pino b1
			while(a>90)
			{
				conta();							// Vai para a função Conta
			}
		}
		else if(a>79.99)							// Senão se o valor da temperatura for maior que 100°C
		{
			lcd_gotoxy(21,1);						// vai para linha 3, no espaço 1
			printf(lcd_putc,"Temperatura ideal");
			lcd_gotoxy(21,2);						// vai para linha 4, no espaço 1
			printf(lcd_putc,"Esteira ligada");
			output_b(0x00);							// Zera o portb
			output_high(pin_b0);					// Liga o pino b0
		}
		else										// Senão 
		{
			lcd_gotoxy(21,1);						// Vai para linha 2, no espaço 1
			printf(lcd_putc,"Aquecendo o sistema");
			lcd_gotoxy(21,2);						// Vai para linha 2, no espaço 1
			printf(lcd_putc,"Aguarde um momento");
			output_b(0x00);							// Zera o port b
		}
		while(l<sup&&l>inf)
		{
			l = read_adc();							// Le o valor do AD enquanto for menor que o sup e maior que o inferior
		}
	}
}
void conta ()
{
		l = read_adc();								// Le o valor do ADC
		a = (150/306.9)*l;							// Valor de a pode chegar até 204,6°C
		inf = l - 2;								// Valor do AD - 10, para ser valor inferior
		sup = l + 2;								// Valor do AD + 10, para ser valor superior
}