#include <16f877a.h>								//Inclui biblioteca do microcontrolador 16f877a
#fuses xt,nowdt,noprotect,put,brownout,nolvp,nocpd,nowrt 	//Configuração dos fusíveis
#use delay(clock=4000000)							//Usa crystal de 4MHz
#byte porta = 0x05									//Diz que o port_a ficara no espaço 0x05 da memoria
#byte portb = 0x06									//Diz que o port_b ficara no espaço 0x06 da memoria
#byte portc = 0x07									//Diz que o port_c ficara no espaço 0x07 da memoria
#byte portd = 0x08									//Diz que o port_d ficara no espaço 0x08 da memoria
#byte porte = 0x09									//Diz que o port_e ficara no espaço 0x09 da memoria
void poste (void);
#define VM pin_b0
#define AM pin_b1
#define VD pin_b2
#define DIA pin_a5
#define ENT pin_a4
#define SAI pin_a3
int a;
void main()											//Inicia função Inicial
{
	set_tris_a(0xff);								//Configura todos os pinos do port_a como entrada
	set_tris_b(0x00);								//Configura todos os pinos do port_b como saída
	set_tris_c(0xff);								//Configura todos os pinos do port_c como entrada
	set_tris_d(0xff);								//Configura todos os pinos do port_d como entrada
	set_tris_e(0xf0);								//Configura todos os pinos do port_e como entrada	
	output_b(0x00);
	while (true)
	{	
		output_b(0x00);
		output_e(0x00);
		output_high(VM);
		for(a=0;a<=14;a++)
		{
			poste();
		}
		output_b(0x00);
		output_high(VD);
		for(a=0;a<=10;a++)
		{
			poste();
		}
		output_b(0x00);
		output_high(AM);
		for(a=0;a<=4;a++)
		{
			poste();
		}
	}
}
void poste()
{
	if(!input(DIA))
	{
		output_c(0xff);
	}
	if(input(DIA))
	{
		output_c(0x00);
	}
	if(!input(ENT))
	{
		output_e(0xff);
	}
	delay_ms(499);
	if(!input(SAI))
	{	
		delay_ms(500);
		output_e(0x00);
		a = a++;
	}
}