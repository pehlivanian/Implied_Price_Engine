from __future__ import division
import string
import math
import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from numpy import nan as NA
import matplotlib.pyplot as plt
from datetime import datetime
from dateutil.parser import parse
import scipy.odr.odrpack as odr
from matplotlib.backends.backend_pdf import PdfPages
import matplotlib.font_manager

class Singleton(object):
    def __new__(self):
        if not hasattr(self, "instance)"):
            self.instance = super(Singleton, self).__new__(self)
        return self.instance

class QuoteData(Singleton):
    def __init__(self):
        udf = pd.read_csv("./data/user_quote.dat", header=0, index_col=0)
        idf = pd.read_csv("./data/implied_quote.dat", header=0, index_col=0)
        self.data = { 'udf' : udf, 'idf' : idf}


