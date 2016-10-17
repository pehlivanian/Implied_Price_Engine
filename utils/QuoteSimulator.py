from __future__ import division
import sys
import getopt
import numpy as np
import pandas as pd
import itertools
from numpy import random

def now(): return time.ctime(time.time())

DEFAULT_NUM_LEGS = 12
DEFAULT_NUM_QUOTES = 1000

helptext = """
Usage...
python QuoteSimulator.py  [-num_legs|12] [-port nnn] [-num_quotes|1000]
"""
def parsecommandline():
    r = {}
    args = sys.argv[1:]
    while len(args) >= 2:
        r[args[0]] = args[1]
        args = args[2:]
    return r

def create_initial_quotes(num_legs):
    return None

def create_next_leg_quote(leg_num):
    return None

def create_next_spread_quote(leg0_num, leg1_num):
    return None

def create_quote_stream(num_legs, num_quotes):
    quotes = create_initial_quotes(num_legs)
    for ind in xrange(num_quotes):
        quotes.append(create_next_leg_quote(leg_num))
        quotes.append(create_next_leg_quote(leg_num))
        quotes.append(create_next_spread_quote(leg0_num, leg1_num))
    yield quotes

def json_serialize(quotes):
    return None

def main(args):
    host = args.get('-host', DEFAULT_HOST)
    port = args.get('-port', DEFAULT_PORT)
    json_serialize(create_quote_stream(num_legs, num_quotes))

if __name__ == "__main__":
    args = parsecommandline()
    main(args)


