#include <list>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

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
        std::vector<struct replace_pair> _replace_pairs;
        for (auto &pair : replace_pairs)
        {
            struct replace_pair p;
            p.src = pair["src"].GetString();
            p.dest = pair["dest"].GetString();
            if (pair.HasMember("src_name"))
            {
                p.src_name = pair["src_name"].GetString();
            }
            if (pair.HasMember("dest_name"))
            {
                p.dest_name = pair["dest_name"].GetString();
            }
        }
        ProxyServer *p = new ProxyServer(src_port, dest_port, delay_time, _replace_pairs);

        if (i.HasMember("src_name"))
        {
            p->set_src_name(i["src_name"].GetString());
        }
        if (i.HasMember("dest_name"))
        {
            p->set_dest_name(i["dest_name"].GetString());
        }
        proxy_servers.push_back(p);
    }

    while (true)
    {
        std::this_thread::sleep_for(2s);
    }

    return 0;
}