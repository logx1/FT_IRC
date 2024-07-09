#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "client.hpp"

class Channel {
    private:
        std::string           name;
        std::string           topic;
        std::map<int, client> clients;
        std::vector<int>      operators;
        std::string           chPassword;
        std::map<int, client> inviteList;
        int                   channelType; 
        int                   hasPassword;

    public:
        Channel();
        Channel(std::string channelName, int channelOperators , int channelType);
        Channel(const Channel &copy);
        Channel &operator=(const Channel &copy);
        ~Channel();

        // setters : 
        void setName(std::string channelName);
        void setTopic(std::string channelTopic);
        void setClients(std::map<int, client> channelClients);
        void setOperators(std::vector<int> channelOperators);
        void setChPassword(std::string channelPassword);
        void setChannelType(int channelType);
        void setHasPassword(int hasPassword);
        void Channel::setInvitedList(int id, client &c);

        // getters :
        std::string getName();
        std::string getTopic();
        std::vector<int> &getOperators();
        std::string getChPassword();
        std::map<int, client>::const_iterator beginClientIter() const;
        std::map<int, client>::const_iterator endClientIter() const;
        int getChannelType();
        int getClientID(const std::string &nickname) const;
        int isClientInChannel(int fd);
        int getHasPassword() const;
        size_t getClientNb() const;
        bool isClientInvited(int clientId) const;

        const std::map<int, client> &Channel::getClientsFromChannel() const;
        void removeClient(int id);
        void removeInitedClient(int id);
        void removeOperator(int id);


}




#endif