struct mod_int{
    static unsigned md;
    static unsigned W;
    static unsigned R;
    static unsigned Rinv;
    static unsigned mntnum;
    static unsigned RR;
    unsigned val;
    mod_int():val(0){
    }
    mod_int(int a):val(mulR(a)){
    }
    mod_int(unsigned a):val(mulR(a)){
    }
    mod_int(long long a):val(mulR(a)){
    }
    mod_int(unsigned long long a):val(mulR(a)){
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
    inline mod_int &operator++(){
        (*this) += 1;
        return *this;
    }
    inline mod_int &operator--(){
        (*this) -= 1;
        return *this;
    }
    inline mod_int operator++(int){
        mod_int res=*this;
        (*this) += 1;
        return res;
    }
    inline mod_int operator--(int){
        mod_int res=*this;
        (*this) -= 1;
        return res;
    }
    mod_int &operator+=(mod_int a){
        if(val < a.val){
            val = val - a.val + md;
        }
        else{
            val -= a.val;
        }
        return *this;
    }
    mod_int &operator-=(mod_int a){
        if(val < a.val){
            val = val + md - a.val;
        }
        else{
            val -= a.val;
        }
        return *this;
    }
    mod_int &operator*=(mod_int a){
        val = reduce((unsigned long long)val * a.val);
        return *this;
    }
    mod_int &operator/=(mod_int a){
        return *this *= a.inverse();
    }
    mod_int operator+(mod_int a){
        return mod_int(*this) += a;
    }
    mod_int operator-(mod_int a){
        return mod_int(*this) -= a;
    }
    mod_int operator*(mod_int a){
        return mod_int(*this) *= a;
    }
    mod_int operator/(mod_int a){
        return mod_int(*this) /= a;
    }
    mod_int operator+(int a){
        return mod_int(*this) += mod_int(a);
    }
    mod_int operator-(int a){
        return mod_int(*this) -= mod_int(a);
    }
    mod_int operator*(int a){
        return mod_int(*this) *= mod_int(a);
    }
    mod_int operator/(int a){
        return mod_int(*this) /= mod_int(a);
    }
    mod_int operator+(long long a){
        return mod_int(*this) += mod_int(a);
    }
    mod_int operator-(long long a){
        return mod_int(*this) -= mod_int(a);
    }
    mod_int operator*(long long a){
        return mod_int(*this) *= mod_int(a);
    }
    mod_int operator/(long long a){
        return mod_int(*this) /= mod_int(a);
    }
    mod_int operator-(void){
        mod_int res;
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
    mod_int inverse(){
        int a = val;
        int b = md;
        int u = 1;
        int v = 0;
        int t;
        mod_int res;
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
    mod_int pw(unsigned long long b){
        mod_int a = *this;
        mod_int res;
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
    mod_int pw(int b){
        return pw((unsigned long long)b);
    }
    mod_int pw(long long b){
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
unsigned mod_int::md;
unsigned mod_int::W;
unsigned mod_int::R;
unsigned mod_int::Rinv;
unsigned mod_int::mntnum;
unsigned mod_int::RR;
mod_int operator+(int a, mod_int b){
    return mod_int(a)+=b;
}
mod_int operator-(int a, mod_int b){
    return mod_int(a)-=b;
}
mod_int operator*(int a, mod_int b){
    return mod_int(a)*=b;
}
mod_int operator/(int a, mod_int b){
    return mod_int(a)/=b;
}
mod_int operator+(long long a, mod_int b){
    return mod_int(a)+=b;
}
mod_int operator-(long long a, mod_int b){
    return mod_int(a)-=b;
}
mod_int operator*(long long a, mod_int b){
    return mod_int(a)*=b;
}
mod_int operator/(long long a, mod_int b){
    return mod_int(a)/=b;
}
