class FastIO {
    static const int read_siz = (1 << 28), write_siz = (1 << 28);
    char read_buf[read_siz], write_buf[write_siz], *rb, *wb;
    char tmp_s[20];

    #define wc(x) *(wb++) = x
    public:
    FastIO() {
        fread(read_buf, 1, read_siz, stdin);
        rb = read_buf;
        wb = write_buf;
    }
    ~FastIO() { fwrite(write_buf, 1, wb - write_buf, stdout); }

    inline void read(double &x) {
        bool neg = false;
        x = 0;
        double factor = 1.0;
        
        while ((*rb < '0' || *rb > '9') && *rb != '-' && *rb != '.')
            ++rb;
        
        if (*rb == '-') {
            neg = true;
            ++rb;
        }

        while ('0' <= *rb && *rb <= '9') {
            x = x * 10 + (*rb - '0');
            ++rb;
        }

        if (*rb == '.') {
            ++rb;
            while ('0' <= *rb && *rb <= '9') {
                factor *= 0.1;
                x += (*rb - '0') * factor;
                ++rb;
            }
        }

        if (neg)
            x = -x;
    }

    template <typename T> inline void read(T &x) {
        bool neg = false;
        x = 0;
        while ((*rb < '0' || *rb > '9') && *rb != '-') ++rb;
        if (*rb == '-') {
            neg = true;
            ++rb;
        }
        while ('0' <= *rb && *rb <= '9') {
            x = 10 * x + (*rb - '0');
            ++rb;
        }
        if (neg) x = -x;
    }

    inline void read(string &s) {
        s.clear();
        while (*rb <= ' ') ++rb;
        while (*rb > ' ') {
            s += *(rb++);
        }
    }

    inline void read(char &c) {
        while (*rb <= ' ') ++rb;
        c = *(rb++);
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

    template <typename T> inline void wt_ln(T x, string sep = "\n") {

        if (x == 0) {
            wc('0');
            wt(sep);
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
        while (t != tmp_s) wc(*(--t));
        wt(sep);
    }

  
    inline void wt_ln(const char *s, string sep = "\n") {
            while (*s) {
                wc(*(s++));
            }
        wt(sep);
    }

    inline void wt_ln(const char &c, string sep = "\n") {
        wc(c);
        wt(sep);
    }
    inline void wt_ln(const string &s, string sep = "\n") {
        for(char c : s) wc(c);
        wt(sep);
    }

    inline void wt(double x, int precision = 10, string sep = "\n") {
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
        while (t != tmp_s) wc(*(--t));
    }

    inline void wt(const char *s)
    {
        while(*s) wc(*(s++));
    }

    inline void wt(const string &s) {
        for (char c : s) {
            wc(c);
        }
    }

    inline void wt(const char &c) { wc(c); }

    template <typename f, typename s> inline void read(pair<f, s> &PAIR) {
        read(PAIR.first);
        read(PAIR.second);
    }
    template <typename f, typename s>
    inline void wt_ln(pair<f, s> &PAIR, string sep = " ") {
        wt(PAIR.first, sep);
        wt(PAIR.second, sep);
        wc('\n');
    }
    template <typename f, typename s>
    inline void wt(pair<f, s> &PAIR, string sep = " ")
    {
        wt(PAIR.first, sep);
        wt(PAIR.second, sep);
    }

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

    #undef wc
    };
FastIO io;
#define wtln io.wt_ln
#define wt io.wt
#define rd io.read