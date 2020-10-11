#include "tm4c1294ncpdt.h"
#include <stdint.h>

//int programarHora(int current){
//		uint8_t hour = 0, min = 0, seg = 0;
//		uint32_t day = 0, temp = 0;
//		TIMER0_CTL_R &= ~TIMER_CTL_TAEN; //Apaga el timer
//				day = ( current / 86400);
//				temp = ( current % 86400);
//				hour = temp / 3600;
//				temp = temp % 3600;
//				min = temp / 60;
//				seg = temp % 60;	
//		int botonAumentarHora  = GPIO_PORTJ_DATA_R & 0x02;
//		//int botonProgramarHora = GPIO_PORTJ_DATA_R & 0x01;
//		while(1){
//			PortN_Output(0x01);
//			if(!botonAumentarHora){ // Presion bot?n 4
//				current = current + 3600;
//				day = ( current / 86400);
//				temp = ( current % 86400);
//				hour = temp / 3600;
//				temp = temp % 3600;
//				min = temp / 60;
//				seg = temp % 60;	
//				mostrar_hora_t(hour,min,seg);

//			}				
//			// Bot?n para salir de modo de programaci?n
//			int boton  = GPIO_PORTJ_DATA_R & 0x01;
//			if(!boton){
//				break;
//			}
//		}
//		return current ;
//}
