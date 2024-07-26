#include "server.hpp"
#include "client.hpp"
#include <iostream>
#include <vector>
#include <string>
#include "utils.hpp"

void Server::setupChannel(const std::string &channelName, int fd, const std::string &password) {
    if (channels.find(channelName) == channels.end()) {
        Channel newChannel(channelName, 0);  
        channels[channelName] = newChannel;
    } else {
        // Ensure not to reset the topic if the channel already exists
        Channel &channel = channels[channelName];
        if (channel.getTopic() == "No topic") {
            channel.setTopic("No topic");
        }
    }

    // Additional setup code
    Channel &channel = channels[channelName];
    channel.setClients(fd, getClientNickname(fd));
    if (password.empty()) {
        channel.setChannelType(0);  // Public
        channel.setHasPassword(0);
    } else {
        channel.setChannelType(1);  // Password protected
        channel.setHasPassword(1);
        channel.setChPassword(password);
    }
}

void Server::JoinMessage(int fd, const std::string &channelName) {

    std::map<std::string, Channel>::iterator it = channels.find(channelName);

    if (channels.find(channelName) != channels.end()) {
        std::string globmsg = "";
        std::string topic = channels[channelName].getTopic();  // Get topic
        std::string message = ":" + getClientNickname(fd) + "!" + getClientNickname(fd) + "@" + getClientHostName(fd) + " JOIN " + channelName + " * :realname\r\n";
        globmsg += message;

        Channel& channel = it->second;
        std::string message2 = ":" + getClientHostName(fd) + " 353 " + getClientNickname(fd) + " = " + channelName + " :";
        bool first = true;
        
        std::map<int, client>::const_iterator clientIt;
        const std::map<int, client>& clients = channel.getClientsFromChannel();
        for (clientIt = clients.begin(); clientIt != clients.end(); ++clientIt) {
            if (!first) {
                message2 += " ";
            }
            first = false;
            message2 += "@" + clientIt->second.getNickname() + "!" + clientIt->second.getNickname() + "@" + getClientHostName(fd);
        }
        message2 += "\n";
        globmsg += message2;    
        
        std::string message3 = ":" + getClientHostName(fd) + " 366 " + getClientNickname(fd) + " " + channelName + " :End of /NAMES list.\r\n";
        globmsg += message3;

        // send to other clients in the channel like this message :simo!simo@88ABE6.25BF1D.D03F86.88C9BD.IP JOIN #chan * :realname
        for (std::map<int, client>::const_iterator it = channel.beginClientIter(); it != channel.endClientIter(); ++it) {
            if (it->first != fd) {
                std::string message = ":" + getClientNickname(fd) + "!" + getClientNickname(fd) + "@" + getClientHostName(fd) + " JOIN " + channelName + " * :realname\r\n";
                sendMessage(it->first, message);
            }
        }
        sendMessage(fd, globmsg);
    } else {
        std::string globmsg = ":" + getNameId(fd) + " 403 " + channelName + " :No such channel\n";
        sendMessage(fd, globmsg);
    }
}

void Server::joinCmd(std::vector<std::string> &param, int fd) {
    if (param.empty() || param[0][0] != '#' || param[0].size() <= 1) {
        std::string message = ":" + getNameId(fd) + " 461 JOIN :Not enough parameters\n";
        sendMessage(fd, message);
        return;
    }

    std::string channelName = param[0];

    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    bool channelCreated = (it == channels.end());

    if (channelCreated) {
        // Create the channel if it doesn't exist
        setupChannel(channelName, fd, (param.size() == 2) ? param[1] : "");
        JoinMessage(fd, channelName);
    }
    else {
        std::cout << "Channel \"" << channelName << "\" already exists \""<< std::endl;
        // std::cout << "client name is : " << getClientNickname(fd) << std::endl;
        // std::cout << "channel name is : " << channelName << std::endl;

        // Get reference to the existing channel
        Channel &channel = channels[channelName];

        // Add the client to the channel
        channel.setClients(fd, getClientNickname(fd));

        // Inform the client about joining the channel
        JoinMessage(fd, channelName);
    }
}


// void Server::joinCmd(std::vector<std::string> &param, int fd) {
//     // Debugging output for received parameters
//     std::cout << "Received /join command with parameters: ";
//     for (size_t i = 0; i < param.size(); ++i) {
//         std::cout << param[i] << " ";
//     }
//     std::cout << std::endl;

//     // Check if the param vector is empty or invalid
//     if (param.empty() || param[0][0] != '#' || (param[0].size() == 1)) {
//         std::string message = ":" + getNameId(fd) + " 461 JOIN :Not enough parameters\n";
//         sendMessage(fd, message);
//         std::cout << "Error: Not enough parameters or invalid channel name." << std::endl;
//         return;
//     }

//     // Split the parameters
//     std::vector<std::string> chn = split(param[0], ',');
//     std::vector<std::string> key;
//     if (param.size() == 2) {
//         key = split(param[1], ',');
//     }

//     std::cout << "Channel names to join: ";
//     for (size_t i = 0; i < chn.size(); ++i) {
//         std::cout << chn[i] << " ";
//     }
//     std::cout << std::endl;

//     std::cout << "Keys provided: ";
//     for (size_t i = 0; i < key.size(); ++i) {
//         std::cout << key[i] << " ";
//     }
//     std::cout << std::endl;

//     // Loop through each channel name
//     for (size_t i = 0; i < chn.size(); ++i) {
//         std::map<std::string, Channel>::iterator it = channels.find(chn[i]);
//         bool channelCreated = (it == channels.end());

//         // If channel doesn't exist, create it
//         if (channelCreated) {
//             setupChannel(chn[i], fd, (param.size() == 2 && key.size() > i) ? key[i] : "");
//             JoinMessage(fd, chn[i]);
//             continue;
//         }

//         Channel &channel = it->second;

//         // Check if client is already in the channel
//         if (channel.isClientInChannel(fd)) {
//             std::string message = ":" + getNameId(fd) + " 443 * :You are already in this channel\n";
//             sendMessage(fd, message);
//             continue;
//         }

//         // Handle invite-only or password-protected channels
//         if (channel.getChannelType() == 1) {
//             if (channel.getInvitedList().empty() && param.size() != 2) {
//                 std::string message = ":" + getNameId(fd) + " 473 * Cannot join channel (+i)\n";
//                 sendMessage(fd, message);
//                 continue;
//             }

//             if (channel.isClientInvited(fd)) {
//                 channel.setClients(fd, getClientNickname(fd));
//                 JoinMessage(fd, chn[i]);
//                 continue;
//             }

//             if (channel.getHasPassword() == 1) {
//                 if ((key.size() > i && channel.getChPassword() == key[i]) || key.empty()) {
//                     channel.setClients(fd, getClientNickname(fd));
//                     JoinMessage(fd, chn[i]);
//                 } else {
//                     std::string message = ":" + getNameId(fd) + " 475 * Cannot join channel (+k)\n";
//                     sendMessage(fd, message);
//                 }
//                 continue;
//             }
//         }

//         // Handle password for existing channels
//         if (channel.getHasPassword() == 1) {
//             if (key.size() > i && channel.getChPassword() == key[i]) {
//                 channel.setClients(fd, getClientNickname(fd));
//                 JoinMessage(fd, chn[i]);
//             } else {
//                 std::string message = ":" + getNameId(fd) + " 475 * Cannot join channel (+k)\n";
//                 sendMessage(fd, message);
//             }
//             continue;
//         }

//         // Add the client to the channel and send a join message
//         channel.setClients(fd, getClientNickname(fd));
//         JoinMessage(fd, chn[i]);
//     }
// }