#include "irc_server.hpp"
#include "server.hpp"
#include "client.hpp"


int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "Usage: ./irc_server <port> <password>" << std::endl;
        return (1);
    }

Server irc_server("0.0.0.0", argv[1]);
irc_server.setsockopt();
irc_server.bind_socket();
irc_server.listen_socket();



std::vector<pollfd> fds;

std::vector<client> clients;

fds = irc_server.get_fds();

// std::cout << "server is running :: fds : " << fds.front().fd << std::endl;

while (1)
{
    int r = poll(&fds[0], fds.size(), -1);
    if (r == -1)
    {
        std::cout << "error in poll function" << std::endl;  
    }
    unsigned int i = 0;
    while ( i < fds.size())
    {
        
        if (fds[i].revents & POLLIN)
        {
            if (fds[i].fd == irc_server.get_sockfd())
            {
                // New client
                sockaddr_in clientaddr;
                socklen_t client_len = sizeof(clientaddr);
                int clientfd = accept(irc_server.get_sockfd(), (struct sockaddr*)&clientaddr, &client_len);

                if (clientfd == -1)
                {
                    std::cout << "error in accept function" << std::endl;
                }
                else
                {
                    // std::cout << "new client connected " << inet_ntoa(clientaddr.sin_addr) << std::endl;
                    pollfd client_pfd;
                    
                    client_pfd.fd = clientfd;
                    client_pfd.events = POLLIN;
                    fds.push_back(client_pfd);
                    
                    clients.push_back(client(client_pfd, argv[2]));
                }
            }
            else
            {
                
                char buffer[1024];
                int r = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if (r > 0)
                {
                    buffer[r] = '\0';
                    
                    // std::cout << buffer;
                    unsigned int l = 0;
                    while (l < clients.size())
                    {
                        if ((clients[l].get_client_pfd().fd == fds[i].fd))
                        {
                            if (clients[l].set_authenticated() == true)
                            {
                                std::cout << "client : " << l << " authenticated" << std::endl;
                            }
                            
                            if (buffer[0] != ':')
                            {
                            //    std::cout << "client found" << std::endl;
                                clients[l].set_massage(buffer);
                                buffer[0] = '\0';
                            }else
                            {
                                std::cout << "---------------------------------" << std::endl;
                                std::cout << "client : " << l << std::endl;
                                clients[l].print_massage();
                                std::cout << "---------------------------------" << std::endl;
                            }
                        }
                        

                        l++;
                    }
                    // unsigned int j = 0;
                    // while ( j < fds.size())
                    // {
                    //     if(fds[j].fd != irc_server.get_sockfd())
                    //     {
                    //         send(fds[j].fd, buffer, r, 0);
                    //     }
                    //     j++;
                    // }
                }
            }
        }
        i++;
    }
}

return (0);
}
