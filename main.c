#include "Nokia5110.c"
#include "portJ.c"
#include "portN.c"
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
void init_Delay(uint32_t tiempo){
	tiempo=(16000*tiempo);
	// NVIC_ST_CTRL_R = 0x00; // Se establece con 4 Mhz
	NVIC_ST_CURRENT_R=0X0000;//asignar el valor actual en 0
	NVIC_ST_RELOAD_R=tiempo; //valor desde la cuenta
	NVIC_ST_CTRL_R|=NVIC_ST_CTRL_CLK_SRC; // cambia la funete del reloj al reloj del systema NVIC_ST_CTRL_CLK_SRC=0x0004;  Se establece con 16 MHz
	NVIC_ST_CTRL_R|=NVIC_ST_CTRL_ENABLE;//enciende el systick
}

void wait_end_Delay(){
	while((NVIC_ST_CTRL_R&NVIC_ST_CTRL_COUNT)==0){};
	NVIC_ST_CTRL_R&=~NVIC_ST_CTRL_ENABLE;//apaga el Systick
}
int main(){
	configN();
	configJ();
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
	uint8_t button_programer_mode=0;
	uint8_t flag=0;
	LcdClear(); //limpia LCD
	while(1){
				init_Delay(250);
				mostrar_hora_min_seg(TIMER0_TAR_R);
		
				button_programer_mode=~(GPIO_PORTJ0<<1);
				if(button_programer_mode==0xFF){
						flag_programmer_mode++;	
					}else{
						flag_programmer_mode=0;	
					}
					if(flag_programmer_mode==4){
						flag_programmer_mode=0;
						//////////Aqui pones tu funcion de programador/////////////////
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