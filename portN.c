#include <stdint.h>

#define GPIO_PORTN1 				(*((volatile uint32_t *)0x40064008))// PARA ESCRIBIR EN el puerto N1

#define GPIO_PORTN_AFSEL_R  (*((volatile uint32_t *)0x40064420)) //para contrOlarlo con los REGISTROS GPIO
#define GPIO_PORTN_DIR_R 		(*((volatile uint32_t *)0x40064400))	 //PARA CONFIGURAR COMO SALIDA O ENTRADA EL PUERTO N
#define GPIO_PORTN_DEN_R 		(*((volatile uint32_t *)0x4006451C))  //PARA CONFIGURAR DIGITAL O ANALOGICA EL PUERTO N
#define SYSCTL_PORTN_PCTL_R (*((volatile uint32_t *)0x4006452C))
#define GPIO_PORTN_PUR_R 		(*((volatile uint32_t *)0x40064510))  //PARA CONFIGURAR RESISTENCIAL PULL UP
#define SYSCTL_RCGCGPIO_R 	(*((volatile uint32_t *)0x400FE608)) //PARA CONFIGURAR ENCENDIDO DEL PUERTO

#define SYSCTL_PRGPIO_R 			(*((volatile uint32_t *)0x400FEA08)) // PARA VERIFICAR QUE LOS PUERTOS ESTEN ENCENDIDOS


// configuracion de LEDs
void configN(void){
	SYSCTL_RCGCGPIO_R |=0X00001000; //HABILITA EL PUERTO N
	while ((SYSCTL_PRGPIO_R&0X00001000)==0){};
	GPIO_PORTN_AFSEL_R &=~0x02; //coloca un zero en la posicion 1
//		del registro GPIO_POTJ_AFSEL_R para controlarlo con los registros GPIO
	GPIO_PORTN_DIR_R |=0X02; ////coloca un uno en la posicion 1 del registro GPIO_POTJ_DIR_R para configurarlo como entrada
														//0 entrada			1 salidad
	GPIO_PORTN_DEN_R |=0X02; ////coloca un UNO en la posicion 1 del registro 	GPIO_PORTJ_DEN_R para configurar como digital
														//0 analogica 1 digital 0
}