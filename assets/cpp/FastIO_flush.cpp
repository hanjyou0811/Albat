class FastIO {
    char tmp_s[20];

public:
    FastIO() {}

    ~FastIO() { flush(); }

    inline void flush() {
        fflush(stdout);
    }

    inline void wc(char x) {
        putchar(x);
        if (x == '\n') flush();
    }

    inline void read(double &x) {
        bool neg = false;
        x = 0;
        double factor = 1.0;
        
        char ch = getchar();
        while ((ch < '0' || ch > '9') && ch != '-' && ch != '.')
            ch = getchar();
        
        if (ch == '-') {
            neg = true;
            ch = getchar();
        }

        while ('0' <= ch && ch <= '9') {
            x = x * 10 + (ch - '0');
            ch = getchar();
        }

        if (ch == '.') {
            ch = getchar();
            while ('0' <= ch && ch <= '9') {
                factor *= 0.1;
                x += (ch - '0') * factor;
                ch = getchar();
            }
        }

        if (neg)
            x = -x;
    }

    template <typename T> inline void read(T &x) {
        bool neg = false;
        x = 0;
        char ch = getchar();
        while ((ch < '0' || ch > '9') && ch != '-')
            ch = getchar();
        if (ch == '-') {
            neg = true;
            ch = getchar();
        }
        while ('0' <= ch && ch <= '9') {
            x = 10 * x + (ch - '0');
            ch = getchar();
        }
        if (neg)
            x = -x;
    }

    inline void read(string &s) {
        s.clear();
        char ch = getchar();
        while (ch <= ' ')
            ch = getchar();
        while (ch > ' ') {
            s += ch;
            ch = getchar();
        }
    }

    inline void read(char &c) {
        c = getchar();
    }

    inline void wt_ln(double x, string sep = "\n") {
        if (x < 0) {
            wc('-');
            x = -x;
        }

        long long int_part = static_cast<long long>(x);
        wt(int_part);
        
        double frac_part = x - int_part;
        if (frac_part > 0) {
            wc('.');
            for (int i = 0; i < 10; ++i) {
                frac_part *= 10;
                int digit = static_cast<int>(frac_part);
                wc('0' + digit);
                frac_part -= digit;
                if (frac_part < 1e-10) break;
            }
        }
        wt(sep);
    }

    template <typename T> inline void wt_ln(T x) {
        if (x == 0) {
            wc('0');
            wc('\n');
            return;
        }
        if (x < 0) {
            wc('-');
            x = -x;
        }
        char *t = tmp_s;
        while (x) {
            T y = x / 10;
            *(t++) = (x - y * 10) + '0';
            x = y;
        }
        while (t != tmp_s)
            wc(*(--t));
        wc('\n');
    }

    inline void wt_ln(const char *s, string sep = "\n") {
        while (*s) {
            wc(*(s++));
        }
        wt(sep);
    }

    inline void wt_ln(const char &c, string sep = "\n") {
        wc(c);
        wc('\n');
    }

    inline void wt_ln(const string &s, string sep = "\n") {
        for (char c : s) wc(c);
        wc('\n');
    }

    inline void wt(double x, int precision = 10) {
        if (x < 0) {
            wc('-');
            x = -x;
        }

        long long head = static_cast<long long>(x);
        wt(head);
        
        double tail = x - head;
        if (tail > 0) {
            wc('.');
            for (int i = 0; i < precision; ++i) {
                tail *= 10;
                int digit = static_cast<int>(tail);
                wc('0' + digit);
                tail -= digit;
                if (tail < 1e-10) break;
            }
        }
    }

    template <typename T> inline void wt(T x) {
        if (x == 0) {
            wc('0');
            return;
        }
        if (x < 0) {
            wc('-');
            x = -x;
        }
        char *t = tmp_s;
        while (x) {
            T y = x / 10;
            *(t++) = (x - y * 10) + '0';
            x = y;
        }
        while (t != tmp_s)
            wc(*(--t));
    }

    inline void wt(const char *s) {
        while (*s) wc(*(s++));
    }

    inline void wt(const std::string &s) {
        for (char c : s) {
            wc(c);
        }
    }

    inline void wt(const char &c) { wc(c); }

    // pair<T1,T2>
    template <typename f, typename s> inline void read(pair<f, s> &P) {
        read(P.first);
        read(P.second);
    }
    template <typename f, typename s>
    inline void wt_ln(pair<f, s> &P, string sep = " ") {
        wt_ln(P.first, sep);
        wt_ln(P.second, sep);
        wc('\n');
    }

    template <typename f, typename s>
    inline void wt(pair<f, s> &P, string sep = " ") {
        wt(P.first, sep);
        wt(P.second, sep);
    }

    // vector<T>
    template <typename T> inline void read(vector<T> &vec) {
        for (auto &v : vec) {
            read(v);
        }
    }
    template <typename T> inline void wt_ln(vector<T> &vec, string sep = " ") {
        int vec_size = vec.size();
        for(int i = 0; i < vec_size; i++) {
            if(i == vec_size - 1) wt_ln(vec[i], "");
            else wt_ln(vec[i], sep);
        }
        wc('\n');
    }

    template <typename T> inline void wt(vector<T> &vec, string sep = " ") {
        int vec_size = vec.size();
        for(int i = 0; i < vec_size; i++) {
            if(i == vec_size - 1) wt_ln(vec[i], "");
            else wt_ln(vec[i], sep);
        }
    }
};

FastIO io;
#define wtln io.wt_ln
#define wt io.wt
#define rd io.read
#define flush io.flush