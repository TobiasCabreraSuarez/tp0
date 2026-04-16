#include "client.h"

int main(void) {
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* clave;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	clave = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	log_info(logger, "El valor de la KEY del archivo 'cliente.config' es: %s ", clave);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(clave, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void) {
	t_log* nuevo_logger;
	
	nuevo_logger = log_create("cliente.log", "CLIENTE", 1, LOG_LEVEL_INFO);
	
	if(nuevo_logger == NULL) {
		perror("Hubo un problema. No se pudo crear o encontrar el archivo.");
		exit(EXIT_FAILURE);
	}
	
	return nuevo_logger;
}

t_config* iniciar_config(void) {
	t_config* nuevo_config;
	
	nuevo_config = config_create("cliente.config");

	if(nuevo_config == NULL) {
		perror("No se pudo crear el .config!");
		//exit(EXIT_FAILURE);
		abort();
	}
	
	return nuevo_config;
}

void leer_consola(t_log* logger) {
	char* leido;
	
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(leido != NULL && *leido != '\0') {
		//Si bien pude usar 'strcmp(logger, "") != 0', este es más costoso ya que lo único que me importa es saber si el usuario presionó Enter sin eescriibir nada.
		// 1. Logueamos lo que el usuario ingresa por consola
		log_info(logger, "> %s", leido);
		// 2. Liberamos la memoria para el siguiente ingreso por sonsola
		free(leido);
		// 3. Pedimos la siguiente línea
		leido = readline("> ");
	}
	
	// Liberamos la memoria del último línea vacía (al presionar Enter) para no dejar basura.
	free(leido);
	printf("Memoria liberada exitósamente!\n\n");

}

void paquete(int conexion) {
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	
	while(leido != NULL && *leido != '\0') {
		// 1. Tenemos que agregar el mensaje por consola al paquete creado
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		// 2. Liberamos la memoria para el siguiente ingreso por sonsola
		free(leido);
		// 3. Pedimos la siguiente línea
		leido = readline("> ");
	}
	
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	
	// ENVIAMOS PAQUETE
	enviar_paquete(paquete, conexion);

	// ELIMINAMOS PAQUETE
	eliminar_paquete(paquete);

}

void terminar_programa(int conexion, t_log* logger, t_config* config) {
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
	printf("Programa finalizado exitósamente!!\n\n");
}
