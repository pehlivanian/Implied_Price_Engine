import sys
sys.path.append('./utils/')

import Model as QuoteModel
import Reporter as QuoteReporter

if __name__ == "__main__":

    qm = QuoteModel.QuoteModel()
    qr = QuoteReporter.AlgoComplexityReporter()
    # qr = QuoteReporter.UservImpliedReporter()
    qm.accept(qr)
