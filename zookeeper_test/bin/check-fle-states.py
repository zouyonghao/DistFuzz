#!/usr/bin/python

import telnetlib
import traceback
import re


def parse_stat_str(s):
    print 'Parsing %s: ' % s
    d = {'mode': 'NOTPARSED', 'zxid': 'NOTPARSED'}
    d['mode'] = re.compile('.*Mode:\s(.*)').search(s).group(1)
    d['zxid'] = re.compile('.*Zxid:\s(.*)').search(s).group(1)
    print 'Parsed %s: ' % d
    return d


def stat(host, port=2181):
    print 'call stat'
    try:
        tn = telnetlib.Telnet(host, port)
        tn.write('stat')
        s = tn.read_all()
        d = parse_stat_str(s)
        tn.close()
        return d
    except Exception as e:
        print 'Could not parse: %s' % e
        # raise e
    return {'mode': 'NOTPARSED', 'zxid': 'NOTPARSED'}


def get_stats():
    l = []
    l.append(stat('127.0.1.1', 2181))
    l.append(stat('127.0.1.1', 2182))
    l.append(stat('127.0.1.1', 2183))
    l.append(stat('127.0.1.1', 2184))
    l.append(stat('127.0.1.1', 2185))
    return l


def main():
    try:
        l = get_stats()
        leaders = filter(lambda d: d['mode'] == 'leader', l)
        observers = filter(lambda d: d['mode'] == 'observer', l)
        # assert len(leaders) == 1, 'Bad leader election: %s' % l
        assert len(observers) == 0, 'Bad observers: %s' % l
        print('OK (%d leaders, %d observers): %s' % (len(leaders), len(observers), l))
    except Exception as e:
        print(traceback.format_exc())
        raise e

if __name__ == '__main__':
    main()
