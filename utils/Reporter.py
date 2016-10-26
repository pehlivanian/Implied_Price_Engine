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
import scipy.odr.odrpack as odr
from Reporter_impl import *

class DataElementVisitor(object):
    def __init__(self):
        self.impl = Reporter_impl()
    def visit(self, DataElement):
        pass

class UservImpliedReporter(DataElementVisitor):
    def __init__(self, outpath='./reports/', nameStr='', closeonExit=True, pdf=None):
        super(UservImpliedReporter, self).__init__()
        self.outpath = outpath
        self.nameStr = nameStr
        self.pdf = pdf

    def visit(self, DataElement):
        UservImpliedReport(self, DataElement)
        # if (self.closeonExit):
        #     self.pdf.close()

class AlgoComplexityReporter(DataElementVisitor):
    def __init__(self, outpath='./reports/', nameStr='', closeonExit=True, pdf=None):
        super(AlgoComplexityReporter, self).__init__()
        self.outpath = outpath
        self.nameStr = nameStr
        self.pdf = pdf

    def visit(self, DataElement):
        AlgoComplexityReport(self, DataElement)
        # if (self.closeonExit):
        #     self.pdf.close()

    @staticmethod
    def func_fit(B,x):
        return B[0]*(x*x*np.log(x))

def UservImpliedReport(self, DataElement):
    if (self.pdf is None):
        pdf = PdfPages(self.outpath + 'quote_comp.pdf')
    else:
        pdf = self.pdf

    udf = DataElement.data.data["udf"]
    idf = DataElement.data.data["idf"]

    sz = udf.shape[0]
    for i in xrange(500,1000,4):
        print i
        fld = udf.index[i]
        t = udf.index[i].split('_')[-1]
        pfx = '_'.join(udf.index[i+0].split('_')[0:-1])
        df = pd.DataFrame({'user_ask_p' : udf.ix[udf.index[i+0]], \
                           'user_bid_p' : udf.ix[udf.index[i+2]], \
                           'impl_ask_p'  : idf.ix[idf.index[i+0]], \
                           'impl_bid_p'  : idf.ix[idf.index[i+2]]})
        df = df.apply(lambda x : x/100, axis=1)
        # Too busy
        # df['user_mid_p'] = .5*(df['user_bid_p'] + df['user_ask_p'])
        # df['impl_mid_p'] = .5*(df['impl_bid_p'] + df['impl_ask_p'])
        df['ex_col'] = df['impl_ask_p'] - .04
        df['impl_bid_p'] = df[['ex_col', 'impl_bid_p']].min(axis=1)
        del df['ex_col']
        df.plot(style=['r--*','r--*','r*-','r*-'],grid=True, title="Iteration: " + t)
        pdf.savefig()
        plt.close()
    pdf.close()


def AlgoComplexityReport(self, DataElement):
    if (self.pdf is None):
        pdf = PdfPages(self.outpath + 'algo_complexity.pdf')
    else:
        pdf = self.pdf

    # Raw timing data
    df = pd.DataFrame( { "SubProblem" :  range(1,13), \
                         "Act_time_in_micros" : [0, .9615, 2.0000, 3.9231, 5.7692, 9.3462, 12.2692, 16.5000, 21.4231, \
                                    26.7692, 35.1154, 44.3846 ] } )
    x = range(1,13)
    y = df["Act_time_in_micros"].values
    func = odr.Model(self.func_fit)
    odrdata = odr.Data(x,y)
    odrmodel = odr.ODR(odrdata, func, beta0=[1], maxit=1000, ifixx=[0])
    o = odrmodel.run()
    fitted_func = lambda xin : self.func_fit(o.beta, xin)
    df['Theor_time_in_micros'] = [fitted_func(x0) for x0 in x]
    del df['SubProblem']
    titleStr = 'Theoretical Algo Complexity : {}*n^2 log(n)'.format(str(round(o.beta[0],4)))
    df.plot(style=['r','r*-'], title=titleStr, xticks=range(1,13))
    plt.xlabel('Num_Legs')
    plt.ylabel('Microseconds')
    pdf.savefig()
    plt.close()
    pdf.close()
