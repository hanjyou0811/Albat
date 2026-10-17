struct mint{
    static unsigned md;
    static unsigned W;
    static unsigned R;
    static unsigned Rinv;
    static unsigned mntnum;
    static unsigned RR;
    unsigned val;
    mint():val(0){
    }
    mint(int a):val(mulR(a)){
    }
    mint(unsigned a):val(mulR(a)){
    }
    mint(long long a):val(mulR(a)){
    }
    mint(unsigned long long a):val(mulR(a)){
    }
    unsigned mulR(unsigned a){
        return (unsigned long long)a*R%md;
    }
    unsigned mulR(int a){
        if(a < 0){
            a = a%((int)md)+(int)md;
        }
        return mulR((unsigned)a);
    }
    unsigned mulR(unsigned long long a){
        return mulR((unsigned)(a%md));
    }
    unsigned mulR(long long a){
        a %= (int)md;
        if(a < 0){
            a += md;
        }
        return mulR((unsigned)a);
    }
    unsigned reduce(unsigned T){
        unsigned m=T*mntnum;
        unsigned t=(T+(unsigned long long)m*md)>>W;
        if(t>=md){
            t-=md;
        }
        return t;
    }
    unsigned reduce(unsigned long long T){
        unsigned m=T*mntnum;
        unsigned t=(T+(unsigned long long)m*md)>>W;
        if(t>=md){
            t-=md;
        }
        return t;
    }
    unsigned get(){
        return reduce(val);
    }
    int get_inv(long long a, int md){
        long long t = a;
        long long s = md;
        long long u = 1;
        long long v = 0;
        long long e;
        while (s){
            e = t / s;
            t = t - e * s;
            swap(t, s);
            u = u - e * v;
            swap(u, v);
        }
        if(u < 0){
            u += md;
        }
        return u;
    }
    void set(unsigned MOD){
        int i;
        unsigned t;
        md = MOD;
        W = 32;
        R = (1ULL << W) % md;
        RR = (unsigned long long) R * R % md;
        if(MOD == 1000000007){
            Rinv = 518424770;
            mntnum = 2226617417U;
        }
        else if(MOD == 998244353){
            Rinv = 232013824;
            mntnum = 998244351;
        }
        else if(MOD == 1000000009){
            Rinv = 171601999;
            mntnum = 737024967;
        }
        else if(MOD == 1004535809){
            Rinv = 234947584;
            mntnum = 1004535807;
        }
        else if(MOD == 1051721729){
            Rinv = 257538304;
            mntnum = 1051721727;
        }
        else{
            Rinv = get_inv(R, MOD);
            mntnum = 0;
            t = 0;
            for(i=0;i<32;i++){
                if(t % 2 == 0){
                    t += md;
                    mntnum |= (1U << i);
                }
                t >>= 1;
            }
        }
    }
    inline mint &operator++(){
        (*this) += 1;
        return *this;
    }
    inline mint &operator--(){
        (*this) -= 1;
        return *this;
    }
    inline mint operator++(int){
        mint res=*this;
        (*this) += 1;
        return res;
    }
    inline mint operator--(int){
        mint res=*this;
        (*this) -= 1;
        return res;
    }
    mint &operator+=(mint a){
        if(val < a.val){
            val = val - a.val + md;
        }
        else{
            val -= a.val;
        }
        return *this;
    }
    mint &operator-=(mint a){
        if(val < a.val){
            val = val + md - a.val;
        }
        else{
            val -= a.val;
        }
        return *this;
    }
    mint &operator*=(mint a){
        val = reduce((unsigned long long)val * a.val);
        return *this;
    }
    mint &operator/=(mint a){
        return *this *= a.inverse();
    }
    mint operator+(mint a){
        return mint(*this) += a;
    }
    mint operator-(mint a){
        return mint(*this) -= a;
    }
    mint operator*(mint a){
        return mint(*this) *= a;
    }
    mint operator/(mint a){
        return mint(*this) /= a;
    }
    mint operator+(int a){
        return mint(*this) += mint(a);
    }
    mint operator-(int a){
        return mint(*this) -= mint(a);
    }
    mint operator*(int a){
        return mint(*this) *= mint(a);
    }
    mint operator/(int a){
        return mint(*this) /= mint(a);
    }
    mint operator+(long long a){
        return mint(*this) += mint(a);
    }
    mint operator-(long long a){
        return mint(*this) -= mint(a);
    }
    mint operator*(long long a){
        return mint(*this) *= mint(a);
    }
    mint operator/(long long a){
        return mint(*this) /= mint(a);
    }
    mint operator-(void){
        mint res;
        if(val){
            res.val = md - val;
        }
        else{
            res.val = 0;
        }
        return res;
    }
    operator bool(void){
        return val != 0;
    }
    operator int(void){
        return get();
    }
    operator long long(void){
        return get();
    }
    mint inverse(){
        int a = val;
        int b = md;
        int u = 1;
        int v = 0;
        int t;
        mint res;
        while(b){
            t = a / b;
            a -= t * b;
            swap(a, b);
            u -= t * v;
            swap(u, v);
        }
        if(u < 0){
            u += md;
        }
        res.val = (unsigned)u*RR % md;
        return res;
    }
    mint pw(unsigned long long b){
        mint a = *this;
        mint res;
        res.val = R;
        while(b){
            if(b & 1){
                res *= a;
            }
            a *= a;
            b >>= 1;
        }
        return res;
    }
    mint pw(int b){
        return pw((unsigned long long)b);
    }
    mint pw(long long b){
        return pw((unsigned long long)b);
    }
    bool operator==(int a){
        return mulR(a) == val;
    }
    bool operator==(unsigned a){
        return mulR(a) == val;
    }
    bool operator==(long long a){
        return mulR(a) == val;
    }
    bool operator!=(int a){
        return mulR(a) != val;
    }
    bool operator!=(unsigned a){
        return mulR(a) != val;
    }
    bool operator!=(long long a){
        return mulR(a) != val;
    }
}
;
unsigned mint::md;
unsigned mint::W;
unsigned mint::R;
unsigned mint::Rinv;
unsigned mint::mntnum;
unsigned mint::RR;
mint operator+(int a, mint b){
  return mint(a)+=b;
}
mint operator-(int a, mint b){
  return mint(a)-=b;
}
mint operator*(int a, mint b){
  return mint(a)*=b;
}
mint operator/(int a, mint b){
  return mint(a)/=b;
}
mint operator+(long long a, mint b){
  return mint(a)+=b;
}
mint operator-(long long a, mint b){
  return mint(a)-=b;
}
mint operator*(long long a, mint b){
  return mint(a)*=b;
}
mint operator/(long long a, mint b){
  return mint(a)/=b;
}
