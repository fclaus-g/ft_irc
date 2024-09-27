# ft_irc desarrollo

Para la ejecución de este proyecto vamos a separarlo en cuatro partes:
* El servidor.
* El parseo de comandos.
* Los canales.
* El cliente.
Vamos a crear un MVP (un proyecto mínimo viable) con el que empezaremos a trabajar. Para ello vamos a comenzar con el server.

## The Server

* Gestión de conexiones de socket.
* Protección de las variables de nivel superior, como la contraseña del servidor. 
* Utiliza otro socket para comunicarse con el cliente.

La utilización de estos sockets implica:
* Crear el socket. La funcion *socket()* crea el socket TCP para el servidor. Establece un punto final de comunicación para que el server escuche las conexiones del cliente.
* Vincularlo a un puerto. La función *bind()* vincula el socket creado a un puerto y a una IP, esto asocia el socket con una dirección de red.
* Escuchar las conexiones entrantes. *listen()+ prepara el socket para aceptar conexiones entrantes, especifica el número máximo de conexiones que se pueden poner en cola para su aceptación.
* Aceptar las conexiones entrantes. Cada conexión aceptada es manejada por un nuevo socket, lo que permite que varios clientes interactúen con el server simultaneamente. La función *accept()* espera a que un cliente se conecte. Cuando se establece la conexión crea un nuevo socket(client_fd) para manejar la comunicacion con el cliente conectado.
* Leer datos del socket del cliente. La función *read()*  se encarga de esto recuperando los datos enviados por el cliente que luego pueden ser procesados por el server.

### Atributos

```cpp
std::string host;//Ip en la que el server escuchará conexiones entrantes.
int port;//puerto en el que el server estará escuchando
int server_socket;//socket principal, se usa para aceptar nuevas conexiones
/*Necesitamos algo para almacenar los clientes conectados(representados por su socket o ID), podria ser un mapa, un vector u otros. El mapa tiene un std::string y un int clave asociado. Podria servir bastante bien.*/
std::map<int, Client> clients;//Client es una clase la veremos mas adelante
/*Necesitamos almacenar los canales y los usuarios asociados a cada canal, de primeras probare con un std::map también*/
std::map<std::string, Channel> channels; //Channel será otra clase
/*Un conjunto de fd(sockets) para monitorear multiples conexiones simultáneas usando select(), poll() o epoll()*/
fd_set fds_set;
int max_fd; //para seguir el fd mas alto
std::string password; //contraseña del servidor
bool is_running;//flag para saber si el server esta corriendo o debe detenerse.
std::string server_name;
```

## The sockets

Son una abstracción que permite la comunicación entre diferentes procesos, ya sea en la misma máquina o en máquinas diferentes a través de una red. En el contexto del servidor los sockets se utilizan para aceptar conexiones entrantes y para comunicarse con los clientes conectados. De hecho y simplificando es un fd que permite la comunicación remota.
Hay varios tipos de sockets entre ellos los principales son:
* **Sockets de corriente**-> *SOCK_STREAM*. Utilizan el TCP (capa de transtporte) para comunicarse. Este protocolo hace cumplir una transmisión de datos confiable y conectada, a costa de un rendimiento ligeramente reducido.
* **Sockets de datagramas**-> *SOCK_DGRAM*. Utiliza UDP (capa de transporte). A diferencia de TCP, UDP permite la transmisión de datos sin conexión, que es rápida, pero sin garantia de recepción. (sin el handshake de 3 vias de TCP).

* **Conceptos clave**

1. **Creación de Socket** -> *socket()* -> return(fd) del punto de conexión.
2. **Enlace (binding)**-> *bind()* Enlaza el socket con una IP y un puerto específico.
3. **Escucha** -> *listen()* Pone el socket en modo escucha, listo para aceptar conexiones entrantes.
4. **Aceptación de Conexiones** -> *accept()* acepta una conexiçon entrante y crea un nuevo socket para manejar la comunicacion con el cliente.
5. **Lectura y escritura** -> *read() y write()* leen y escriben datos en el socket.
6. **Cierre del socket** -> *close()*.

### <sys/sockets.h>

Este header define varias structs, tipos de datos y funciones para trabajar con sockets.
* **Creación de Socket**
    * **int socket(int domain, int type, int protocol)**
        + **domain** -> Dominio de comunicación deseado. Permite seleccionar la familia de protocolos que se utilizará en la comunicación.
            - 
        + **type** -> Especifica la semántica de la comunicación, sus valores más comunes son SOCK_STREAM y SOCK_DGRAM.
            - 
        + **protocol** -> Protocolo particular a utilizar. Generalmente un dominio y tipo solo admite un protocolo particular, por lo que suele tomar el valor 0.

### <netinet/in.h>

Es parte de la familia de headers de la familia BSD(Berkeley Software Distribution). Define archivos y structs utilizados en programación de redes y sockets.
* Tipos definidos:
    + in_port_t-> Unsigned int de 16bits, usado para representar tipos de puerto.
    + in_addr_t-> Unsigned int de 32bits para representar direcciones IP.

* Structs definidas:
    + in_addr-> Contiene al menos el miembro s_addr de 32bits para la IP
    + sockaddr_in-> Contiene los siguientes miembros
        - sin_family-> int que indica la familia de direcciones(AF_INET para IPv4)
        - sin_port->
        - sin_addr->
* Macros
El archivo <netinet/in.h> también define macros para utilizar como valores del argumento level en las funciones getsockopt() y setsockopt().