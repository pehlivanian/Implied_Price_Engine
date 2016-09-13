from __future__ import division
import sys
import getopt
import numpy as np
import pandas as pd
import itertools

def randGraph(m,n,maxAbsWeight,NEG_ALLOWED):
    vertices = range(m)
    edges = []
    weights = []
    ii = itertools.combinations(range(0,m),2)
    p = n/(m*(m-1)/2)
    for el in ii:
        if (np.random.uniform() < p):
            edges.append(el)
            weights.append(np.random.randint(1,maxAbsWeight))
            edges.append((el[1],el[0]))
            if (NEG_ALLOWED):
                # Negative weights
                weights.append(1-weights[-1])
            else:
                # Positive weights
                weights.append(max(1, weights[-1] - 1))
    return {'vertices' : vertices, 'edges' : edges, 'weights' : weights}


def serialize(g):
    print "Vertices Edges"
    print str(len(g['vertices'])) + " " + str(len(g['edges'])) + " directed"
    for i in range(0,len(g['edges'])):
        edge = g['edges'][i]
        wt = g['weights'][i]
        print str(edge[0]) + "," + str(edge[1]) + "," + str(wt)
                   
def create(m, n, wt, NEG_ALLOWED):
    g = randGraph(m, n, wt, NEG_ALLOWED)
    serialize(g)

if __name__ == "__main__":
    m =  int(sys.argv[1])
    n =  int(sys.argv[2])
    wt = int(sys.argv[3])
    NEG_ALLOWED = False
    if (len(sys.argv) > 4):
        NEG_ALLOWED = (sys.argv[4] == "True")
    create(m, n, wt, NEG_ALLOWED)
    
