add_library(IElib Trace.cpp timing.cpp report.cpp benchmark.cpp threadpool.cpp
        Client.cpp Visitor.cpp DataElement.cpp BinaryHeap.cpp Graph.cpp
        MarketGraph.cpp SecPair.cpp Decomposer.cpp ImpliedQuoteSubscriber.cpp
        CMemPool.cpp cat1_visitor.cpp cat2_visitor.cpp ImpliedEngine.cpp)
target_include_directories(IElib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})