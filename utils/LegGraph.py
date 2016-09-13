from __future__ import division
import sys
import getopt
import numpy as np
import pandas as pd
import itertools

def legGraph(n, maxAbsWeight, NEG_ALLOWED):
    vertices = range(n+1)
    edges = []
    weights = []
    ii = itertools.combinations(range(0,n+1),2)
    for el in ii:
        if (el[0] == 0):
            if (not (el[1] == 1)):
                edges.append(el)
                weights.append(np.random.randint(1,maxAbsWeight))
                edges.append((el[1],el[0]))
                if (NEG_ALLOWED):
                    weights.append(1-weights[-1])
                else:
                    weights.append(max(1, weights[-1] - 1))
        elif (el[0] == 1):
            if (not (el[1] == 0)):
                edges.append(el)
                weights.append(np.random.randint(1,maxAbsWeight))
                edges.append((el[1],el[0]))
                if (NEG_ALLOWED):
                    weights.append(1-weights[-1])
                else:
                    weights.append(max(1, weights[-1] - 1))
        else:
            edges.append(el)
            weights.append(np.random.randint(1,maxAbsWeight))
            edges.append((el[1],el[0]))
            if (NEG_ALLOWED):
                weights.append(1-weights[-1])
            else:
                weights.append(max(1, weights[-1] - 1))
    return {'vertices' : vertices, 'edges' : edges, 'weights' : weights}
      

# Format as specifed by graph_utils::load
def serialize(g):
    print "Vertices Edges"
    print str(len(g['vertices'])) + " " + str(len(g['edges'])) + " directed"
    for i in range(0,len(g['edges'])):
        edge = g['edges'][i]
        wt = g['weights'][i]
        print str(edge[0]) + "," + str(edge[1]) + "," + str(wt)
                   
def create(n, wt, NEG_ALLOWED):
    g = legGraph(n, wt, NEG_ALLOWED)
    serialize(g)

if __name__ == "__main__":
    n =  int(sys.argv[1])
    wt = int(sys.argv[2])
    NEG_ALLOWED = False
    if (len(sys.argv) > 3):
        NEG_ALLOWED = (sys.argv[4] == "True")
    create(n, wt, NEG_ALLOWED)
