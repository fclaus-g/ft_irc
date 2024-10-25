#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ft_irc.hpp"
/** + Debe poder autenticarse, establecer un apodo, un nombre de usuario, unirse a un canal, enviar y recibir mensajes privados utilizando el cliente de referencia.
	+ Todos los mensajes enviados desde un cliente a un canal deben reenviarse a todos los demás clientes que se hayan unido al canal.
	+ Debe tener operadores y usuarios regulares.
	+ Luego, debe implementar los comandos que son específicos para los operadores de canal:
		* KICK: expulsa a un cliente del canal
		* INVITE: invita a un cliente a un canal
		* TOPIC: cambia o visualiza el tema del canal
		* MODE: cambia el modo del canal:
			* i: establece o elimina el canal solo para invitados
			* t: establece o elimina las restricciones del comando TOPIC para los operadores de canal
			* k: establece o elimina la clave del canal (contraseña)
			* o: otorga o quita privilegios de operador de canal
			* l: Establecer/eliminar el número de users para el canal.*/
class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		std::vector<User>	_users;
		std::vector<User>	_op;
		std::map<int, bool> _usersMap; //
		bool				_inviteMode;
		bool				_topicMode;
		bool				_keyMode;
		int					_usersLimit;
		std::string			_password;
	public:
		Channel();
		Channel(std::string name);
		Channel(const Channel &rhs);
		~Channel();
		//getters
		Channel& operator=(const Channel &rhs);
		const std::string&			getName() const;
		const std::string&			getTopic() const;
		const std::vector<User>&	getUsers() const;
		bool						getInviteMode() const;
		bool						getTopicMode() const;
		bool 						getKeyMode() const;
		int 						getUsersLimit() const;
		const std::string& getPassword() const;
		//setters
		void 						setName(const std::string& name);
		void 						setTopic(const std::string& topic);
		void 						setInviteMode(const bool inviteMode);
		void 						setTopicMode(const bool topicMode);
		void 						setKeyMode(const bool keyMode);
		void 						setUsersLimit(const int usersLimit);
		void 						setPassword(const std::string& password);
		//check methods
		bool 						isUserInChannel(User& user);
		bool 						isOp(User& user);
		bool 						channelIsFull();
		//methods
		void 						addUserChannel(User& user);
		void 						removeUserChannel(User& user);
		void 						addOpChannel(User& user);
		void 						removeOpChannel(int userFd);
};

std::ostream& operator<<(std::ostream& out, const Channel& channel);

#endif
