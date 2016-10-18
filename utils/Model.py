from __future__ import division
import os
import sys
import string
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from numpy import nan as NA
import scipy.odr.odrpack as odr

import Data as QuoteData

class DataElement(object):
    def __init__(self):
        pass
    def accept(self, visitor):
        visitor.visit(self)

class QuoteModel(DataElement):
    def __init__(self, working_dir='../data/'):
        self.working_dir = working_dir
        self.data = QuoteData.QuoteData()
