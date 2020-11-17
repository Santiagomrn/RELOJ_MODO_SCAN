#include <stdint.h>
// **************************************************************************************
// **************************************************************************************
// Variables globales
// **************************************************************************************
// **************************************************************************************
		// Verificación
		int bandera = 0 ;
		// verificación de botón presionado 
		int bandera2 = 1 ;
		// Declaración de variables donde se almacenan las horas, minutos, segundos
		uint8_t hour = 0, min = 0, seg = 0;
		uint32_t day = 0, temp = 0;
// **************************************************************************************
// **************************************************************************************
// **************************************************************************************

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
// 
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
	GPIO_PORTD_PUR_R=0XF;
}

/// SE QUEDA ASI --------------------------------------------------------------------------------------------------------
int programarHora(int current){
	//Declaración de variales
		DisableInterrupts();
		uint8_t hour = 0, min = 0, seg = 0;
		uint32_t day = 0, temp = 0;
		//Calculo del tiempo actual para mostrar, dividido en horas, minutos y segundos
		//GPIO_PORTN1=0X02;//enciende el led que indica la entrada al modo programación
				//GPIO_PORTN_DATA_R = 0x02;
				TIMER0_CTL_R=0x00000000;	 //Apaga el timer 0
				day = ( current / 86400);
				temp = ( current % 86400);
				hour = temp / 3600;
				temp = temp % 3600;
				min = temp / 60;
				seg = temp % 60;	
				int selector = 0;
	// mostrar hora con parpadeo que el usuario está modificando
		while(1){
			// Variables para la lectura de los botones de aumento y disminución 
			int botonAumentar  = GPIO_PORTD_DATA_R & 0x01; // bot�n 3  [aumentar] // D0
			int botonDisminuir  = GPIO_PORTD_DATA_R & 0x04; // bot�n 4 [disminuir] // D2
			// Si selector es 0 significa que se encuentra modificando la hora
			if(selector == 0){
					limpiar_horas();//limpia las horas en pantalla
					init_Delay(100);//inicia el sistick	
				/////////////////// Verificacion de presion del boton DISMINUIR
				if(!(botonDisminuir & 0x04) ){ // Presion bot?n 4
						//genera un tiempo de espera de 250 ms
						init_Delay(100);
						day = ( current / 86400);
						temp = ( current % 86400);
						hour = temp / 3600;
						temp = temp % 3600;
						min = temp / 60;
						seg = temp % 60;
						if(hour <= 1){ //Caso especial si se necesita restar cuando este en 0
							current += 3600*24;
						}	
						current -= 3600; // RESTA una hora al tiempo actual
						day = ( current / 86400);
						temp = ( current % 86400);
						hour = temp / 3600;
						temp = temp % 3600;
						min = temp / 60;
						seg = temp % 60;	
					}
					wait_end_Delay();
					// parpadeo de hora
					mostrar_hora_min_seg(current);	
					init_Delay(300);//inicia el sistick
					wait_end_Delay();
					////////////// AUMNENTAR
					if(!(botonAumentar & 0x01) ){ // Presion bot?n 4
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
			// Si selector es igual a 1, nos encontramos modificando minutos 
			if(selector == 1){
					limpiar_minutos();//limpia las horas en pantalla
					init_Delay(100);//inicia el sistick	
					//RESTA
				if(!(botonDisminuir & 0x04) ){ // Presion bot?n 4
						//genera un tiempo de espera de 250 ms
						init_Delay(100);
						current -= 60; // resta una hora al tiempo actual
						day = ( current / 86400);
						temp = ( current % 86400);
						hour = temp / 3600;
						temp = temp % 3600;
						min = temp / 60;
						seg = temp % 60;
						if(current <= 3600){ //Caso especial si se necesita restar cuando este en 0
							current += 3600*24;
						}	
					
					}
					//SUMA
					if(!(botonAumentar & 0x01) ){ // Presion bot?n 4
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
			// Si selector es igual a 2, estamosK configurando segundos
			if(selector == 2){
					limpiar_segundos();//limpia las horas en pantalla
					init_Delay(100);//inicia el sistick	
				//RESTA
				if(!(botonDisminuir & 0x04) ){ // Presion bot?n 4
						//genera un tiempo de espera de 250 ms
						init_Delay(100);
						current -= 1; // resta una hora al tiempo actual
						day = ( current / 86400);
						temp = ( current % 86400);
						hour = temp / 3600;
						temp = temp % 3600;
						min = temp / 60;
						seg = temp % 60;
						if(current <= 3600){ //Caso especial si se necesita restar cuando este en 0
							current += 3600*24;
						}			
					}
					//SUMA
					if(!(botonAumentar & 0x01) ){ // Presion bot?n 4
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

///----------------------------------------------------------------------------------------------------------------

void inicializacionPuertos(){ 
	// Se inicializa el puerto N
	configN();
	// Se inicializa el puerto J [Modo programación]
	configJ();
	// Se inicializa el puerto D
	initPortD();
}
// Se configura el módulo de RTC, el cuál tendrá la hora
void configurarRTC(){
//	RTCCLK_PK7_Init();//inicializa el PK7 para modo RTC
//	HIB_Init();       //Configura el modulo de invernacion
//	T0CCP0_PL4_Init();//inicializa el PL4 como entrada del **timer 0
	Timer0A_Init(); 	//inicializa el Timer
}

void configurarTimer1(){
	/*
	// inicialización del timer 1 para la verificación del segundo de presión
	// TIMER1 para la verificación de presionado
	// void Timer1A_Init(void){
		//Modo Real Time Clock
		SYSCTL_RCGCTIMER_R|=0x02;
		while ((SYSCTL_PRTIMER_R &0x02) == 0){
		}; //verifica la habilitacion del timer0

		TIMER1_CTL_R &= ~TIMER_CTL_TAEN; //Apaga el timer
		TIMER1_TAMR_R = 0;
		TIMER1_CFG_R |= TIMER_CFG_32_BIT_RTC ; //Configura para 32 bits 
		TIMER1_CC_R &= ~TIMER_CC_ALTCLK; // Timer0 fuente de reloj del reloj del sistema
		TIMER1_TAILR_R=16000000;							//PASO 5: CARGAMOS EL VALOR
		//TIMER1_TAILR_R = 0xF42400;         // Carga del valor máximo del timer
		// TIMER1 Configura
		// TIMER1_TAMATCHR_R = 0X2400;			  //Configura valor de coincidencia
		// TIMER1_TBMATCHR_R = 0XF4;

		TIMER1_ICR_R |= TIMER_ICR_RTCCINT; //Input Clear Registro
		TIMER1_IMR_R |= TIMER_IMR_RTCIM;   //Mascara de interrupcion

		NVIC_PRI4_R = (NVIC_PRI4_R & 0X0FFFFFFF) | 0X80000000; //prioridad 2
		NVIC_EN0_R |= 1 << 21;
		// Para el debugger
		// No se requieren interrupciones
		// TIMER1_CTL_R |= TIMER_CTL_TAEN; //Activacion del timer
	// }
*/
	SYSCTL_RCGCTIMER_R|=0x02;							//Activa timer 0
	while((SYSCTL_PRTIMER_R&0x02)==0){}	//Compruebo si se activo el timer
	TIMER1_CC_R|=0x00000001;							//Fuente de reloj alternativo
	SYSCTL_ALTCLKCFG_R=0x00000000;				//Seleccionamos PIOSC
	TIMER1_CTL_R|=~0x00000001;						//PASO 1: APAGO EL TIMER	
	TIMER1_CFG_R=0x00000000;							//PASO 2: CONFIGURAMOS PARA 32 BITS	  
	TIMER1_TAMR_R=0x00000002;							//PASO 3: CONFIGURAMOS EN MODO PERIODICO
	//**************************************PASO 4: LO OMITIMOS
	TIMER1_TAILR_R=16000000;							//PASO 5: CARGAMOS EL VALOR
	TIMER1_ICR_R|=0X00000001;							//PASO 6. BORRO BANDERA
	//**************************************PASO 7: ACTIVAMOS EL TIMER EN EL MAIN
	TIMER1_IMR_R|=0x00000001;							//Activamos interrupcion del Timer A
	NVIC_EN0_R |= 1 <<21;									//Activamos prioridad de la interrupcion 21
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

//
uint32_t segTimer = 0;
void TIMER0A_Handler(){
	//  Se hace el cálculo de las horas, minutos y segundos
		// uint8_t hour = 0, min = 0, seg = 0;
		// uint32_t day = 0, temp = 0;
		//hora
		// Se actualiza el valor de la hora
		// Se guarda copia del valor del timer
		TIMER0_ICR_R=0x01;										//Borro bandera en el registro GPTMICR pag 1002
		segTimer++;
		int timeCopy = segTimer;
		day = ( timeCopy / 86400);
		temp = ( timeCopy % 86400);
		hour = temp / 3600;
		temp = temp % 3600;
		min = temp / 60;
		seg = temp % 60;	
		//mostrar_hora_t(hour,min,seg);
}
// 
// 
// 
//void DisableInterrupts(void);//Desactiva interrupciones globales
void EnableInterrupts(void); //Activa interrupciones globales	void DisableInterrupts(void);//Desactiva interrupciones globales
	
void primerPlano(){
	int current; //HORA ACTUAL
	//Contador para acceder a modo programador
	int horaModificada=0,horaActual=0;
	// Variable que almacena 
	// Habilitar inter
	EnableInterrupts();	
	TIMER0_CTL_R|=0x00000001;							//Activamos el timer 0A
	segTimer = 3600*23 + 60*59 + 59;
	while(1){
				// Se inicializa el systick y se arranca (no se espera a que termine)
				// init_Delay(250);
				//presion del boton para modo proframacion 
				if(bandera==1){ 
						// Función de programar hora
						GPIO_PORTN_DATA_R = 0x02;
						horaModificada = programarHora(segTimer);
						// Se modifica la hora con base a lo que el usuario cambió
						EnableInterrupts();
						segTimer = horaModificada;
						// TIMER0_TAV_R = horaModificada; 
						//salida del modo programador
						TIMER0_CTL_R |= TIMER_CTL_TAEN; //Activacion del timer tiempo real
						bandera=0; //reinicia la bandera
						init_Delay(1);
						// Se genera interrupción en el flanco de bajada
						bandera=0;
						GPIO_PORTN_DATA_R = 0x00;
					}else{
						bandera=0;	
					}

				// Se muestra en la pantalla
				//horaActual  =  TIMER0_TAR_R ;
				//mostrar_hora_min_seg(horaActual);		

				mostrar_hora_t(hour,min,seg);
}
}
int main(){
	// Módulo inicialización
	inicializacionPuertos();	
	configurarRTC();
	configurarTimer1();
	// ******************************************************************************
	// ConfigurarTimer1();
	// ******************************************************************************
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
	//TIMER0_TAV_R = 0 ;
	// El programa se queda ejecutando el ciclo Scan 
	primerPlano(); 
}

	// mostrar_hora_t(hour,min,seg);


