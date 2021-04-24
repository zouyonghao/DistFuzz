#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <thread>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <proxy_server.h>

using namespace std::chrono_literals;

const std::string STRING_REPLACE_MODE = "string";
const std::string HEX_REPLACE_MODE = "hex";

struct replace_pair parse_replace_pair(rapidjson::Value &pair,
                                       std::string &replace_mode)
{
    struct replace_pair p;
    p.src.target_name = pair["src_name"].GetString();
    p.dest.target_name = pair["dest_name"].GetString();

    if (replace_mode == STRING_REPLACE_MODE)
    {
        p.src.size = pair["src"].GetStringLength();
        p.src.target_string = new char[p.src.size];
        memcpy(p.src.target_string, pair["src"].GetString(), p.src.size);

        p.dest.size = pair["dest"].GetStringLength();
        p.dest.target_string = new char[p.dest.size];
        memcpy(p.dest.target_string, pair["dest"].GetString(), p.dest.size);
    }
    else
    {
        std::vector<char> src_replaces;
        int tmp;
        uint8_t u8_tmp;
        std::istringstream ss(pair["src"].GetString());
        while (!ss.eof())
        {
            ss >> std::hex >> tmp;
            u8_tmp = tmp;
            printf("%x ", u8_tmp);
            src_replaces.push_back(u8_tmp);
        }
        p.src.target_string = new char[src_replaces.size()];
        p.src.size = src_replaces.size();
        memcpy(p.src.target_string, src_replaces.data(), src_replaces.size());
        std::cout << "size = " << p.src.size << "\n";

        std::vector<char> dest_replaces;
        ss = std::istringstream(pair["dest"].GetString());
        while (!ss.eof())
        {
            ss >> std::hex >> tmp;
            u8_tmp = tmp;
            printf("%x ", u8_tmp);
            dest_replaces.push_back(u8_tmp);
        }
        p.dest.target_string = new char[dest_replaces.size()];
        p.dest.size = dest_replaces.size();
        memcpy(p.dest.target_string, dest_replaces.data(),
               dest_replaces.size());
        std::cout << "size = " << p.dest.size << "\n";
    }
    return p;
}

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

    std::string common_replace_mode = STRING_REPLACE_MODE;
    std::vector<struct replace_pair> _common_replace_pairs;
    if (document.HasMember("common_replace_mode"))
    {
        common_replace_mode = document["common_replace_mode"].GetString();
    }
    if (document.HasMember("common_replace_pairs"))
    {
        auto replace_pairs = document["common_replace_pairs"].GetArray();
        for (auto &pair : replace_pairs)
        {
            auto p = parse_replace_pair(pair, common_replace_mode);
            _common_replace_pairs.push_back(p);
        }
    }

    std::cout << "common replace pairs number: " << _common_replace_pairs.size() << "\n";

    std::list<ProxyServer *> proxy_servers;
    for (auto &server : document["proxy_servers"].GetArray())
    {
        std::string src_ip = server["src_ip"].GetString();
        int src_port = server["src_port"].GetInt();
        std::string dest_ip = server["dest_ip"].GetString();
        int dest_port = server["dest_port"].GetInt();
        int delay_time = server["delay_time"].GetInt();
        std::vector<struct replace_pair> _replace_pairs;
        std::string replace_mode = STRING_REPLACE_MODE;
        if (server.HasMember("replace_mode"))
        {
            replace_mode = server["replace_mode"].GetString();
        }
        if (server.HasMember("replace_pairs"))
        {
            auto replace_pairs = server["replace_pairs"].GetArray();
            for (auto &pair_json : replace_pairs)
            {
                auto pair = parse_replace_pair(pair_json, replace_mode);
                _replace_pairs.push_back(pair);
            }
        }
        for (auto &pair : _common_replace_pairs)
        {
            _replace_pairs.push_back(pair);
        }
        ProxyServer *p = new ProxyServer(src_ip, src_port, dest_ip, dest_port,
                                         delay_time, _replace_pairs);
        proxy_servers.push_back(p);
    }

    std::cout << "proxy_servers size: " << proxy_servers.size() << "\n";

    while (true)
    {
        std::this_thread::sleep_for(2s);
    }

    return 0;
}