int main() {
    int a = 5;          /* variabile corretta */
    int 2b = 10;        /* errore: nome inizia con numero */
    float f = 3.14;     /* float corretto */
    int c = "ciao";     /* errore: stringa assegnata a int */
    bool attivo = forse; /* errore: valore bool non valido */
    char lettera = 'AB'; /* errore: char con piu caratteri */
    int mai_usata = 99; /* questa non viene mai usata */

    /* assegnamenti */
    int somma = a + f;  /* somma tra int e float */
    int k = 0;

    if (a > 0) {
        somma = somma + 1; /* incremento */
    } else {
        k = k - 1; /* decremento */
    }

    while (k < a) {
        k = k + 1; /* loop */
    }

    for (int j = 0; j < a; j = j + 1) {
        somma = somma + j; /* accumulo */
    }

    return somma + k; /* return corretto */
}