#define     FOO(p)      g(p); f(p); //IF error
#define     FOO(p)      { g(p); f(p); } 



int main(int argc, char const *argv[])
{
    int p = 4;
    if(1 < 4)
        FOO(p);

    {
        

    };
    return 0;
}
