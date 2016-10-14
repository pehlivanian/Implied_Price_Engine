#ifndef __IMPLIEDSERVER_IMPL_HPP__
#define __IMPLIEDSERVER_IMPL_HPP__

template<int N>
class ImpliedServer;

template<int N>
struct impl<ImpliedServer<N>>
{
    impl(bool process_feed) :
            process_feed_(process_feed),
            IE_(std::make_unique<ImpliedEngine<N>>()),
            C_(std::make_unique<Client>(8008, (char*)"0.0.0.0")),
            pool_(std::make_unique<threadpool>()) {}

    bool process_feed_;
    std::unique_ptr<ImpliedEngine<N>> IE_;
    std::unique_ptr<Client> C_;
    std::unique_ptr<threadpool> pool_;

};

template<int N>
void
ImpliedServer<N>::init_()
{
    if (p_->process_feed_)
        (p_->C_)->fetch();
}

template<int N>
void
ImpliedServer<N>::process()
{
    using namespace std;
    using namespace rapidjson;

    Document document;

#define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))
    string tok;
    stringstream ss((p_->C_)->get_buf());

    // XXX
    // At least make this static
    regex leg_pat(R"((Leg)([\d]{1})([\d]?))");
    regex spread_pat(R"((Spread)([\d]{1})([\d]?))");
    smatch match;
    string Inst;
    SecPair sp;
    int leg, spd0, spd1, pc;
    size_t sz;

    while(getline(ss, tok, '\n'))
    {
        if (document.Parse(tok.c_str()).HasParseError())
        {
            fprintf(stderr, "Parse error!\n");
            return;
        };
        assert(document.HasMember("Inst"));
        if (document.HasMember("bid"))
        {
            Inst = document["Inst"].GetString();
            if (regex_match(Inst, match, leg_pat))
            {
                istringstream ss(match[2]);
                ss >> leg;
                sp = SecPair(leg, -1, 1);
                pc = document["bid"].GetInt();
                sz = static_cast<size_t>(document["size"].GetInt());
            }
            if (regex_match(Inst, match, spread_pat))
            {
                istringstream ss0(match[2]), ss1(match[3]);
                ss0 >> spd0; ss1 >> spd1;
                sp = SecPair(spd0, spd1, 1);
                pc = document["bid"].GetInt();
                sz = static_cast<size_t>(document["size"].GetInt());
            }
            // Multi-threaded version call
            auto fn = [this,&sp,&pc,&sz]() mutable { (p_->IE_)->publish_bid(sp, QUOTE(pc,sz)); return 0; };
            // Submit to pool
            int r = (p_->pool_)->submit(fn).get();

            // Single-threaded version call
            // (p_->IE_)->publish_bid(sp, QUOTE(pc, sz));
        }
        else if (document.HasMember("ask"))
        {
            Inst = document["Inst"].GetString();
            if (regex_match(Inst, match, leg_pat))
            {
                istringstream ss(match[2]);
                ss >> leg;
                sp = SecPair(leg, -1, 1);
                pc = document["ask"].GetInt();
                sz = static_cast<size_t>(document["size"].GetInt());
            }
            if (regex_match(Inst, match, spread_pat))
            {
                istringstream ss0(match[2]), ss1(match[3]);
                ss0 >> spd0; ss1 >> spd1;
                sp = SecPair(spd0, spd1, 1);
                pc = document["ask"].GetInt();
                sz = static_cast<size_t>(document["size"].GetInt());
            }
            // Multi-threaded version call
            auto fn = [this,&sp,&pc,&sz]() mutable { (p_->IE_)->publish_ask(sp, QUOTE(pc,sz)); return 0; };
            // Submit to pool
            int r = (p_->pool_)->submit(fn).get();

            // Single-threaded version call
            // (p_->IE_)->publish_ask(sp, QUOTE(pc, sz));
        }
    }
#undef ask_p
#undef bid_p
#undef QUOTE

}


#endif