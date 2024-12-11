#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ft_irc.hpp"
/**
 * @brief TO_DO Eliminar los vectores de user y op y usar el map de users
 * para ello hay que cambiar varias cosas en el codigo dentro de channel.cpp
 * 
 */
class Channel
{
	private:
		std::string				_name;
		std::string				_topic;
		std::map<User*, bool>	_usersMap;
		bool					_inviteMode;
		bool					_topicMode;
		bool					_keyMode;
		int						_usersInChannel;
		int						_usersLimit;
		std::string				_password;
	public:
		Channel();
		Channel(const std::string& name);
		Channel(const Channel &rhs);
		~Channel();
		Channel& operator=(const Channel &rhs);
		
		//Getters
		const std::string				getName() const;
		const std::string				getTopic() const;
		const std::string				getUsersChannelStr() const;
		const std::string				getPassword() const;
		const std::map<User*, bool>&	getUsers() const;
		bool							getInviteMode() const;
		bool							getTopicMode() const;
		bool 							getKeyMode() const;
		int 							getUsersInChannel() const;
		int 							getUsersLimit() const;
		
		//Setters
		void							setName(const std::string& name);
		void							setTopic(const std::string& topic);
		void							setInviteMode(const bool inviteMode);
		void							setTopicMode(const bool topicMode);
		void							setKeyMode(const bool keyMode);
		void							setUsersInChannel(const int usersInChannel);
		void							setUsersLimit(const int usersLimit);
		void							setPassword(const std::string& password);
		//Check methods
		bool							isUserInChannel(User& user);
		bool							isOp(User& user) const;
		bool							channelIsFull();
		bool							isValidChannelName(const std::string& name);
		//Channel methods
		void							addUserChannel(User& user);
		void							removeUserChannel(User& user);
		void							broadcastMessage(const std::string& message, User &sender);
		void							sendTopicMessage(User& user);
		void							addOpChannel(User& user);
		//Copilot suggestions
		// void addUser(User* user, bool isOp = false);
		// void setOp(User* user, bool isOp);
};

std::ostream& operator<<(std::ostream& out, const Channel& channel);

#endif
