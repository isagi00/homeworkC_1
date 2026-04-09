int main() {
    int x = 10;
    int y = -7;
    int z;
    float pi = 3.14;
    float unused_float = 2.71;
    bool flag = true;
    char lettera = 'A';
    char unused_char = 'z';

    // assegnamenti semplici
    z = x + y;
    x = z * 2;

    if (x > 0) {
        z = z + 1;
    } else {
        z = z - 1;
    }

    // while con condizione composta
    while (z > 0) {
        z = z - 1;
    }

    // for normale
    for (int k = 0; k < x; k = k + 1) {
        y = y + k;
    }

    // assegnamenti con operatori unari
    int risultato = x + - -y;
    int altro = + - + z;

    // if annidato
    if (flag) {
        if (x > 5) {
            risultato = risultato + 1;
        }
    }

    return risultato + altro;
}