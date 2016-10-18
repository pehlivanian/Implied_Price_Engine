from __future__ import division
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from matplotlib.ticker import NullFormatter
from matplotlib import mlab, cm
from scipy import stats
import statsmodels.formula.api as sm
from sklearn.linear_model import LinearRegression

class Reporter_impl(object):
    def __init__(self):
        pass

    # @staticmethod defs below__
