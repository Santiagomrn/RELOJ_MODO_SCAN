#include <stdint.h>

#define GPIO_PORTJ0 (*((volatile uint32_t *)0x40060004))// PARA ESCRIBIR EN el puerto J0
#define GPIO_PORTJ_AFSEL_R (*((volatile uint32_t *)0x40060420)) //para contrlarlo con los REGISTROS GPIO
#define GPIO_PORTJ_DIR_R (*((volatile uint32_t *)0x40060400))	 //PARA CONFIGURAR COMO SALIDA O ENTRADA EL PUERTO J
#define GPIO_PORTJ_DEN_R (*((volatile uint32_t *)0x4006051C))  //PARA CONFIGURAR DIGITAL O ANALOGICA EL PUERTO J
#define GPIO_PORTJ_PUR_R (*((volatile uint32_t *)0x40060510))  //PARA CONFIGURAR RESISTENCIAL PULL UP
#define GPIO_PORTJ_PDR_R (*((volatile uint32_t *)0x40060514))		//PARA CONFIGURAR RESISTENCIAL PULL DOWN
#define SYSCTL_RCGCGPIO_R (*((volatile uint32_t *)0x400FE608)) //PARA CONFIGURAR ENCENDIDO DEL PUERTO
#define SYSCTL_PRGPIO_R 			(*((volatile uint32_t *)0x400FEA08))
	
// configuraci�n dos push boton TIVA 
// void configJ(void){
// 	SYSCTL_RCGCGPIO_R |=0X0100; //HABILITA EL PUERTO J
// 	while ((SYSCTL_PRGPIO_R&0X0100)==0){};
// 	GPIO_PORTJ_AFSEL_R &=~0x03; //coloca un zero en la posicion 0 y 1 del registro GPIO_POTJ_AFSEL_R para controlarlo con los registros GPIO
// 	GPIO_PORTJ_DIR_R &=~0X03; ////coloca un zero en la posicion 0 y 1 del registro GPIO_POTJ_DIR_R para configurarlo como entrada
// 														//0 entrada			1 salidad
// 	GPIO_PORTJ_DEN_R |=0X03; ////coloca un UNO en la posicion 0 y 1 del registro 	GPIO_PORTJ_DEN_R para configurar como digital
// 											//0 analogica 1 digital 
// 	// GPIO_PORTJ_IM_R
// 	GPIO_PORTJ_PUR_R |=0X03;// COLOCO UN UNO en la posicion 0 y 1 del registro 	GPIO_PORTJ_PUR_R PARA CONFIGURAR resistencias pull up
	
// }


void GPIOPortJ_Handler(void);

void configJ(void){
SYSCTL_RCGCGPIO_R |=0X00100; //Habilito el puerto J
while ((SYSCTL_PRGPIO_R&0X00100)==0){} //Compruebo si se habilit� el puerto J
GPIO_PORTJ_AFSEL_R&=~0X01; //coloco un cero para controlarlo con los registros GPIO
GPIO_PORTJ_DIR_R&=~0X01;  //Configuro el puerto J como entrada
GPIO_PORTJ_AMSEL_R&=0X00; //Desactivo el modo anal�gico
GPIO_PORTJ_DEN_R =0X01; //Configure el puerto J como digital
GPIO_PORTJ_PUR_R|=0X01; //Activo resistencias Pull-Up
GPIO_PORTJ_PCTL_R&=~0X00F; //No selecciono ninguna de las funciones perif�ricas
GPIO_PORTJ_IM_R|=0X01; //Activar interrupcion por cambio en el puerto J pin 0
GPIO_PORTJ_IS_R&=~0X01; //Registro que configura interrupciones por flanco o por nivel
GPIO_PORTJ_IEV_R&=~0X01; //Genera interrupci�n cuando el pin pasa de 1 a 0, bajada
GPIO_PORTJ_IBE_R&=~0X01; //Configura pin 0 o puerto j 
GPIO_PORTJ_ICR_R=0X01; //Registro para borrar banderas, escribir 1
GPIO_PORTJ_IM_R|=0X01; //Activar interrupcion por cambio en el puerto J pin 0
NVIC_PRI12_R=(NVIC_PRI12_R&0X0FFFFFFF)|0XE0000000;
NVIC_EN1_R=0X00080000;
}

// *********************************************************************
// Interrupción modo programación 
// *********************************************************************
// Cambio de configuración de flanco para verificar presionado
//  Uso de la BANDERA

void GPIOPortJ_Handler(void){
	// Limpiar interrupción de Puerto J
	GPIO_PORTJ_ICR_R=0X01;
	if(bandera2){
		
		// Se usa systick para crear delay de 100ms ;
		// Evita rebotes del botón (evita falsos disparos)
		init_Delay(10);
		wait_end_Delay();
		//ACTIVAR TIMER
		TIMER1_CTL_R|=0x00000001;	
		// Flanco de bajada => pin 0K
		// Flanco de subida => pin 1
		GPIO_PORTJ_IEV_R=0X01; //Genera interrupci�n cuando el pin pasa de 1 a 0, flanco de subida
		bandera2=0;
	}else
	{
		// TIMER1_CTL_R |= TIMER_CTL_TAEN; //MATAR EL TIMER <-------
		//TIMER1_CTL_R &= ~TIMER_CTL_TAEN; //Apaga el timer de tiempo real
		
		TIMER1_CTL_R=0x00000000;
		GPIO_PORTJ_IEV_R=0X00;
		init_Delay(10);
		wait_end_Delay();
		bandera2=1;
	}
	
}
//--------------------------------
void TIMER1A_Handler(void){
		TIMER1_ICR_R=0x01;										//Borro bandera en el registro GPTMICR pag 1002
		GPIO_PORTJ_IEV_R=0X00;
		bandera2=1;
		bandera=1;
	
}
//----------------

/*

GPIO_PORTN1=0X00;	//apaga el led que indica la salida del modo programador
TIMER0_CTL_R &= ~TIMER_CTL_TAEN; //Apaga el timer de tiempo real
// Función de programar hora
horaModificada = programarHora(horaActual);
// Se modifica la hora con base a lo que el usuario cambió
TIMER0_TAV_R = horaModificada; 
//salida del modo programador
TIMER0_CTL_R |= TIMER_CTL_TAEN; //Activacion del timer
flag_programmer_mode=0; //reinicia la bandera
init_Delay(1);*/