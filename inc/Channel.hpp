#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::vector<int> _users;
    public:
        Channel();
        Channel(std::string name);
        ~Channel();
        
        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::vector<int>& getUsers() const;

        void setName(const std::string& name);
        void setTopic(const std::string& topic);
        void addUser(int userFd);
        void removeUser(int userFd);
};

#endif
