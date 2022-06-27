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

#define DemoraColaAeropuerto					1	/* Sampst 1: Demora en cola del aeropuerto       */
#define DemoraColaTaxis							2	/* Sampst 2: Demora en cola	de la estación de taxis		*/




/* Declaraci¢n de variables propias */

float media_interarribos_aeropuerto, media_interarribos_estacion, media_interarribos_colectivos, media_servicio, max_colectivo, min_colectivo;
int num_pasajeros, cantidad_viajes,cantidad_cola_aeropuerto,cantidad_cola_taxi, i;
bool mantenimiento = false;

/* Declaraci¢n de Funciones propias */

void inicializa(void);
void Rutina_arribos_aeropuerto(void);
void Rutina_arribos_taxi(void);
void Rutina_llegadas_colectivo_aeropuerto(void);
void Rutina_llegadas_colectivo_estacion(void);
void Rutina_arribo_mantenimiento(void);
void Rutina_fin_mantenimiento(void);
void reporte(void);


int main()  /* Main function. */
{
	/* Apertura de Archivos que sean necesarios */
	
	media_interarribos_aeropuerto = 12.8;
	media_interarribos_estacion = 7.9;
	min_colectivo = 10;
	max_colectivo = 15;
	cantidad_cola_aeropuerto = 0;
	cantidad_viajes = 0;
	cantidad_cola_taxi = 0;
	num_pasajeros = 0;

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
				Rutina_arribos_taxi();
				break;
			case LlegadaColectivoEstacion:
				Rutina_llegadas_colectivo_aeropuerto();
				break;
			case ArriboPasajerosEstacion:
				Rutina_llegadas_colectivo_estacion();
				break;
			case ArriboMantenimiento:
				Rutina_arribo_mantenimiento();
				break;
			case FinMantenimiento:
				Rutina_fin_mantenimiento();
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

void Rutina_arribos_taxi(void)
{
	
	// Determinar pr¢ximo arribo y cargar en Lista de Eventos
	
	transfer[1] = sim_time + expon(media_interarribos_aeropuerto,ArriboPasajerosEstacion);
	transfer[2] = ArriboPasajerosEstacion;
	list_file(INCREASING,LIST_EVENT);

	//guardar valores en cola taxis teniendo en cuenta las probabilidades de que lleguen uno o más pasajeros por arribo

	float prob;

	prob = lcgrand(1);

	if (prob <= 0.3)
	{
		transfer[1] = sim_time;
		list_file(LAST, ColaTaxis);
		cantidad_cola_taxi++;
	}
	else
	{
		if (prob <= 0.75)
		{
			for (i = 0; i < 2; i++)
			{
				transfer[1] = sim_time;
				list_file(LAST, ColaTaxis);
				cantidad_cola_taxi++;
			}
		}
		else
		{
			if (prob <= 0.90)
			{
				for (i = 0; i < 3; i++)
				{
					transfer[1] = sim_time;
					list_file(LAST, ColaTaxis);
					cantidad_cola_taxi++;
				}
			}
			else
			{
				for (i = 0; i < 4; i++)
				{
					transfer[1] = sim_time;
					list_file(LAST, ColaTaxis);
					cantidad_cola_taxi++;
				}
			}
		}
	}
}

void Rutina_arribos_aeropuerto(void)
{

	// Determinar próximo arribo y cargar en Lista de Eventos
	
	transfer[1] = sim_time + expon(media_interarribos_aeropuerto,ArriboPasajerosAeropuerto);
	transfer[2] = ArriboPasajerosAeropuerto;
	list_file(INCREASING,LIST_EVENT);

	//guardar valores en cola aeropuerto teniendo en cuenta las probabilidades de que lleguen uno o más pasajeros por arribo
	
	float prob;

	prob = lcgrand(1);

	if (prob <= 0.3)
	{
		transfer[1] = sim_time;
		list_file(LAST, ColaAeropuerto);
		cantidad_cola_aeropuerto++;
	}
	else
	{
		if (prob <= 0.75)
		{
			for (i = 0; i < 2; i++)
			{
				transfer[1] = sim_time;
				list_file(LAST, ColaAeropuerto);
				cantidad_cola_aeropuerto++;
			}
		}
		else
		{
			if (prob <= 0.90)
			{
				for (i = 0; i < 3; i++)
				{
					transfer[1] = sim_time;
					list_file(LAST, ColaAeropuerto);
					cantidad_cola_aeropuerto++;
				}
			}
			else
			{
				for (i = 0; i < 4; i++)
				{
					transfer[1] = sim_time;
					list_file(LAST, ColaAeropuerto);
					cantidad_cola_aeropuerto++;
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

	//vacia el colectivo

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
			sampst(sim_time - transfer[1], DemoraColaAeropuerto);
		}
	}
	else
	{
		for (i = 0; i < 25; i++)
		{
			list_remove(LAST, ColaAeropuerto);
			sampst(sim_time - transfer[1], DemoraColaAeropuerto);
		}
	}
}

void Rutina_llegadas_colectivo_estacion(void) {

	//acumula el número de viajes

	cantidad_viajes++;

	//Se vacia el colectivo

	int tamaño_colectivo = list_size[Colectivo];
	int tamaño_cola_taxi = list_size[ColaTaxis];

	for  (i = 0; i < tamaño_colectivo; i++)
	{
		list_remove(FIRST, Colectivo);
	}

	// Prtegunta para ver si esta en mantenimiento
	//Reduce la cola de pasajeros de la estación de taxis y aumenta la ocupación del colectivo

	if (!mantenimiento)
	{
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
				sampst(sim_time - transfer[1], DemoraColaTaxis);
			}
		}
		else
		{
			for (i = 0; i < 25; i++)
			{
				list_remove(LAST, ColaTaxis);
				sampst(sim_time - transfer[1], DemoraColaTaxis);
			}
		}

		//genera próximo arribo de colectivo a aeropuerto

		transfer[1] = sim_time + uniform(min_colectivo, max_colectivo, LlegadaColectivoAeropuerto);
		transfer[2] = LlegadaColectivoAeropuerto;
		list_file(INCREASING, LIST_EVENT);
	}

}

void Rutina_arribo_mantenimiento(void) 
{
	// establece que empieza el mantenimiento

	mantenimiento = true;

	/* Programa el fin del mantenimiento. 15 minutos */
	transfer[1] = sim_time + 15;
	transfer[2] = FinMantenimiento;
	list_file(INCREASING, LIST_EVENT);
	
	/* Programa el próximo mantenimiento. 4 horas*/
	transfer[1] = sim_time + 240; 
	transfer[2] = ArriboMantenimiento;
	list_file(INCREASING, LIST_EVENT);
}

void Rutina_fin_mantenimiento(void)
{

	// establece que se termino el mantenimiento

	mantenimiento = false;

	// continúa con el trabajo cotidiano

	int tamaño_cola_taxi = list_size[ColaTaxis];
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
			sampst(sim_time - transfer[1], DemoraColaTaxis);
		}
	}
	else
	{
		for (i = 0; i < 25; i++)
		{
			list_remove(LAST, ColaTaxis);
			sampst(sim_time - transfer[1], DemoraColaTaxis);
		}
	}

	//genera próximo arribo de colectivo a aeropuerto

	transfer[1] = sim_time + uniform(min_colectivo, max_colectivo, LlegadaColectivoAeropuerto);
	transfer[2] = LlegadaColectivoAeropuerto;
	list_file(INCREASING, LIST_EVENT);

}

void reporte( void )
{
	//Cantidad promedio de pasajeros transportados por el micro.

	printf("El promedio de pasajeros transportados por el micro es de: %i",(num_pasajeros/cantidad_viajes));

	//Número medio en cada una de las colas y entre las 2 colas juntas.

	printf("\nEl número medio de la cola en el aeropuerto es de: %i",cantidad_cola_aeropuerto/cantidad_viajes );
	printf("\nEl número medio de la cola en la estación de taxis es de: %i",cantidad_cola_taxi/cantidad_viajes);
	printf("\nEl número medio en ambas colas es de: %i",(cantidad_cola_aeropuerto+cantidad_cola_taxi)/cantidad_viajes );

	//Demora media y máxima de las personas en cola, en cualquiera de los lugares.

	sampst(0.0,- DemoraColaAeropuerto);
	printf("\nLa demora media en la cola del aeropuerto es de: %f", (transfer[1]));
	printf("\nLa demora máxima en la cola del aeropuerto es de: %f\n", (transfer[3]));

	sampst(0.0, -DemoraColaTaxis);
	printf("\nLa demora media en la cola de una estación de taxi es de: %f", (transfer[1]));
	printf("\nLa demora máxima en las cola de una estación de taxi es de: %f\n", (transfer[3]));
	
}
