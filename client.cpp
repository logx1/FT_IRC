#include "client.hpp"

std::vector<pollfd> client::fds;
std::vector<client> client::clients;

client::client()
{
    this->is_authenticated = false;
    this->is_registered = false;
    this->index = 0;
}

client::client(pollfd client_pfd, std::string password)
{
    this->client_pfd = client_pfd;
    this->is_authenticated = false;
    this->is_registered = false;
    this->password = password;
    this->index = 0;
}


client::~client()
{
//     std::cout << "client with nickname " << this->nickname << " is destroyed" << std::endl;

}

void client::set_nickname(std::string nickname)
{
    this->nickname = nickname;
}

void client::set_username(std::string username)
{
    this->username = username;
}

void client::set_realname(std::string realname)
{
    this->realname = realname;
}

std::string client::getNickname() const
{
    return this->nickname;
}

std::string client::getUsername()const
{
    return this->username;
}
std::vector<std::string> buffer_to_line(std::string buffer, std::string siparator)
{
    std::vector<std::string> lines;
    std::string line;
    std::istringstream stm(buffer);
    while (std::getline(stm, line))
    {
        size_t found = line.find_first_of(siparator);

        if (found != std::string::npos)
        {
            line = line.substr(0, found);
            lines.push_back(line);
        }
        
    }
    return lines;
}





bool client::set_authenticated()
{
    std::string line;
    if (this->is_authenticated == true)
    {
        return (true);
    }else
    { 
        std::vector<std::string> lines = buffer_to_line(this->massage, "\r\n");

        
   

        // std::cout << "--------------- not authonticatef --------------------" << std::endl;
        // std::cout << "the size of lines : " << lines.size() << std::endl;
        for (unsigned int i = 0; i < lines.size(); i++)
        {
        //    std::cout << lines[i] << std::endl;
              if (lines[i].find("NICK") != std::string::npos)
              {
                std::string nickname = lines[i].substr(5, lines[i].size());
                this->set_nickname(nickname);
              }
                if (lines[i].find("USER") != std::string::npos)
                {
                    std::string username = lines[i].substr(5, lines[i].size());
                    this->set_username(username);


                    // try to grt the hodt name { this->username = user_abdel-ou user_abdel-ou localhost :realname} try to get the host name

                    size_t found1 = this->username.find(" ");
                    if (found1 != std::string::npos) {
                        size_t found2 = this->username.find(" ", found1 + 1);
                        if (found2 != std::string::npos) {
                     
                            std::string tmp_realname = this->username.substr(found2 + 1, this->username.size());

                            size_t found3 = tmp_realname.find(" ");
                            if (found3 != std::string::npos) {
                                std::string realname = tmp_realname.substr(0, found3);
                                // std::cout << "realname : " << realname << std::endl;
                                this->hostname = realname;

                                // std::cout << get_host_name() << std::endl;
                            }
                        }
                    }
                    
                }

                if (lines[i].find("PASS") != std::string::npos)
                {
                    this->sabmit_password = lines[i].substr(5, lines[i].size());
                    
                }

                
        }
                if ((this->nickname.size() > 0) && (this->username.size() > 0) && (this->password == this->sabmit_password))
                {
                    this->is_authenticated = true;
                    return (true);
                }








    }

     return (false);      
}

void client::set_registered(bool is_registered)
{
    this->is_registered = is_registered;
}

pollfd client::get_client_pfd() const 
{
    return this->client_pfd;
}

void client::set_client_pfd(pollfd client_pfd)
{
    this->client_pfd = client_pfd;
}





void client::set_massage_for_auth(std::string massage)
{
    this->massage = this->massage + massage;
}

// get message 
std::string client::get_message()
{
    return this->massage;
}

void client::print_massage()
{
    this->index++;
    std::cout << "--------------" << "the cleint " << this->client_pfd.fd << " : " << this->index << " : print massage method start ----------------" << std::endl;
    std::cout << this->massage;
    std::cout << "--------------print massage method end----------------" << std::endl;
}

void client::print_client()
{
    std::cout << "nickname : " << this->nickname << std::endl;
    std::cout << "username : " << this->username << std::endl;
    std::cout << "realname : " << this->realname << std::endl;
    std::cout << "password sabmited : " << this->sabmit_password << std::endl;
    std::cout << "password : " << this->password << std::endl;
}


void client::clear_massage()
{
    this->massage = "";
}

void client::set_massage(std::string massage)
{
    this->massage = massage;
}

std::vector<pollfd>& client::get_fds() { return fds; }
std::vector<client>& client::get_clients() { return clients; }

std::string client::get_host_name()
{
    return this->hostname;
}