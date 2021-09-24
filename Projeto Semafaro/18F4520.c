#include <18F4520.h>
#FUSES XT														//Utilização de cristal externo >=4MHz
#FUSES NOWDT 													//Sem Timer Cão de Guarda
#FUSES NOPUT 													//Sem timer de alimentação
#FUSES NOPROTECT 												//Epprom desprotegida (permite a leitura)
#FUSES NOBROWNOUT 												//Brownout desabilitado
#FUSES NOLVP 													//Programação em baixa tensão desabilitada
#FUSES NOCPD 													//Código desprotegido (permite a leitura)
#use delay(clock=4000000)										//Clock externo de 4MHz
int a,i,j,c,aant,t;												//Definição das variaveis do tipo inteiro
long int aguard=50000;											//Definição da variavel do tipo long inteira ()
#define liga output_high(										//Define que o nome liga é igual ao comando output_high(
#define desliga output_low(										//Define que o nome desliga é igual ao comando output_low(
#define altera output_toggle(									//Define que o nome altera é igual ao comando output_toggle(
#define ledcarverm pin_c0);										//Define que o nome ledcarverm é igual ao pino c0
#define ledcaramar pin_c1);										//Define que o nome ledcaramar é igual ao pino c1
#define ledcarverd pin_c2);										//Define que o nome ledcarverd é igual ao pino c2
#define ledverdped pin_c3);										//Define que o nome ledverdped é igual ao pino c3
#define ledvermped pin_c4);										//Define que o nome ledvermped é igual ao pino c4
#INT_EXT//														//Função interrupção externa
void interrupt ()												//Função interrupção externa
{	
	if(aguard>=40000)											//Se a variavel aguard for maior que 40000
	{
		a = a + 1;												//Variavel a incrementa 1 em seu valor
		aguard=0;												//Variavel aguard passa a valer 0
	}
}
void contagem();												//Definição de uma função com nome de contagem
void espera();													//Definição de uma função com nome de espera
void main()														//Inicio da função principal
{
	clear_interrupt(INT_EXT);									//Limpa interrupção externa
	enable_interrupts(GLOBAL);									//Inicializa a interrupção Global
	enable_interrupts(INT_EXT);									//Inicializa a interrupção externa
	ext_int_edge(0,H_TO_L);										//Indica que a interrupção externa será no pino b0 e com borda de descida
	set_tris_a(0xFF);											//Configura o port A como entrada
	set_tris_b(0xFF);											//Configura o port B como entrada
	set_tris_c(0xE0);											//Configura os pinos C0 a C4 como saida e os demais como entrada
	set_tris_d(0x90);											//Configura os pinos D0 a D5 como saida e os demais como entrada	
	set_tris_e(0xFF);											//Configura o port E como entrada
    port_b_pullups(1);											//Aciona o pull up interno do port b
	output_c(0);												//Zera a saída do port C
	output_d(0);												//Zera a saída do port D
	a = 0;														//Atribui o valor 0 para variavel a
	while(TRUE)													//Condição de laço infiniteo
	{
		aant = a;												//Todo começo da repitição, atribui o valor da variavel a para variavel aant (a anterior)
		switch(a)
		{
			case 0:												//Caso o valor de a for igual a 0
				output_c(0);									//Zera a saida do port C
				liga ledcaramar									//Liga o led Amarelo para o carro
				liga ledvermped if(a>aant){break;}				//Liga o led vermelho para o pedestre, Se a for maior que a anterior, significa que foi a interrupção foi acionada e sai do case
				t=60;espera();if(a>aant){break;}				//Atribui o valor 60 para variavel t, Vai para função de espera Se a for maior que a anterior, sai do case
				a++;											//Incrementa 1 na variavel a
			break;												//Sai do Case
			case 1:												//Caso o valor de a for igual a 1
				output_c(0);									//Zera a saida do port C
				liga ledcarverm									//Liga o led vermelho para o carro
				liga ledvermped if(a>aant){break;}				//Liga o led vermelho para o pedestre, Se a for maior que aant, sai do case
				t=40;espera();if(a>aant){break;}				//Atribui o valor 40 para variavel t, Vai para função de espera, Se a for maior que aant sai do case
				a++;											//Incrementa 1 na variavel a
			break;												//Sai do Case
			case 2:												//Caso o valor de a for igual a 2
				output_c(0);									//Zera a saida do port C
				liga ledcarverm									//Liga o led vermelho para o carro
				liga ledverdped if(a>aant){break;}				//Liga o led verde para o pedestre, Se a for maior que aant, sai do case
				output_d(2);									//Atribui o valor 1 para o pino RD2
				contagem();										//Vai para função de contagem
				desliga ledverdped								//Desliga o led verde para o pedestre
				liga ledvermped if(a>aant){break;}				//Liga o led vermelho para o pedestre, Se a for maior que aant, sai do case
				for(i=0;i<6;i++)								//Para i=0, enquanto i for menor do que 6, incrementa 1 na variavel i
				{
					altera ledvermped							//Altera a saida do led vermelho pedestre
					altera pin_d6);								//Altera a saida do pino d6(pino de BI do display de 7 segmentos)
					delay_ms(500);aguard = aguard + 500;		//Espera 500 miliSegundos, incrementa 500 para variavel de aguard
				}
				liga ledvermped									//liga o led vermelho para pedestre
				t=8;espera();if(a>aant){break;}					//Atribui o valor 8 para variavel t, Se a for maior que aant, sai do case
				output_d(0);if(a>aant){break;}					//Zera a saida do port D, Se a for maior que aant, sai do case
				a++;											//Incrementa 1 na variavel a
			break;												//Sai do Case
			case 3:												//Caso o valor de a for igual a 3
				output_c(0);output_d(0);						//Zera a saida do port C e D
				liga ledvermped									//Liga o led vermelho para pedestre
				liga ledcarverd if(a>aant){break;}				//Liga o led verde para o carro
				t=100;espera();if(a>aant){break;}				//Atribui o valor 100 para variavel 100, Vai para função de espera, Se a for maior que aant sai do case
				a++;											//Incrementa 1 na variavel a
			break;												//Sai do case
			default:											//Caso o valor de seja diferente dos demais
				a = 0;											//Atribui 0 para variavel a
			break;												//Sai do case
		}
	}
}
void contagem(void)												//Função de contagem do display de 7 segmentos
{
	i=2;j=0;c=0;												//Atribui o valor 2 para variavel i e 0 para variavel j e c
	while(c<=20)												//Enquanto c for menor ou igual a 20
	{
		output_d(i+32+64);										//Valor da saída do portd igual a i + 32 + 64 (32 valor do pino LE/STB do display 2) (64 valor de BI)
	    output_high(pin_d4);									//Aciona o pino RD4 (LE/STB display 1)
	    output_d(j+16+64);										//Valor da saída do portd igual a j + 16 + 64 (16 valor do pino LE/STB do display 1) (64 valor de BI)
	    output_high(pin_d5);									//Aciona o pino RD5 (LE/STB display 2)
		delay_ms(1000);aguard = aguard + 1000;					//Aguarda 1 Segundo, acrescenta 1000 na variavel aguard
		if(j==0){j=10;i--;}										//Se j for igual a 0, atribui 10 para variavel j, decrementa 1 da variavel i
	    j--;c++;												//Decrementa 1 da variavel j, incrementa 1 da variavel c
	}
}
void espera(void)												//Função de espera (aguardar tempo enquanto outros led's ficam ligados/desligados)
{
	while(t>0)													//Enquando t for maior que 0
	{
		delay_ms(50);											//Espera 50 mili Segundos
		aguard = aguard + 50;									//Incrementa 50 na variavel aguard
		t--;if(a>aant){t=0;}									//Decrementa 1 da variavel t, Se a for maior que aant, então atribui 0 para variavel t
	}
}