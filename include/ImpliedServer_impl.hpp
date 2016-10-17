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
ImpliedServer<N>::profiled_process_tasks_()
{
    const int R = 10;
    const int C = tasks_.size();

    std::string WORK_DIR = "~/ClionProjects/Implied_Price_Engine_All/data/";

    std::ofstream fsu("user_quote.dat");
    std::ofstream fsi("implied_quote.dat");
    auto q = (p_->IE_)->get_user_quote();
    fsu << "Type";
    fsi << "Type";
    for(size_t i=0; i<q[0].size(); ++i)
    {
        fsu << ",Leg_" + std::to_string(i);
        fsi << ",Leg_" + std::to_string(i);
    }
    fsu << "\n";
    fsi << "\n";

    long* Micro_times[R];

    for(size_t i=0; i<R; ++i)
    {
        Micro_times[i] = (long *) malloc(C * sizeof(long));
    }

    struct timeval beforeV, afterV;

    for(int r=0; r<R; ++r)
            for(int c=0; c<C; ++c)
            {
                gettimeofday(&beforeV, 0);
                // int res = (p_->pool_)->submit(tasks_[c]).get();
                int res = tasks_[c]();
                gettimeofday(&afterV, 0);
                // std::cout << beforeV.tv_usec << " : " << afterV.tv_usec << "\n";
                Micro_times[r][c] = diffTimer(&beforeV, &afterV);
                if ((r == 0) && (c > 99) && ((c%10) == 0))
                {

                    (p_->IE_)->write_user_quote(c, fsu);
                    (p_->IE_)->write_implied_quote(c, fsi);

                }

            }

    fsu.close();
    fsi.close();

    printf("Table (micros) for Implied Quote Update Step\n");
    printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
    buildTable(Micro_times, R, C);

}

template<int N>
void
ImpliedServer<N>::process_tasks_()
{
    for(auto& task : tasks_)
        int r = (p_->pool_)->submit(task).get();
}

template<int N>
void
ImpliedServer<N>::preload_tasks_()
{
    using namespace std;
    using namespace rapidjson;

    Document document;

#define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))
    string tok;
    stringstream ss((p_->C_)->get_buf());

    // XXX
    // At least make this static
    regex leg_pat(R"((Leg_)([\d]+))");
    regex spread_pat(R"((Spread_)([\d]+)[_]([\d]+))");
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
            std::function<int()> fn = [this,sp,pc,sz]() mutable { (p_->IE_)->publish_bid(sp, QUOTE(pc,sz)); return 0; };
            tasks_.push_back(fn);
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
            std::function<int()> fn = [this,sp,pc,sz]() mutable { (p_->IE_)->publish_ask(sp, QUOTE(pc,sz)); return 0; };
            tasks_.push_back(fn);
        }
    }
#undef ask_p
#undef bid_p
#undef QUOTE

}

#endif