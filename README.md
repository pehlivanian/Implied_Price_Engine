`# Implied_Price_Engine

Efficient computation of implied prices in energy complex. This is an
optimized Implied Price Engine for the CME/ICE Energy complex, our
test case. By Implied Price Engine we mean an algorithm for generating
inside full implied {bid,ask} {price,size} information for outright
contracts {0,...,N-1} based on user-input (non-implied) quotes for
outrights, calendar spreads, calendar strips, etc in contracts
{0,...,M-1}. 

The code is optimized and times in microsends for the exchange quote update --> generated implied quote cycle for the n-leg problems are shown in the table below. The 12-leg (full calendar year in most crude, produces) takes on average less than 10 micros for the update cycle.

```
 legs    average         min        max        stdev        #trials
 -----     -------            ---            ---             -----         -------
 0        0.000000        0        0        0.000000        999998
 1        0.000000        0        0        0.000000        999998
 2        1.450224        0        16335        2.157503        999998
 3        1.744384        1        12412        3.437309        999998
 4        2.052856        1        8510        3.748844        999998
 5        2.687152        2        11847        3.882017        999998
 6        3.173562        2        12475        3.407387        999998
 7        3.325170        2        3444        6.540734        999998
 8        4.148126        3        4086        7.450844        999998
 9        4.994720        4        3519        8.256040        999998
 10       5.668034        4        4799        8.603363        999998
 11       6.691828        5        11881        17.564463        999998
 12       7.913124        6        10121        10.412915        999998
 13       9.453345        7        13721        21.430757        999998
 14       9.586401        8        7875        24.379706        999998
 15       10.084670      8        8246        16.196867        999998
 16       11.625233      10        4032        12.451166        999998
 17       12.661665      11        12258        19.556137        999998
 18       13.993410      12        9616        18.107115        999998
 19       16.782749      13        9092        29.949022        999998
 20       19.950690      14        59956        62.886111        999998
 21       24.439750      16        66186        43.988407        999998
 22       21.510550      17        11464        42.936446        999998
 23       22.624264      18        15784        41.984122        999998
 24       24.506462      19        12628        47.052661        999998
```
