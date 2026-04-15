// Caso 1: singola riga
typedef struct { int a; int b; } Point;

// Caso 2: multi-riga
typedef struct {
    int a;
    int b;
} Point;

// Caso 3: con tag opzionale
typedef struct PointTag { int x; } Point;

// Caso 4: multipli alias
typedef struct { int v; } Vec, *VecPtr;
