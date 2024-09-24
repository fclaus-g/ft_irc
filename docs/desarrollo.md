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


