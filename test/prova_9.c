typedef unsigned long ulong;
typedef struct {
    int x;
    int y;
} Punto;

int main(int argc, char *argv[]){
    const int a = 5;
    extern long b;
    static int c = 10;
    volatile int d;
    const unsigned int e = 3;
    extern const int f;
    ulong g = 100;
    Punto p;
    
    //1 bisogna controllare il valore associato alla variabile?
    int test = 10;
    char* c[5] = "12345 6"; //matrici

    
    //variabili multiple ?
    int v1,v2,v3;

    
    return 0;
}

