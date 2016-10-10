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
                            ImpliedEngine)
target_include_directories(IElib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})