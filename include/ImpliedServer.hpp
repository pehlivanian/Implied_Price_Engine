#ifndef __IMPLIEDSERVER_HPP__
#define __IMPLIEDSERVER_HPP__

#include <memory>
#include <regex>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include "ImpliedEngine.hpp"
#include "Client.hpp"
#include "SecPair.hpp"
#include "threadpool.hpp"
#include "impl.hpp"

#define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))

using Price_Size_Pair = std::pair<int, size_t>;

template<int N>
class ImpliedServer;

template<int N>
class ImpliedServer
{
public:

    ImpliedServer(bool process_feed=true) :
            p_(std::make_unique<impl<ImpliedServer<N>>>(process_feed)) { init_(); }
    void process();

    // Here come the delegators
    void publish_bid(const SecPair& sp, const QuotePublishEvent& e)  { (p_->IE_)->publish_bid(sp, e); }
    void publish_bid(const SecPair& sp, int pc, size_t sz)           { (p_->IE_)->publish_bid(sp, QUOTE(pc,sz)); }
    void publish_bid(int leg, const QuotePublishEvent& e)            { (p_->IE_)->publish_bid(SecPair(leg, -1, 1), e); }
    void publish_bid(int leg, int pc, size_t sz)                     { (p_->IE_)->publish_bid(SecPair(leg, -1, 1), QUOTE(pc,sz)); }
    void publish_bid(int leg0, int leg1, const QuotePublishEvent& e) { (p_->IE_)->publish_bid(SecPair(leg0, leg1, 1), e); }
    void publish_bid(int leg0, int leg1, int pc, size_t sz)          { (p_->IE_)->publish_bid(SecPair(leg0, leg1, 1), QUOTE(pc,sz)); }
    void publish_ask(const SecPair& sp, const QuotePublishEvent& e)  { (p_->IE_)->publish_ask(sp, e); }
    void publish_ask(const SecPair& sp, int pc, size_t sz)           { (p_->IE_)->publish_ask(sp, QUOTE(pc,sz)); }
    void publish_ask(int leg, const QuotePublishEvent& e)            { (p_->IE_)->publish_ask(SecPair(leg, -1, 1), e); }
    void publish_ask(int leg, int pc, size_t sz)                     { (p_->IE_)->publish_ask(SecPair(leg, -1, 1), QUOTE(pc,sz)); }
    void publish_ask(int leg0, int leg1, const QuotePublishEvent& e) { (p_->IE_)->publish_ask(SecPair(leg0, leg1, 1), e); }
    void publish_ask(int leg0, int leg1, int pc, size_t sz)          { (p_->IE_)->publish_ask(SecPair(leg0, leg1, 1), QUOTE(pc,sz)); }
    char* get_buf() const                          { return (p_->C_)->get_buf(); }
    void write_user_curve() const                  { (p_->IE_)->write_user_curve(); }
    void write_implied_curve() const               { (p_->IE_)->write_implied_curve(); }
    void write_merged_curve() const                { (p_->IE_)->write_merged_curve(); }
    int get_num_legs() const                       { return (p_->IE_)->get_num_legs(); }
    Price_Size_Pair get_bid(int leg) const         { return (p_->IE_)->get_bid(leg);}
    Price_Size_Pair get_user_bid(int leg) const    { return (p_->IE_)->get_user_bid(leg); }
    Price_Size_Pair get_implied_bid(int leg) const { return (p_->IE_)->get_implied_bid(leg); }
    Price_Size_Pair get_ask(int leg) const         { return (p_->IE_)->get_ask(leg); }
    Price_Size_Pair get_user_ask(int leg) const    { return (p_->IE_)->get_user_ask(leg); }
    Price_Size_Pair get_impied_ask(int leg) const  { return (p_->IE_)->get_implied_ask(leg); }

private:
    void init_();
    std::unique_ptr<impl<ImpliedServer>> p_;
};

#undef QUOTE
#include "ImpliedServer_impl.hpp"

#endif