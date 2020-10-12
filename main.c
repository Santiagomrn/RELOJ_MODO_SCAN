#include "Nokia5110.c"
#include "portJ.c"
#include "portN.c"
#include "modoProgramacion.c"
#define Time 950000
void mostrar_hora_min_seg(int tiempo){
		uint8_t hour = 0, min = 0, seg = 0;
		uint32_t day = 0, temp = 0;
		//hora
		int current = tiempo;
		day = ( current / 86400);
		temp = ( current % 86400);
		hour = temp / 3600;
		temp = temp % 3600;
		min = temp / 60;
		seg = temp % 60;	
		mostrar_hora_t(hour,min,seg);
}
//

void init_Delay(uint32_t tiempo){
	tiempo=(16000*tiempo);
	// NVIC_ST_CTRL_R = 0x00; // Se establece con 4 Mhz
	NVIC_ST_CURRENT_R=0X0000;//asignar el valor actual en 0
	NVIC_ST_RELOAD_R=tiempo; //valor desde la cuenta
	NVIC_ST_CTRL_R|=NVIC_ST_CTRL_CLK_SRC; // cambia la funete del reloj al reloj del systema NVIC_ST_CTRL_CLK_SRC=0x0004;  Se establece con 16 MHz
	NVIC_ST_CTRL_R|=NVIC_ST_CTRL_ENABLE;//enciende el systick
}

void wait_end_Delay(){
	//NVIC_ST_CURRENT_R
	while((NVIC_ST_CTRL_R&NVIC_ST_CTRL_COUNT)==0){};
	NVIC_ST_CTRL_R&=~NVIC_ST_CTRL_ENABLE;//apaga el Systick
}
void initPortD(void){
	SYSCTL_RCGCGPIO_R |=SYSCTL_RCGCGPIO_R3;//Habilitacion del puerto A
	while ((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R3)==0){};//Verificacion de encendido del puerto
	GPIO_PORTD_LOCK_R=0X4C4F434B;//Se debloquea el pin 7 del puerto D
	GPIO_PORTD_CR_R=0XFF;
	GPIO_PORTD_DIR_R |= 0x00;//Se eligen los pines de las 8 entradas
	GPIO_PORTD_AFSEL_R |=0x00;//Funcion alternativa
	GPIO_PORTD_DEN_R |=0xFF;//Digital eneble
	GPIO_PORTD_AMSEL_R=0x00;
	//GPIO_PORTD_PUR_R|=0X01;
}
int programarHora(int current){
		uint8_t hour = 0, min = 0, seg = 0;
		uint32_t day = 0, temp = 0;
		int horasModificadas=0, minutosModificados=0, segundoModificados = 0 ;
		TIMER0_CTL_R &= ~TIMER_CTL_TAEN; //Apaga el timer
				day = ( current / 86400);
				temp = ( current % 86400);
				hour = temp / 3600;
				temp = temp % 3600;
				min = temp / 60;
				seg = temp % 60;	
		int botonAumentarHora  = GPIO_PORTJ_DATA_R & 0x02;
		//int botonProgramarHora = GPIO_PORTJ_DATA_R & 0x01;
		while(1){
			int botonAumentarHora  = GPIO_PORTJ_DATA_R & 0x02; // bot�n 3
			int botonProgramarHora = GPIO_PORTJ_DATA_R & 0x01;
			if(botonAumentarHora){ // Presion bot?n 4
				current = current - 1; // resta tiene error 85 hrs
				day = ( current / 86400);
				temp = ( current % 86400);
				hour = temp / 3600;
				temp = temp % 3600;
				min = temp / 60;
				seg = temp % 60; // [00:00:12] -> [86:27:12]
				
				//for(uint32_t n=0;n<Time;n++){}
				limpiar_minutos();
				for(uint32_t n=0;n<Time;n++){}
				mostrar_minutos(min);
				for(uint32_t n=0;n<Time;n++){}
				mostrar_horas(hour);
				mostrar_minutos(min);
				mostrar_segundos(seg);
					
			}
			//if(){} boton programacion
			//break;
			int salirProgramacion = 0;

		}
		return current ;
}

int main(){
	configN();
	configJ();
	initPortD();
	RTCCLK_PK7_Init();//inicializa el PK7 para modo RTC
	HIB_Init();       //Configura el modulo de invernacion
	T0CCP0_PL4_Init();//inicializa el PL4 como entrada del timer 0
	Timer0A_Init(); 	//inicializa el Timer
	Nokia5110_Init(); //Inicializacion del Nokia5110
	LcdClear();       //limpia el nokia_5511
	uint8_t hour = 0, min = 0, seg = 0, lastHour = 0, lastMin = 0, lastSeg = 0;
	uint32_t day = 0, temp = 0, LastTIMER0_TAR_R = 0;
	int current; //HORA ACTUAL
	
	//programer mode variables
	int flag_programmer_mode=0;
	int horaModificada=0,horaActual=0;
	uint8_t button_programer_mode=0;
	uint8_t flag=0;
	LcdClear(); //limpia LCD
	horaModificada = horaModificada + 60*5;
	while(1){
				init_Delay(250);
				int horaActual  =  TIMER0_TAR_R + horaModificada;
				mostrar_hora_min_seg(horaActual);
		
				button_programer_mode= ~(GPIO_PORTD_DATA_R&0x02);//~(GPIO_PORTJ0<<1);
				if(button_programer_mode==0xFF){
						flag_programmer_mode++;	
					}else{
						flag_programmer_mode=0;	
					}
					if(flag_programmer_mode==4){
						flag_programmer_mode=0;
						//////////Aqui pones tu funcion de programador/////////////////
						PortN_Output(0x01);
						horaModificada += programarHora(horaActual);
						//programarHora(&horaActual);
						TIMER0_CTL_R |= TIMER_CTL_TAEN; //Activacion del timer
						if(flag==0)	{
							GPIO_PORTN1=0X02;
							flag=1;
						}else{
							GPIO_PORTN1=0X00;
							flag=0;
						}
					}
				wait_end_Delay();	
	}
}