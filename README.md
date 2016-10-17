# Implied_Price_Engine_All

Complete Implied Price Server for CME, ICE energy markets.
Configurable, prices read in-process by socket server (json-based),
with user-only, implied-only, and merged-prices available via callback
subscription.

Application runs worker threads asynchronously to update quote user,
implied quote information as quote objects arrive. 

For more information contact me at pehlivaniancharles@gmail.com.
