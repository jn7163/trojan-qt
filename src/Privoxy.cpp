#include "Privoxy.h"
#ifdef Q_OS_WIN

Privoxy::Privoxy(QObject *parent) : QProcess(parent)
{
    _config="listen-address __PRIVOXY_BIND_IP__:__PRIVOXY_BIND_PORT__ \n "
            "toggle 0 \n "
            "logfile privoxy.log \n "
            "forward-socks5 / 127.0.0.1:__SOCKS_PORT__ .";
    AppManager::checkFile(APP_DATA_DIR+"privoxy.exe",":/privoxy/privoxy.exe");
    setProgram(APP_DATA_DIR+"privoxy.exe");
    setNativeArguments(APP_DATA_DIR+"privoxy.conf");
}

unsigned short Privoxy::getAvailablePort()
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
///This function will change the port of trojan
void Privoxy::configure(bool localOnly,unsigned short &trojanPort)
{
    unsigned short port =getAvailablePort();
    _config.replace("__PRIVOXY_BIND_IP__",localOnly?"127.0.0.1":"0.0.0.0");
    _config.replace("__PRIVOXY_BIND_PORT__",QString(port));
    _config.replace("__SOCKS_PORT__",QString(trojanPort));
    trojanPort =port;
    try{
        QFile qfile(APP_DATA_DIR+"/privioxy.conf");
        qfile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
        QTextStream ts(&qfile);
        ts<<_config<<endl;
        qfile.close();
    }
    catch(const std::exception &e){
        emit exception(QString(e.what()));
    }


}

#endif // Q_OS_WIN
