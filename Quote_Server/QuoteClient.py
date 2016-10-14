import sys
import os
import thread
import time
from socket import *

def now() : return time.ctime(time.time())

BLOCK_SIZE= 1024
DEFAULT_MODE = 'client'
DEFAULT_HOST = 'localhost'
DEFAULT_PORT = 8008

helptext = """
Usage...
client=> python QuoteClient.py  [-mode client] [-port nnn] [-host hhh|localhost]
"""
def parsecommandline():
    r = {}
    args = sys.argv[1:]
    while len(args) >= 2:
        r[args[0]] = args[1]
        args = args[2:]
    return r


def receive(host, port):
    filename = 'local_version.dat'
    file = open(filename, 'wb')
    sock = socket(AF_INET, SOCK_STREAM)
    sock.connect((host, port))
    while True:
        data = sock.recv(BLOCK_SIZE)
        if not data: break
        file.write(data)
    sock.close()
    file.close()
    print 'Client got', filename, 'at', now()

def main(args):
    host = args.get('-host', DEFAULT_HOST)
    port = args.get('-port', DEFAULT_PORT)
    if (args.get('-mode', DEFAULT_MODE) == 'client'):
        receive(host, port)

if __name__ == "__main__":
    args = parsecommandline()
    main(args)
