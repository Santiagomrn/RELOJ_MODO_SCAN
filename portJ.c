#include <stdint.h>
#
#define GPIO_PORTJ0 (*((volatile uint32_t *)0x40060004))// PARA ESCRIBIR EN el puerto J0

#define GPIO_PORTJ_AFSEL_R (*((volatile uint32_t *)0x40060420)) //para contrlarlo con los REGISTROS GPIO
#define GPIO_PORTJ_DIR_R (*((volatile uint32_t *)0x40060400))	 //PARA CONFIGURAR COMO SALIDA O ENTRADA EL PUERTO J
#define GPIO_PORTJ_DEN_R (*((volatile uint32_t *)0x4006051C))  //PARA CONFIGURAR DIGITAL O ANALOGICA EL PUERTO J
#define GPIO_PORTJ_PUR_R (*((volatile uint32_t *)0x40060510))  //PARA CONFIGURAR RESISTENCIAL PULL UP
#define GPIO_PORTJ_PDR_R (*((volatile uint32_t *)0x40060514))		//PARA CONFIGURAR RESISTENCIAL PULL DOWN
#define SYSCTL_RCGCGPIO_R (*((volatile uint32_t *)0x400FE608)) //PARA CONFIGURAR ENCENDIDO DEL PUERTO
#define SYSCTL_PRGPIO_R 			(*((volatile uint32_t *)0x400FEA08))
	

void configJ(void){
	SYSCTL_RCGCGPIO_R |=0X0100; //HABILITA EL PUERTO J
	while ((SYSCTL_PRGPIO_R&0X0100)==0){};
	GPIO_PORTJ_AFSEL_R &=~0x03; //coloca un zero en la posicion 0 y 1 del registro GPIO_POTJ_AFSEL_R para controlarlo con los registros GPIO
	GPIO_PORTJ_DIR_R &=~0X03; ////coloca un zero en la posicion 0 y 1 del registro GPIO_POTJ_DIR_R para configurarlo como entrada
														//0 entrada			1 salidad
	GPIO_PORTJ_DEN_R |=0X03; ////coloca un UNO en la posicion 0 y 1 del registro 	GPIO_PORTJ_DEN_R para configurar como digital
														//0 analogica 1 digital 
	
	GPIO_PORTJ_PUR_R |=0X03;// COLOCO UN UNO en la posicion 0 y 1 del registro 	GPIO_PORTJ_PUR_R PARA CONFIGURAR resistencias pull up
	
}