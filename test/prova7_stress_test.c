int main() {
    int a = 5;          // variabile intera
    int b = -10;        // valore negativo
    float f = 0.5;      // float
    bool attivo = true; // bool
    char c = 'X';       // char
    int mai_usata = 42; // questa non viene mai usata

    // assegnamenti base
    int somma = a + b;  // somma
    int diff = a - b;   // differenza

    // if else
    if (a > 0) {
        somma = somma + 1; // incremento
    } else {
        diff = diff - 1;   // decremento
    }

    // while
    int contatore = 0; // contatore loop
    while (contatore < a) {
        contatore = contatore + 1; // incremento contatore
    }

    // for
    int accumulatore = 0;
    for (int j = 0; j < b; j = j + 1) {
        accumulatore = accumulatore + j; // accumulo
    }

    // if annidato con assegnamenti
    if (attivo) {
        if (somma > diff) {
            accumulatore = accumulatore + 1;
        } else {
            accumulatore = accumulatore - 1;
        }
    }

    // operatori unari
    int valore = + - + - a; // unari concatenati
    int altro_valore = - -b; // doppio negativo

    return valore + altro_valore + accumulatore; // return finale
}