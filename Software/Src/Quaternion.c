typedef struct {
    double a;
    double i;
    double j;
    double k;
}Quaternion;//ËÄÔªÊý

void Quater_Init(Quaternion* Q,double a,double i,double j,double k){
    Q->a = a;
    Q->i = i;
    Q->j = j;
    Q->k = k;
}

void Quater_ADD(Quaternion* a, Quaternion* b){
    a->a += b->a;
    a->i += b->i;
    a->j += b->j;
    a->k += b->k;
}

void Quater_SUB(Quaternion* a, Quaternion* b){
    a->a -= b->a;
    a->i -= b->i;
    a->j -= b->j;
    a->k -= b->k;
}

void Quater_MUL(Quaternion* a, Quaternion* b){
    a->a = a->a * b->a - a->i * b->i - a->j * b->j - a->k * b->k;
    a->i = a->a * b->i + b->a * a->i + a->j * b->k - a->k * b->j;
    a->j = a->a * b->j + b->a * a->j + a->k * b->i - a->i * b->k;
    a->k = a->a * b->k + b->a * b->k + a->i * b->j - a->j * b->i;
}

double Quater_Magnitude(Quaternion* a){
    return (a->a * a->a + a->i * a->i + a->j * a->j + a->k * a->k);
}

Quaternion Quater_conjugate(Quaternion a){
    Quaternion ans ; 
    ans.a = a.a ;
    ans.i = -a.i ;
    ans.j = -a.j ;
    ans.k = -a.k ;
}


