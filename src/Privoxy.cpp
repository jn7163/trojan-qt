#include "Privoxy.h"
#ifdef Q_OS_WIN

Privoxy::Privoxy(QObject *parent) : QObject(parent)
{

}
unsigned short Privoxy::getPort()
{
    using namespace boost::asio;
    io_context io_context;
    bool portinuse=false;
    unsigned short port=1984;
    do{
        try{
            portinuse=false;
            ip::tcp::socket tcp_socket(io_context,ip::tcp::endpoint(ip::tcp::v4(),port));
            ip::tcp::socket tcp_socketv6(io_context,ip::tcp::endpoint(ip::tcp::v6(),port));
            ip::udp::socket udp_socket(io_context,ip::udp::endpoint(ip::udp::v4(),port));
            ip::udp::socket udp_socketv6(io_context,ip::udp::endpoint(ip::udp::v6(),port));
        }
        catch(...){
            portinuse=true;
            port++;
        }
    }while(portinuse);
    io_context.stop();
    return port;
}

#endif // Q_OS_WIN
