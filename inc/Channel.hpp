#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
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
		bool        _isPrivate;
		size_t      _maxUsers;
		std::string _channelName;
		std::string _channelPass;
		std::string _topic;
		std::map<User&, bool> _users;
		std::vector<User> _channelUsers;
		std::vector<User> _channelOp;
	public:
		Channel();
		Channel(std::string name);
		Channel(const Channel& rhs);
		Channel& operator=(const Channel& rhs);
		~Channel();
		
		const bool& getIsPrivate() const;
		const size_t& getMaxUsers() const;
		const std::string& getName() const;
		const std::string& getPass() const;
		const std::string& getTopic() const;
		const std::vector<User>& getUsers() const;
		const std::vector<User>& getOp() const;

		void setIsPrivate(bool isPrivate);
		void setMaxUsers(size_t maxUsers);
		void setName(const std::string& name);
		void setPass(const std::string& pass);
		void setTopic(const std::string& topic);
		
		
		void addUserChannel(int userFd);
		void removeUserChannel(int userFd);
		void addOpChannel(int userFd);
		void removeOpChannel(int userFd);
		void kickUserChannel(int userFd);
		void inviteUserChannel(int userFd);
		void changeTopicChannel(const std::string& topic);
		void changeModeChannel(const std::string& mode);
		void changePassChannel(const std::string& pass);
		void changeMaxUsersChannel(size_t maxUsers);
		void changeNameChannel(const std::string& name);
		void printUsersChannel();
		void printOpsChannel();
};

std::ostream& operator<<(std::ostream& out, const Channel& channel);

#endif
