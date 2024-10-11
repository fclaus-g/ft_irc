#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		std::vector<int> _users;
		//std::map<User &, bool> _users;
		bool _inviteMode;
		bool _topicMode;
		bool _keyMode;
		int _usersLimit;
		std::string _password;
	public:
		Channel();
		Channel(std::string name);
		~Channel();
		
		const std::string& getName() const;
		const std::string& getTopic() const;
		const std::vector<int>& getUsers() const;
		const bool getInviteMode() const;
		const bool getTopicMode() const;
		const bool getKeyMode() const;
		const int getUsersLimit() const;
		const std::string& getPassword() const;

		void setName(const std::string& name);
		void setTopic(const std::string& topic);
		void setInviteMode(const bool inviteMode);
		void setTopicMode(const bool topicMode);
		void setKeyMode(const bool keyMode);
		void setUsersLimit(const int usersLimit);
		void setPassword(const std::string& password);

		void addUser(int userFd);
		void removeUser(int userFd);
};

std::ostream& operator<<(std::ostream& os, const Channel& channel);
#endif
