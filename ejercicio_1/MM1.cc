/* Modelo MM1 - Un Servidor, Una Cola  */


#include "simlib.c"
#include <stdio.h>
#include <stdlib.h> 


#define ArriboPasajerosAeropuerto            1  /* Tipo de Evento 1: Arribo pasajeros aeropuerto           */
#define LlegadaColectivoAeropuerto           3  /* Tipo de Evento 3: Llegada colectivo aeropuerto          */
#define LlegadaColectivoEstacion            4  /* Tipo de Evento 4: Arribo pasajeros estacion          */
#define ArriboPasajerosEstacion           2  /* Tipo de Evento 2: Llegada colectivo aeropuerto          */
#define ArriboMantenimiento				5  /* Tipo de Evento 5: Arribo para mantenimiento			*/
#define FinMantenimiento           6  /* Tipo de Evento 6: Finalización del mantenimiento          */

#define Colectivo					1	/* Lista 1: Colectivo                  */
#define ColaAeropuerto              2  /* Lista 2: Cola en el aeropuerto                       */
#define ColaTaxis					3	/* Lista 3: Cola en la estacion de taxis                      */

#define DemoraCola					1	/* Sampst 1: Demora en cola        */




/* Declaraci¢n de variables propias */

float media_interarribos_aeropuerto, media_interarribos_estacion, media_interarribos_colectivos, media_servicio, max_colectivo, min_colectivo;
int num_pasajeros, cantidad_viajes, i;
bool mantenimiento = false;

/* Declaraci¢n de Funciones propias */

void inicializa(void);
void Rutina_arribos_aeropuerto(void);
void Rutina_arribos_estacion(void);
void Rutina_llegadas_colectivo_aeropuerto(void);
void Rutina_llegadas_colectivo_estacion(void);
void Rutina_arribo_mantenimiento(void);
void Rutina_fin_mantenimiento(void);
void reporte(void);


int main()  /* Main function. */
{
	/* Apertura de Archivos que sean necesarios */

	float minimo, maximo;
	

	// Open for write 
 
   //fprintf(outfile,"The file 'data2' was not opened\n" );
	
	media_interarribos_aeropuerto = 12.8;
	media_interarribos_estacion = 7.9;
	min_colectivo = 10;
	max_colectivo = 15;

	/* Initializar Simlib */
	init_simlib();

	/* Initializar el Sistema */
	inicializa();

	/* Ejecutar la simulaci¢n. */

	//ver lo del corte cada 4 hs - se trabaja las 24 horas - Simular una semana arrancando el micro a funcionar a los 30 minutos de inicida la simulación
	// en este caso la media de servicio se obtiene directamente cuando sea necesaria con la función uniform(maximo,minimo,semilla(normalmente numero de evento))
	// para el while se puede decir que se un evento con determinado tiempo o controlar que sea menor a siete
	//sim_time = 15 ----->avanza 15 minutos o la unidad que sea
	while (sim_time < 10080)
	{

		/* Determinar pr¢ximo Evento */
		timing();

		/* Invoke the appropriate event function. */


		switch( next_event_type )
		{
			case ArriboPasajerosAeropuerto:
				Rutina_arribos_aeropuerto();
				break;
			case LlegadaColectivoAeropuerto:
				Rutina_arribos_estacion();
				break;
			case LlegadaColectivoEstacion:
				Rutina_llegadas_colectivo_aeropuerto();
				break;
			case ArriboPasajerosEstacion:
				Rutina_llegadas_colectivo_estacion();
				break;
			case ArriboMantenimiento:
				Arribo_Mantenimiento();
				break;
			case FinMantenimiento:
				Fin_Mantenimiento();
				break;
        }
	}

	/* Llamada al Reporte para mostrar los resultados */
	reporte();
	//getchar();
	system("pause");
}

void inicializa(void)  /* Inicializar el Sistema */
{
	//Se carga el primer Arribo en la Lista de Eventos. En este caso los arribos son por grupos

	transfer[1] = sim_time + expon(media_interarribos_aeropuerto, ArriboPasajerosAeropuerto);
	transfer[2] = ArriboPasajerosAeropuerto;
	list_file(INCREASING,LIST_EVENT);

	//inicializar el primer arribo de pasajeros a la estación de taxis

	transfer[1] = sim_time + expon(media_interarribos_estacion, ArriboPasajerosEstacion);
	transfer[2] = ArriboPasajerosEstacion;
	list_file(INCREASING, LIST_EVENT);

	//inicializar primer llegada del colectivo a la estación de taxis 30 minutos despues de iniciar la simulación

	transfer[1] = sim_time + 30;
	transfer[2] = LlegadaColectivoEstacion;
	list_file(INCREASING, LIST_EVENT);

	//inicializar el primer mantenimiento en 4 hs despues de iniciar a trabajar el colectivo

	transfer[1] = sim_time + 270;
	transfer[2] = ArriboMantenimiento;
	list_file(INCREASING, LIST_EVENT);
}

void Rutina_arribo_taxi(void)
{
	
	// Determinar pr¢ximo arribo y cargar en Lista de Eventos
	
	transfer[1] = sim_time + expon(media_interarribos_aeropuerto,ArriboPasajerosEstacion);
	transfer[2] = ArriboPasajerosEstacion;
	list_file(INCREASING,LIST_EVENT);

	//guardar valores en cola taxis teniendo en cuenta las probabilidades de que lleguen uno o más pasajeros por arribo

	float prob;

	prob = lcgrand(ArriboPasajerosEstacion);

	if (prob <= 0.1)
	{
		for (i = 0; i < 4; i++)
		{
			list_file(LAST, ColaTaxis);
		}

	}
	else
	{
		if (prob <= 0.25)
		{
			for (i = 0; i < 3; i++)
			{
				list_file(LAST, ColaTaxis);
			}
		}
		else
		{
			if (prob <= 0.55)
			{
				list_file(LAST, ColaTaxis);
			}
			else
			{
				for (i = 0; i < 2; i++)
				{
					list_file(LAST, ColaTaxis);
				}
			}
		}
	}
	
	/* Chequear si el Servidor est  desocupado */
	/*
	if (list_size[Servidor] == 0)
	{

	   ++clientes_act;
	   */
	   /* Si está desocupado ocuparlo y generar la partida */
	/*
	   list_file(FIRST, Servidor);
	   sampst(0.0, Demora_cola);


	   transfer[1] = sim_time + expon(media_servicio, Partida);
	   transfer[2] = Partida;
	   list_file(INCREASING,LIST_EVENT);

	}

	else
	{
		*/
	   /* Si el Servidor est  ocupado poner el Trabajo en Cola */
		/*
	   transfer[1] = sim_time;
	   list_file(LAST, Cola);
	}
	*/
}

void Rutina_arribos_aeropuerto(void)
{

	// Determinar próximo arribo y cargar en Lista de Eventos
	
	transfer[1] = sim_time + expon(media_interarribos_aeropuerto,ArriboPasajerosAeropuerto);
	transfer[2] = ArriboPasajerosAeropuerto;
	list_file(INCREASING,LIST_EVENT);

	//guardar valores en cola aeropuerto teniendo en cuenta las probabilidades de que lleguen uno o más pasajeros por arribo
	
	float prob;

	prob = lcgrand(ArriboPasajerosAeropuerto);

	if (prob<=0.1) 
	{
		for ( i = 0; i < 4; i++)
		{
			list_file(LAST, ColaAeropuerto);
		}
	}
	else 
	{
		if (prob <= 0.25) 
		{
			for (i = 0; i < 3; i++)
			{
				list_file(LAST, ColaAeropuerto);
			}
		}
		else
		{
			if (prob <= 0.55) 
			{
				list_file(LAST, ColaAeropuerto);
			}
			else 
			{
				for (i = 0; i < 2; i++)
				{
					list_file(LAST, ColaAeropuerto);
				}
			}
		}
	}
}

void Rutina_llegadas_colectivo_aeropuerto(void) 
{
	//genera próximo arribo de colectivo a estación de taxis

	transfer[1] = sim_time + uniform(min_colectivo,max_colectivo,LlegadaColectivoEstacion);
	transfer[2] = LlegadaColectivoEstacion;
	list_file(INCREASING, LIST_EVENT);

	//acumula el número de viajes

	cantidad_viajes++;

	//reduce la cola colectivo

	int tamaño_colectivo =list_size[Colectivo];
	int tamaño_cola_aeropuerto =list_size[ColaAeropuerto];

	for  (i = 0; i < tamaño_colectivo; i++)
	{
		list_remove(FIRST, Colectivo);
	}

	//reduce la cola de pasajeros de la estación de taxis y aumenta la ocupación del colectivo

	if (list_size[ColaAeropuerto] <= 25) 
	{
		for ( i = 0; i < tamaño_cola_aeropuerto; i++)
		{
			list_file(FIRST, Colectivo);
			num_pasajeros++;
		}
	}
	else 
	{
		for (i = 0; i < 25; i++)
		{
			list_file(FIRST, Colectivo);
			num_pasajeros++;
		}
	}

	if (list_size[ColaAeropuerto] <= 25)
	{
		for (i = 0; i < tamaño_cola_aeropuerto; i++)
		{
			list_remove(LAST, ColaAeropuerto);
		}
	}
	else
	{
		for (i = 0; i < 25; i++)
		{
			list_remove(LAST, ColaAeropuerto);
		}
	}
}

void Rutina_llegadas_colectivo_estacion(void) {

	//genera próximo arribo de colectivo a aeropuerto

	transfer[1] = sim_time + uniform(min_colectivo, max_colectivo, LlegadaColectivoAeropuerto);
	transfer[2] = LlegadaColectivoAeropuerto;
	list_file(INCREASING, LIST_EVENT);

	//acumula el número de viajes

	cantidad_viajes++;

	//LO DE LA VARIABLE BANDERA

	//reduce la cola de pasajeros de la estación de taxis y aumenta la ocupación del colectivo

	int tamaño_colectivo = list_size[Colectivo];
	int tamaño_cola_taxi = list_size[ColaTaxis];

	for  (i = 0; i < tamaño_colectivo; i++)
	{
		list_remove(FIRST, Colectivo);
	}

	if (list_size[ColaTaxis] <= 25)
	{
		for (i = 0; i < tamaño_cola_taxi; i++)
		{
			list_file(FIRST, Colectivo);
			num_pasajeros++;
		}
	}
	else
	{
		for (i = 0; i < 25; i++)
		{
			list_file(FIRST, Colectivo);
			num_pasajeros++;
		}
	}
	
	if (list_size[ColaTaxis] <= 25)
	{
		for (i = 0; i < tamaño_cola_taxi; i++)
		{
			list_remove(LAST, ColaTaxis);
		}
	}
	else
	{
		for (i = 0; i < 25; i++)
		{
			list_remove(LAST, ColaTaxis);
		}
	}

}
//preguntar por la bandera al cargar pasajeros. Si es true, no se genera nuevo arribo. hacer if

void Arribo_Mantenimiento(void) 
{
	/* 15 minutos */
	transfer[1] = sim_time + 15;
	transfer[2] = FinMantenimiento;
	list_file(INCREASING, LIST_EVENT);
	
	/* 4 horas */
	transfer[1] = sim_time + 255; 
	transfer[2] = ArriboMantenimiento;
	list_file(INCREASING, LIST_EVENT);

	mantenimiento = true;
}

void Fin_Mantenimiento(void)
{

	// establece que se termino el mantenimiento

	mantenimiento = false;

	//preguntar por la proxima carga de pasajeros directamente. Usar la lógica de ese evento en este

}

void reporte( void )
{
	//Cantidad promedio de pasajeros transportados por el micro.

	printf("El promedio de pasajeros transportados por el micro es de: ",num_pasajeros/cantidad_viajes);

	//Número medio en cada una de las colas y entre las 2 colas juntas.

	//printf("El número medio de la cola en el aeropuerto es de: ", );
	//printf("El número medio de la cola en la estación de taxis es de: ",);
	//printf("El número medio en ambas colas es de: ", );

	//Demora media y máxima de las personas en cola, en cualquiera de los lugares.

	//printf("La demora media en la cola del aeropuerto es de: ", );
	//printf("La demora máxima en la cola del aeropuerto es de: ", );
	//printf("La demora media en la cola de la estación de taxis es de: ", );
	//printf("La demora máxima en la cola de la estación de taxis es de: ", );


	/* Mostrar Par metros de Entrada */

	/* -------- Por pantalla -------- */

	/*
	printf( "Sistema M/M/1 - Simulaci¢n con Simlib \n\n" );
	printf( "Media de Interarribos          : %8.3f minutos\n", media_interarribos);
	printf( "Media de Servicios             : %8.3f minutos\n", media_servicio);
	printf( "Cantidad de Clientes Demorados : %i \n\n", num_clientes);

	/* Calcular los Estad¡sticos */

	/* Estad¡sticos Escalaras - Sampst */
	/*
	sampst(0.0,-Demora_cola);
	printf( "\nDemora en Cola                 : %f \n ",transfer[1]);


	/* Estad¡sticos Temporales - Timest y Filest */
	/*
	filest(Cola);
	printf( "\nN£mero Promedio en Cola        : %f \n ",transfer[1]);
	filest(Servidor);
	printf( "\nUtilizaci¢n Servidor           : %f \n ",transfer[1]);
	printf( "\nTiempo Final de Simulation     : %10.3f minutes\n", sim_time );


		/* Estad¡sticos Temporales - Timest y Filest */
		/*
	filest(Cola);
	printf( "\nN£mero Promedio en Cola        : %f \n ",transfer[1]);
	filest(Servidor);
	printf( "\nUtilizaci¢n Servidor           : %f \n ",transfer[1]);
	printf( "\nSimulation end time            : %10.3f minutes\n", sim_time );

	*/
}
