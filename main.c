#include "Nokia5110.c"
#include "portJ.c"
#include "portN.c"
#include "modoProgramacion.c"
#define Time 950000
#define Time2 500
void mostrar_hora_min_seg(int tiempo){
	//  Se hace el cálculo de las horas, minutos y segundos
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
	// Se inicializa el systick 
	tiempo=(16000*tiempo);
	NVIC_ST_CTRL_R&=~NVIC_ST_CTRL_ENABLE;//apaga el Systick
	NVIC_ST_CURRENT_R=0X0000;//asignar el valor actual en 0
	NVIC_ST_RELOAD_R=tiempo; //valor desde la cuenta
	NVIC_ST_CTRL_R|=NVIC_ST_CTRL_CLK_SRC; // cambia la funete del reloj al reloj del systema NVIC_ST_CTRL_CLK_SRC=0x0004;  Se establece con 16 MHz
	NVIC_ST_CTRL_R|=NVIC_ST_CTRL_ENABLE;//enciende el systick
}

void wait_end_Delay(){
	//  Ciclo que espera que terminen los 250 ms 
	while((NVIC_ST_CTRL_R&NVIC_ST_CTRL_COUNT)==0){};
	NVIC_ST_CTRL_R&=~NVIC_ST_CTRL_ENABLE;//apaga el Systick
}
// Configuración de 2 push botons
void initPortD(void){
	SYSCTL_RCGCGPIO_R |=SYSCTL_RCGCGPIO_R3;//Habilitacion del puerto A
	while ((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R3)==0){};//Verificacion de encendido del puerto
	GPIO_PORTD_DIR_R |= 0x00;//Se eligen los pines de las 8 entradas
	GPIO_PORTD_AFSEL_R |=0x00;//Funcion alternativa
	GPIO_PORTD_DEN_R |=0xFF;//Digital eneble
	GPIO_PORTD_AMSEL_R=0x00;
}
int programarHora(int current){
	//Declaración de variales
		uint8_t hour = 0, min = 0, seg = 0;
		uint32_t day = 0, temp = 0;
		 //Calculo del tiempo actual para mostrar, dividido en horas, minutos y segundos
				day = ( current / 86400);
				temp = ( current % 86400);
				hour = temp / 3600;
				temp = temp % 3600;
				min = temp / 60;
				seg = temp % 60;	
				int selector = 0;
///////////////////////////////////////////////////////////////////
	// mostrar hora con parpadeo que el usuario está modificando
		while(1){
			// Variables para la lectura de los botones de aumento y disminución 
			int botonAumentar  = GPIO_PORTJ_DATA_R & 0x02; // bot�n 3  [aumentar]
			int botonDisminuir  = GPIO_PORTD_DATA_R & 0x01; // bot�n 4 [disminuir]
			// Si selector es 0 significa que se encuentra modificando la hora
			if(selector == 0){
					limpiar_horas();//limpia las horas en pantalla
					init_Delay(100);//inicia el sistick	
				/////////////////// Verificacion de presion del boton DISMINUIR
				if(!(botonDisminuir & 0x01) ){ // Presion bot?n 4
						//genera un tiempo de espera de 250 ms
						init_Delay(100);
						current -= 3600; // RESTA una hora al tiempo actual
						day = ( current / 86400);
						temp = ( current % 86400);
						hour = temp / 3600;
						temp = temp % 3600;
						min = temp / 60;
						seg = temp % 60;
						if(hour == 0){ //Caso especial si se necesita restar cuando este en 0
							current += 3600*24;
						}		
					}
					wait_end_Delay();
					// parpadeo de hora
					mostrar_hora_min_seg(current);	
					init_Delay(300);//inicia el sistick
					wait_end_Delay();
					////////////// AUMNENTAR
					if(!(botonAumentar & 0x02) ){ // Presion bot?n 4
					//genera un tiempo de espera de 250 ms
					init_Delay(100);
					current += 3600; // suma una hora al tiempo actual
					day = ( current / 86400);
					temp = ( current % 86400);
					hour = temp / 3600;
					temp = temp % 3600;
					min = temp / 60;
					seg = temp % 60;
					}
					// Se verifica presión del botón de selector
						while(!(GPIO_PORTD_DATA_R & 0x02)){ // Cambio del selector
							for(uint32_t n=0;n<Time2;n++){}
							if(!(GPIO_PORTD_DATA_R & 0x02)){
							selector= 1; // Con selector igual a 1 esto cambia a minutos 
							break;
							}else{selector = 0;}
						}
		}
			// Si selector es igual a 1, nos encontramos modificando miuntos 
			if(selector == 1){
					limpiar_minutos();//limpia las horas en pantalla
					init_Delay(100);//inicia el sistick	
					//RESTA
				if(!(botonDisminuir & 0x01) ){ // Presion bot?n 4
						//genera un tiempo de espera de 250 ms
						init_Delay(100);
						current -= 60; // resta una hora al tiempo actual
						day = ( current / 86400);
						temp = ( current % 86400);
						hour = temp / 3600;
						temp = temp % 3600;
						min = temp / 60;
						seg = temp % 60;
					}
					//SUMA
					if(!(botonAumentar & 0x02) ){ // Presion bot?n 4
						//genera un tiempo de espera de 250 ms
						init_Delay(100);
						current += 60; // suma una hora al tiempo actual
						day = ( current / 86400);
						temp = ( current % 86400);
						hour = temp / 3600;
						temp = temp % 3600;
						min = temp / 60;
						seg = temp % 60;
					}	
				
					wait_end_Delay();
					mostrar_hora_min_seg(current);	
					init_Delay(300);//inicia el sistick
						while(!(GPIO_PORTD_DATA_R & 0x02)){ // cambiar selector
							for(uint32_t n=0;n<Time2;n++){}
							if(!(GPIO_PORTD_DATA_R & 0x02)){
								selector = 2;
								break;
							}else{selector = 1;}
						}
					wait_end_Delay();

				}
			// Si selector es igual a 2, estamos configurando segundos
			if(selector == 2){
					limpiar_segundos();//limpia las horas en pantalla
					init_Delay(100);//inicia el sistick	
				//RESTA
				if(!(botonDisminuir & 0x01) ){ // Presion bot?n 4
						//genera un tiempo de espera de 250 ms
						init_Delay(100);
						current -= 1; // resta una hora al tiempo actual
						day = ( current / 86400);
						temp = ( current % 86400);
						hour = temp / 3600;
						temp = temp % 3600;
						min = temp / 60;
						seg = temp % 60;

					}
					//SUMA
					if(!(botonAumentar & 0x02) ){ // Presion bot?n 4
						//genera un tiempo de espera de 250 ms
						init_Delay(100);
						current += 1; // suma una hora al tiempo actual
						day = ( current / 86400);
						temp = ( current % 86400);
						hour = temp / 3600;
						temp = temp % 3600;
						min = temp / 60;
						seg = temp % 60;
					}
					wait_end_Delay();
					mostrar_hora_min_seg(current);	
					init_Delay(300);//inicia el sistick
					wait_end_Delay();
					// Se verifica presión del botón SELECTOR
						while(!(GPIO_PORTD_DATA_R & 0x02)){ // cambiar selector
							for(uint32_t n=0;n<Time2;n++){}
							if(!(GPIO_PORTD_DATA_R & 0x02)){
								selector=0;//Regresa a el cambio horas
								break;
							}else{selector = 2;}
						}
			}
		
// Verificación de botón de programación [boton 1]
if(!(GPIO_PORTJ_DATA_R&0x01)){
	return current;
}
		
		}
}

void inicializacionPuertos(){ 
	// Se inicializa el puerto N
	configN();
	// Se inicializa el puerto J
	configJ();
	// Se inicializa el puerto D
	initPortD();
}
// Se configura el módulo de RTC, el cuál tendrá la hora
void configurarRTC(){
	RTCCLK_PK7_Init();//inicializa el PK7 para modo RTC
	HIB_Init();       //Configura el modulo de invernacion
	T0CCP0_PL4_Init();//inicializa el PL4 como entrada del timer 0
	Timer0A_Init(); 	//inicializa el Timer

}

// Se configura el módulo de la pantalla
void configurarPantallaNokia(){
	Nokia5110_Init(); //Inicializacion del Nokia5110
	LcdClear();       //limpia los carácteres nokia_5511
}
// Bandera global que se activa en interrupción 
uint8_t flag=0;
// Interrupción -> flag = 1;
// void InterrupcionModoProgramador(){
// 	flag = 1;
// 	// Inicializar el systick
// }

void primerPlano(){
	// Declaración de variables donde se almacenan las horas, minutos, segundos
	uint8_t hour = 0, min = 0, seg = 0;
	uint32_t day = 0, temp = 0;
	int current; //HORA ACTUAL
	
	//Contador para acceder a modo programador
	int flag_programmer_mode=0;
	int horaModificada=0,horaActual=0;
	// Variable que almacena 
	uint8_t button_programer_mode=0;
	// Ciclo Scan
	while(1){
				// Se inicializa el systick y se arranca (no se espera a que termine)
				init_Delay(250);
				// Se actualiza el valor de la hora
				horaActual  =  TIMER0_TAR_R ;
				// Se muestra en la pantalla
				mostrar_hora_min_seg(horaActual);
				// Se checa el botón de programador
				button_programer_mode= ~(GPIO_PORTJ0<<1);
				//presion del boton para modo proframacion
				if(button_programer_mode==0xFF){
						flag_programmer_mode++;	
						// Se genera interrupción 
					}else{
						flag_programmer_mode=0;	
					}
					if(flag_programmer_mode==4){
						GPIO_PORTN1=0X00;	//apaga el led que indica la salida del modo programador
						TIMER0_CTL_R &= ~TIMER_CTL_TAEN; //Apaga el timer de tiempo real
						// Función de programar hora
						horaModificada = programarHora(horaActual);
						// Se modifica la hora con base a lo que el usuario cambió
						TIMER0_TAV_R = horaModificada; 
						//salida del modo programador
						TIMER0_CTL_R |= TIMER_CTL_TAEN; //Activacion del timer
						flag_programmer_mode=0; //reinicia la bandera
						init_Delay(1);
					}
					GPIO_PORTN1=0X02;//enciende el led que indica la entrada al modo programación
				wait_end_Delay();	// Espera a que se terminen 250 ms (con lo siguiente se aseguran los 250ms)
	}
}
int main(){
	// Módulo inicialización
	inicializacionPuertos();	
	configurarRTC();
	// Configurar Pantalla
	configurarPantallaNokia();
	// 
	LcdClear(); //limpia LCD
  	//********************************************************************************
	//  Función para configurar y habilitar las interrupciones 
	//	configurarInterrupcion();
  	//********************************************************************************
	//  Se establece un valor arbitrario del timer para que lo primero que se vea en la
	//  pantalla sea la hora [23:59:59]
	TIMER0_TAV_R = 3600*23 + 60*59 + 59 ;
	// El programa se queda ejecutando el ciclo Scan 
	primerPlano(); 
}