cmake_minimum_required(VERSION 2.6)

add_library(IElib STATIC    Trace
                            timing
                            report
                            benchmark
                            threadpool
                            Client
                            Visitor
                            DataElement
                            BinaryHeap
                            Graph
                            MarketGraph
                            SecPair
                            Decomposer
                            ImpliedQuoteSubscriber
                            CMemPool
                            cat1_visitor
                            cat2_visitor
                            ImpliedEngine
                            ImpliedServer)
target_include_directories(IElib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})