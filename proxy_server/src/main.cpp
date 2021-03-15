#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <thread>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

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
    for (auto &server : document["proxy_servers"].GetArray())
    {
        int src_port = server["src_port"].GetInt();
        int dest_port = server["dest_port"].GetInt();
        int delay_time = server["delay_time"].GetInt();
        auto replace_pairs = server["replace_pairs"].GetArray();
        std::vector<struct replace_pair> _replace_pairs;
        for (auto &pair : replace_pairs)
        {
            struct replace_pair p;
            p.src.target_string = pair["src"].GetString();
            p.src.target_name = pair["src_name"].GetString();

            p.dest.target_string = pair["dest"].GetString();
            p.dest.target_name = pair["dest_name"].GetString();

            _replace_pairs.push_back(p);
        }
        ProxyServer *p =
            new ProxyServer(src_port, dest_port, delay_time, _replace_pairs);
        proxy_servers.push_back(p);
    }

    while (true)
    {
        std::this_thread::sleep_for(2s);
    }

    return 0;
}