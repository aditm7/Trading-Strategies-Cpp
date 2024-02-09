#include<bits/stdc++.h>
using namespace std;
template <class P, class Q = vector<P>, class R = less<P>>
ostream &operator<<(ostream &out, priority_queue<P, Q, R> const &M)
{
    static priority_queue<P, Q, R> U;
    U = M;
    out << "{ ";
    while (!U.empty())
        out << U.top() << " ", U.pop();
    return (out << "}");
}

template <class P>
ostream &operator<<(ostream &out, queue<P> const &M)
{
    static queue<P> U;
    U = M;
    out << "{ ";
    while (!U.empty())
        out << U.front() << " ", U.pop();
    return (out << "}");
}

template <typename T, typename S>
ostream &operator<<(ostream &os, const map<T, S> &v)
{
    os << "[ ";
    for (auto it : v)
        os << it.first << " : "
           << it.second << " , ";
    os << " ]";
    return os;
}

//---------don't go beyond this line-------//
vector<string> vec_splitter(string S)
{
    S += ',';
    vector<string> res;
    while (!S.empty())
    {
        res.push_back(S.substr(0, S.find(',')));
        S = S.substr(S.find(',') + 1);
    }
    return res;
}
void debug_out(vector<string> __attribute__((unused)) args, __attribute__((unused)) int idx, __attribute__((unused)) int LINE_NUM) { cerr << "\n"; }
template <typename Head, typename... Tail>
void debug_out(vector<string> args, int idx, int LINE_NUM, Head H, Tail... T)
{
    if (idx > 0)
        cerr << ", ";
    else
        cerr << "* Line(" << LINE_NUM << ") : \n";
    stringstream ss;
    ss << H;
    cerr << args[idx] << " = " << ss.str();
    debug_out(args, idx + 1, LINE_NUM, T...);
}
#define debug(...) debug_out(vec_splitter(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
template <typename T>
void console(T t)
{
    cerr << t << endl;
}
template <typename T, typename... Args>
void console(T t, Args... args)
{
    cerr << t << " ";
    error(args...);
}
template<class L,class R> ostream& operator << (ostream& out, pair<L, R> const &val){ if(out.rdbuf()==cout.rdbuf()) return (out<<val.first<<" "<<val.second); else return (out << "{" << val.first << "," << val.second << "}"); }
template<class Ch, class Tr, class Container> basic_ostream <Ch, Tr> & operator << (basic_ostream <Ch, Tr> & os, Container const&X) { if (os.rdbuf() != cout.rdbuf()) os << "[ "; if constexpr(is_same_v<Container, vector<bool>>) for (int i = 0; i < X.size(); i++) os << (X[i] ? "true" : "false") << ' '; else for (auto& elem : X) os << elem << ' '; if (os.rdbuf() != cout.rdbuf()) os << "]"; return os; }