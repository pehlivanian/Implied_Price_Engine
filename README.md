# Implied_Price_Engine

Complete Implied Price Server for CME, ICE energy markets. 

The algorithm is new represents significant efficiency enhancements over
the standard algorithms. Prices are read in-process by socket server
(json-based) object, and user-only, implied-only, and merged-prices
are published via callback subscription.

Application runs worker threads asynchronously to update quote user,
implied quote information as quote objects arrive. 

For more information contact me at pehlivaniancharles@gmail.com.
