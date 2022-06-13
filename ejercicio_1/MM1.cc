/* Modelo MM1 - Un Servidor, Una Cola  */


#include "simlib.c"
#include <stdio.h>
#include <stdlib.h> 


#define ArriboPasajerosAeropuerto            1  /* Tipo de Evento 1: Arribo pasajeros aeropuerto           */
#define LlegadaColectivoAeropuerto           3  /* Tipo de Evento 3: Llegada colectivo aeropuerto          */
#define LlegadaColectivoEstacion            4  /* Tipo de Evento 4: Arribo pasajeros estacion          */
#define ArriboPasajerosEstacion           2  /* Tipo de Evento 2: Llegada colectivo aeropuerto          */
#define Colectivo          1  /* Lista 1: Colectivo                  */
#define ColaAeropuerto              2  /* Lista 2: Cola en el aeropuerto                       */
#define ColaTaxis             2  /* Lista 2: Cola en la estacion de taxis                      */
#define DemoraCola      1  /* Sampst 1: Demora en cola        */




/* Declaraci¢n de variables propias */

float media_interarribos_aeropuerto, media_interarribos_estacion, media_interarribos_colectivos, media_servicio;
int num_clientes, clientes_act, i;




/* Declaraci¢n de Funciones propias */

void inicializa(void);
void Rutina_arribos_aeropuerto(void);
void Rutina_arribos_estacion(void);
void Rutina_llegadas_colectivo_aeropuerto(void);
void Rutina_llegadas_colectivo_estacion(void);
void reporte(void);


int main()  /* Main function. */
{
	/* Apertura de Archivos que sean necesarios */

	float minimo, maximo;
	

	// Open for write 
 
   //fprintf(outfile,"The file 'data2' was not opened\n" );
	
		

	media_interarribos_aeropuerto = 12.8;
	media_interarribos_estacion = 7.9;
	media_interarribos_colectivos = 12;
	num_clientes =10000;

	/* Initializar Simlib */
	init_simlib();

	/* Initializar el Sistema */
	inicializa();

	/* Ejecutar la simulaci¢n. */

	clientes_act = 0;

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
				Rutina_llegadas_colectivo_aeropuerto;
				break;
			case ArriboPasajerosEstacion:
				Rutina_llegadas_colectivo_estacion;
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
	/* Se carga el primer Arribo en la Lista de Eventos */

	transfer[1] = sim_time + expon(media_interarribos_aeropuerto, ArriboPasajerosAeropuerto);
	transfer[2] = ArriboPasajerosAeropuerto;
	list_file(INCREASING,LIST_EVENT);

	transfer[1] = sim_time + expon(media_interarribos_estacion, ArriboPasajerosEstacion);
	transfer[2] = ArriboPasajerosEstacion;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + expon(media_interarribos_colectivos, LlegadaColectivoEstacion);
	transfer[2] = ArriboPasajerosAeropuerto;
	list_file(INCREASING, LIST_EVENT);

}


void Rutina_arribos(void)  /* Evento Arribo */
{
	
	/* Determinar pr¢ximo arribo y cargar en Lista de Eventos */
	/*
	transfer[1] = sim_time + expon(media_interarribos_aeropuerto,Arribo);
	transfer[2] = Arribo;
	list_file(INCREASING,LIST_EVENT);
	*/
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


void Rutina_partidas(void)  /* Evento Partida */
{

	/* Desocupar el Servidor */
	/*
	list_remove(FIRST, Servidor);

	/* Ver si hay trabajos en cola */
		/*
	if (list_size[Cola] > 0)
	{
	   /* Sacar el primero de la cola y actualizar Demoras */
		/*
	   ++clientes_act;
	   list_remove(FIRST, Cola);
	   sampst(sim_time - transfer[1], Demora_cola);

	   /* Cargar en el Servidor y generar la partida */
	   /*
	   list_file(FIRST, Servidor);

	   transfer[1] = sim_time + expon(media_servicio, Partida);
	   transfer[2] = Partida;
	   list_file(INCREASING,LIST_EVENT);
	}
	*/
}


void reporte( void )  /* Generar Reporte de la Simulaci¢n */
{

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
