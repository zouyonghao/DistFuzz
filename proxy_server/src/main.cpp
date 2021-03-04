#include <list>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>

#include <document.h>
#include <prettywriter.h>
#include <istreamwrapper.h>
#include <ostreamwrapper.h>

#include <proxy_server.h>

using namespace std::chrono_literals;
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "usage: ./proxy_server config.json" << std::endl;
        exit(0);
    }
    std::ifstream ifs(argv[1]);
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document document;
    if (document.ParseStream(isw).HasParseError())
    {
        std::cout << "Parse " << argv[1] << " failed!\n";
        std::cout << document.GetParseError() << "\n";
        return -EBADF;
    }
    ifs.close();

    std::list<ProxyServer *> proxy_servers;
    for (auto &i : document["proxy_servers"].GetArray())
    {
        int src_port = i["src_port"].GetInt();
        int dest_port = i["dest_port"].GetInt();
        int delay_time = i["delay_time"].GetInt();
        auto replace_pairs = i["replace_pairs"].GetArray();
        std::map<std::string, std::string> replace_map;
        for (auto &pair : replace_pairs)
        {
            replace_map[pair["src"].GetString()] = pair["dest"].GetString();
        }
        ProxyServer *p = new ProxyServer(src_port, dest_port, delay_time, replace_map);
        proxy_servers.push_back(p);
    }

    while (true)
    {
        std::this_thread::sleep_for(2s);
    }

    return 0;
}