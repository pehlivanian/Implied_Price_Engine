import sys
import os
import thread
import time
from socket import *

def now(): return time.ctime(time.time())

BLOCK_SIZE= 1024
DEFAULT_MODE = 'server'
DEFAULT_HOST = 'localhost'
DEFAULT_PORT = 50001
DEFAULT_FILENAME=''

helptext = """
Usage...
server=> python QuoteServer.py  [-mode server] -file fff [-port nnn] [-host hhh|localhost]
"""
def parsecommandline():
    r = {}
    args = sys.argv[1:]
    while len(args) >= 2:
        r[args[0]] = args[1]
        args = args[2:]
    return r

def servethread(clientsock, filename):
    file = open(filename)
    while True:
        bytes = file.read(BLOCK_SIZE)
        if not bytes: break
        sent = clientsock.send(bytes)
        assert sent == len(bytes)
    clientsock.close()

def serve(host, port, filename):
    serversock = socket(AF_INET, SOCK_STREAM)
    serversock.bind((host, port)) 
    serversock.listen(5)
    while True:
        clientsock, clientaddr = serversock.accept()
        print 'Server connected by', clientaddr, 'at', now()
        thread.start_new_thread(servethread, (clientsock,filename,))

def main(args):
    host = args.get('-host', DEFAULT_HOST)
    port = args.get('-port', DEFAULT_PORT)
    filename = args.get('-file', DEFAULT_FILENAME)
    if (args.get('-mode', DEFAULT_MODE) == 'server'):
        serve(host, port, filename)

if __name__ == "__main__":
    args = parsecommandline()
    main(args)
