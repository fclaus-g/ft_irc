# ft_irc

Debes evaluar a 2 estudiantes en este equipo.

## Introducción

Por favor, siga las reglas a continuación:

Manténgase educado, cortés, respetuoso y constructivo durante todo el proceso de evaluación. El bienestar de la comunidad depende de ello.

Trabaje con el estudiante o el grupo que se está evaluando para identificar posibles problemas en su proyecto. Tómese el tiempo para analizar y debatir los problemas identificados.

Comprenda que puede haber diferencias en la forma en que los compañeros interpretan las instrucciones y el alcance del proyecto. Mantenga siempre una mente abierta y califique de la manera más honesta posible. La pedagogía es eficaz solo cuando las evaluaciones de los compañeros se toman en serio.


## Pautas

Por favor, siga las pautas a continuación:

Calificar únicamente el trabajo enviado al repositorio Git del estudiante o grupo evaluado.

Verifique que el repositorio Git pertenezca al estudiante y que el proyecto sea el esperado. Asegúrese de que Git clone se use en una carpeta vacía.

Verifique cuidadosamente que no se utilicen alias maliciosos para engañar al evaluador y lograr que califique contenido no oficial.

Si corresponde, revise junto con el estudiante cualquier script utilizado para pruebas o automatización.

Si no has completado la tarea que estás evaluando, lee todo el tema antes de comenzar la evaluación.

Utilice las banderas disponibles para informar sobre un repositorio vacío, un programa que no funciona, un error de Norm o trampas. El proceso de evaluación finaliza con una calificación final de 0 (o -42 por trampas). Sin embargo, excepto en casos de trampas, se anima a los estudiantes a revisar el trabajo en conjunto para identificar errores que se deben evitar en el futuro.

Recuerde que no se tolerarán errores de segmentación ni otras terminaciones inesperadas del programa durante la evaluación. Si esto ocurre, la calificación final será 0. Utilice la bandera correspondiente.

No debería ser necesario editar ningún archivo, excepto el archivo de configuración, si existe. Si es necesario editar un archivo, explique las razones al estudiante evaluado y asegúrese de que haya un acuerdo mutuo.

Verificar la ausencia de fugas de memoria. Toda la memoria asignada en el montón debe liberarse correctamente antes de que finalice el programa.

Puede utilizar herramientas como leaks, valgrind o e_fence para comprobar si hay fugas de memoria. Si encuentra fugas de memoria, marque la casilla correspondiente.

## Parte obligatoria

### Networking specials

Las comunicaciones en red pueden verse perturbadas por muchas situaciones extrañas.

* Al igual que en el subject, utilizando nc, intenta enviar comandos parciales. Comprueba que el servidor responde correctamente. Con un comando parcial enviado, asegúrate de que las demás conexiones sigan funcionando correctamente.

* Matar inesperadamente a un cliente. Luego, verificar que el servidor aún esté operativo para las demás conexiones y para cualquier nuevo cliente entrante.

* Matar inesperadamente a un NC con solo la mitad de un comando enviado. Verifique nuevamente que el servidor no esté en un estado extraño o bloqueado.

* Detenga un cliente (^-Z) conectado a un canal. Luego, inunde el canal con otro cliente. El servidor no debería bloquearse. Cuando el cliente vuelva a estar activo, todos los comandos almacenados deberían procesarse con normalidad. Además, verifique si hay fugas de memoria durante esta operación.

### Operador del canal de comandos del cliente

* Con nc y el cliente IRC de referencia, verifique que un usuario normal no tenga privilegios para realizar acciones de operador de canal. 
* Luego, realice una prueba con un operador. Se deben probar todos los comandos de operación del canal (elimine un punto por cada función que no esté funcionando).

Calificalo desde 0 (reprobado) hasta 5 (excelente)

### Comprobaciones básicas

* Hay un Makefile, el proyecto se compila correctamente con las opciones requeridas, está escrito en C++ y el ejecutable se llama como se espera.

* Pregunte y verifique cuántos poll() (o equivalentes) hay en el código. Debe haber solo uno.

* Verifique que se llame a poll() (o equivalente) cada vez antes de cada aceptación, lectura/recepción, escritura/envío. Después de estas llamadas, no se debe utilizar errno para activar una acción específica (por ejemplo, volver a leer después de errno == EAGAIN).

* Verifique que cada llamada a fcntl() se realice de la siguiente manera: fcntl(fd, F_SETFL, O_NONBLOCK); Cualquier otro uso de fcntl() está prohibido.

Si alguno de estos puntos es incorrecto, la evaluación termina ahora y la nota final es 0.


Sí	No

### Comandos básicos del cliente

* Con nc y el cliente IRC de referencia, comprueba que puedes autenticarte, establecer un apodo, un nombre de usuario y unirte a un canal. Esto debería funcionar bien (ya deberías haberlo hecho anteriormente).

* Verificar que los mensajes privados (PRIVMSG) sean completamente funcionales con diferentes parámetros.

Sí	No
## Parte extra

### Un pequeño robot

Hay un bot de IRC.


Calificalo desde 0 (reprobado) hasta 5 (excelente)

### Transferencia de archivos

Se evaluará la parte adicional si, y solo si, la parte obligatoria se ha realizado completa y perfectamente, y la gestión de errores se ocupa de un uso inesperado o incorrecto. En caso de que no se hayan superado todos los puntos obligatorios durante la defensa, los puntos adicionales deben ignorarse por completo.

La transferencia de archivos funciona con el cliente IRC de referencia.

Sí	No
