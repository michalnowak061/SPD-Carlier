#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

#define HEAPSIZE 10000

using namespace std;

class Process {
public:
    int r, p, q;    // <- wartości r,p,q
    int number;     // <- numer zadania
    int c;          // <- czas dostarczenia zadania
    
    void Print() {
        cout << endl << "Numer zadania: " << number << endl <<
        "r: " << r << " p: " << p << " q: " << q << endl;
    }
};

// ------------------------------- Algorytm Schrage -------------------------------------------------------------------------------------------------------------------
// Algorytm zaimplementowany przy pomocy pseudokodu pochodzacego ze strony:
// http://mariusz.makuchowski.staff.iiar.pwr.wroc.pl/download/courses/sterowanie.procesami.dyskretnymi/lab.instrukcje/lab04.schrage/schr.literatura/a.schrage.pdf
/* Dane:
 * N - liczba zadań
 * R[i] - termin dostępności i-tego zadania
 * P[i] - czas wykonania i-tego zadania
 * Q[i] - czas dostarczenia i-tego zadania
 *
 * Szukane:
 * Permutacja wykonania zadań na maszynie, Cmax - maksymalny z terminów dostarczenia zadań
 *
 * Struktury pomocnicze:
 * t – chwila czasowa,
 * k – pozycja w permutacji π
 * N – zbiór zadań nieuszeregowanych
 * G – zbiór zadań gotowych do realizacji
 */
int Schrage(Process *Process_Array, int n);

// ------------------------- Algorytm Schrage z podziałem -------------------------------------------------------------------------------------------------------------
// Algorytm zaimplementowany przy pomocy pseudokodu pochodzacego ze strony:
// http://mariusz.makuchowski.staff.iiar.pwr.wroc.pl/download/courses/sterowanie.procesami.dyskretnymi/lab.instrukcje/lab04.schrage/schr.literatura/a.schrage.pdf
/* Dane:
 * N - liczba zadań
 * R[i] - termin dostępności i-tego zadania
 * P[i] - czas wykonania i-tego zadania
 * Q[i] - czas dostarczenia i-tego zadania
 *
 * Szukane:
 * Permutacja wykonania zadań na maszynie, Cmax - maksymalny z terminów dostarczenia zadań
 *
 * Struktury pomocnicze:
 * t – chwila czasowa,
 * k – pozycja w permutacji Pi
 * l - aktualne wykonywane zadanie
 * N – zbiór zadań nieuszeregowanych
 * G – zbiór zadań gotowych do realizacji
 */
int SchrageWithParity(Process *Process_Array, int n);

// ------------------------- Algorytm Carlier  ------------------------------------------------------------------------------------------------------------------------
// Algorytm zaimplementowany przy pomocy pseudokodu pochodzacego ze strony:
// http://dominik.zelazny.staff.iiar.pwr.wroc.pl/materialy/Algorytm_Carlier.pdf
/* Dane:
 * N - liczba zadań
 * R[i] - termin dostępności i-tego zadania
 * P[i] - czas wykonania i-tego zadania
 * Q[i] - czas dostarczenia i-tego zadania
 * UB – górne oszacowanie wartości funkcji celu (wartości funkcji celu dla najlepszego dotychczas rozwiązania
 *
 * Szukane:
 * Pi* - optymalna permutacja wykonania zadań na maszynie
 *
 * Struktury pomocnicze:
 * Pi - permutacja wykonania zadań na maszynie
 * U – wartość funkcji celu
 * LB – dolne oszacowanie wartości funkcji celu
 */
int Carlier(Process *Process_Array, int n, int UB);

/* Funkcje pomocnicze */
int find_b(Process *Process_Array, int n, int C_max);
int find_a(Process *Process_Array, int n, int C_max, int b);
int find_c(Process *Process_Array, int n, int b, int a);

// ------------------------- Implementacja kolejki priorytetowej ------------------------------------------------------------------------------------------------------
/* Źródło: http://mariusz.makuchowski.staff.iiar.pwr.wroc.pl/download/courses/sterowanie.procesami.dyskretnymi/lab.instrukcje/lab04.schrage/heap.demo.v1.5/demoheap.exe
 * Autor: Dr inż. Mariusz Makuchowski
 * Modyfikacje:
 * - stworzenie klasy ProcessPriorityQueue
 * - dodanie metody Print(), isEmpty()
 * - zmiana nazw niektórych zmiennych
 * Autorzy modyfikacji: Michał Nowak i Mariusz Perskawiec
 */

class ProcessPriorityQueue {
    
private:
    
    Process *array;
    int ActualSize;
    int HeapSize;
    bool maxHeap;
    char priority;
    
public:
    
    ProcessPriorityQueue(int _heapSize, char _priority, bool maxheap) {
        ActualSize = 0;
        HeapSize = _heapSize;
        array = new Process[HeapSize];
        maxHeap = maxheap;
        priority = _priority;
    }
    
    void ShiftDownMax(int node);
    void ShiftUpMax(int node);
    void ShiftDownMin(int node);
    void ShiftUpMin(int node);
    void Make();
    void HeapSort();
    
    void Push(Process value);
    void Pop();
    Process Top();
    
    bool isEmpty();
    void Print();
    
};

void ProcessPriorityQueue::ShiftDownMax(int node){
    
    int next = 2 * node;
    
    while (next <= ActualSize) {
        
        if (priority == 'r') {
            
            if ( (next < ActualSize && (array[next].r < array[next+1].r) ) ) {
                next++;
            }
            
            if (array[node].r < array[next].r) {
                swap(array[node], array[next]);
                node = next;
                next *= 2;
            }
            else {
                next = ActualSize + 1;
            }
        }
        else if (priority == 'p') {
            
            if ( (next < ActualSize && (array[next].p < array[next+1].p) ) ) {
                next++;
            }
            
            if (array[node].p < array[next].p) {
                swap(array[node], array[next]);
                node = next;
                next *= 2;
            }
            else {
                next = ActualSize + 1;
            }
        }
        else if (priority == 'q') {
            
            if ( (next < ActualSize && (array[next].q < array[next+1].q) ) ) {
                next++;
            }
            
            if (array[node].q < array[next].q) {
                swap(array[node], array[next]);
                node = next;
                next *= 2;
            }
            else {
                next = ActualSize + 1;
            }
        }
    }
}

void ProcessPriorityQueue::ShiftUpMax(int node){
    
    int next = node / 2;
    
    if (priority == 'r') {
        
        while ( (node > 1) && (array[next].r < array[node].r) ) {
            swap(array[node], array[next]);
            node = next;
            next /= 2;
        }
    }
    else if (priority == 'p') {
        
        while ( (node > 1) && (array[next].p < array[node].p) ) {
            swap(array[node], array[next]);
            node = next;
            next /= 2;
        }
    }
    else if (priority == 'q') {
        
        while ( (node > 1) && (array[next].q < array[node].q) ) {
            swap(array[node], array[next]);
            node = next;
            next /= 2;
        }
    }
}

void ProcessPriorityQueue::ShiftDownMin(int node){
    
    int next = 2 * node;
    
    while (next <= ActualSize) {
        
        if (priority == 'r') {
            
            if ( (next < ActualSize && (array[next].r > array[next+1].r) ) ) {
                next++;
            }
            
            if (array[node].r > array[next].r) {
                swap(array[node], array[next]);
                node = next;
                next *= 2;
            }
            else {
                next = ActualSize + 1;
            }
        }
        else if (priority == 'p') {
            
            if ( (next < ActualSize && (array[next].p > array[next+1].p) ) ) {
                next++;
            }
            
            if (array[node].p > array[next].p) {
                swap(array[node], array[next]);
                node = next;
                next *= 2;
            }
            else {
                next = ActualSize + 1;
            }
        }
        else if (priority == 'q') {
            
            if ( (next < ActualSize && (array[next].q > array[next+1].q) ) ) {
                next++;
            }
            
            if (array[node].q > array[next].q) {
                swap(array[node], array[next]);
                node = next;
                next *= 2;
            }
            else {
                next = ActualSize + 1;
            }
        }
    }
}

void ProcessPriorityQueue::ShiftUpMin(int node){
    
    int next = node / 2;
    
    if (priority == 'r') {
        
        while ( (node > 1) && (array[next].r > array[node].r) ) {
            swap(array[node], array[next]);
            node = next;
            next /= 2;
        }
    }
    else if (priority == 'p') {
        
        while ( (node > 1) && (array[next].p > array[node].p) ) {
            swap(array[node], array[next]);
            node = next;
            next /= 2;
        }
    }
    else if (priority == 'q') {
        
        while ( (node > 1) && (array[next].q > array[node].q) ) {
            swap(array[node], array[next]);
            node = next;
            next /= 2;
        }
    }
}

void ProcessPriorityQueue::Make() {
    
    if (maxHeap == true) {
        
        for (int count = ActualSize / 2; count > 0; count--) {
            ShiftDownMax(count);
        }
    }
    else if (maxHeap == false) {
        for (int count = ActualSize / 2; count > 0; count--) {
            ShiftDownMin(count);
        }
    }
}

void ProcessPriorityQueue::Push(Process value) {
    
    array[++ActualSize] = value;
    
    if (maxHeap == true) {
        ShiftUpMax(ActualSize);
    }
    else if (maxHeap == false){
        ShiftUpMin(ActualSize);
    }
}

void ProcessPriorityQueue::Pop() {
    
    swap(array[1], array[ActualSize--]);
    
    if (maxHeap == true) {
        ShiftDownMax(1);
    }
    else if(maxHeap == false){
        ShiftDownMin(1);
    }
}

Process ProcessPriorityQueue::Top() {
    
    if (ActualSize == 0) {
        perror("Niedozwolona operacja TOP");
    }
    
    return array[1];
}

void ProcessPriorityQueue::HeapSort() {
    
    Make();
    
    while ( ActualSize > 0 ) {
        Pop();
    }
}

bool ProcessPriorityQueue::isEmpty() {
    
    if (ActualSize == 0) {
        return true;
    }
    else return false;
}

void ProcessPriorityQueue::Print() {
    
    cout << endl << "Heap elements: " << endl;
    
    for (int count = 1; count <= ActualSize ; count++) {
        array[count].Print();
    }
    
    cout << endl;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------

int main(void) {
    
    int N = 0; // Liczba danych
    string s;
    
    ifstream data("carl.data.txt");
    
    while( s != "data.000:" ) {
        data >> s;
    }
    
    data >> N;
    
    Process *Process_Array = new Process[N];
    
    //cout << "---- Wczytane zadania -----" << endl;
    for(int i = 1; i <= N; i++) {
        
        Process temp_process;
        
        temp_process.number = i;
        data >> temp_process.r >> temp_process.p >> temp_process.q;
        
        Process_Array[i] = temp_process;
        //Process_Array[i].Print();
    }
    
    Carlier(Process_Array, N, INT_MAX);
    
    //system("pause");
    
    return 0;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------

int Schrage(Process *Process_Array, int n) {
    
    // krok 1: inicjacja wszystkich zmiennych
    int k = 1;                                      // <- pozycja w permutacji
    int Cmax = 0;                                   // <- maksymalny z terminów dostarczenia zadań
    Process *PI = Process_Array;                    // <- permutacje zadań
    ProcessPriorityQueue N(100, 'r', false);        // <- zbiór zadań nieuszeregowanych
    ProcessPriorityQueue G(100, 'q', true);         // <- zbiór zadań gotowych do realizacji
    
    for (int i = 1; i <= n; i++) {
        N.Push(PI[i]);
    }
    
    int t = N.Top().r;  // <- chwila czasowa
    
    // krok 3 i 4: ze zbioru zadań jeszcze nieuszeregowanych usuwane są i umieszczane w zbiorze G
    // zadania gotowe do realizacji w chwili t, tj. zadania których termin dostępności ri jest
    // mniejszy lub równy chwili t.
    while ( (G.isEmpty() == false) || (N.isEmpty() == false) ) {
        
        while ( (N.isEmpty() == false) && (N.Top().r <= t) ) {
            
            Process e = N.Top();
            G.Push(e);
            N.Pop();
        }
        
        // krok 5: sprawdzane jest czy zbiór zadań gotowych w chwili t nie jest pusty. Jeżeli jest
        // to chwila czasowa t przesuwana jest do momentu dostępności najwcześniejszego zadania ze
        // zbioru N i wznawiany jest proces aktualizowania zbioru zadań gotowych.
        if (G.isEmpty() == true) {
            
            t = N.Top().r;
        }
        else {
            // krok 7: ze zboru zadań gotowych wyznaczane jest zadanie o największym qi, zadanie
            // to usuwane jest ze zbioru G
            Process e = G.Top();
            G.Pop();
            
            // krok 8: zwiększany jest o jeden indeks k określający pozycję w permutacji PI,
            // zadanie e wstawiane jest na tą pozycję, chwila czasowa t zwiększana jest o czas
            // realizacji zadania, uaktualniany jest najpóźniejszy moment dostarczenia Cmax
            PI[k] = e;
            t += PI[k].p;
            
            Cmax = max(Cmax, t + PI[k].q);
            
            PI[k].c = t; // wyliczenie zmiennej potrzebnej w algorytmie Carliera
            
            k++;
        }
    }
    
    /*
    cout << endl << "------ Wyznaczona permutacja PI Schrage bez podziału  ---------" << endl;
    for (int i = 1; i <= n; i++) {
        PI[i].Print();
    }
    
    cout << endl << "Cmax wyznaczone za pomoca Schrage bez podziału: " << Cmax << endl;
    */
    
    return Cmax;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------

int SchrageWithParity(Process *Process_Array, int n) {
    
    // krok 1: inicjacja wszystkich zmiennych
    int k = 1;                                      // <- pozycja w permutacji
    int Cmax = 0;                                   // <- maksymalny z terminów dostarczenia zadań
    Process l;                                      // <- aktualne wykonywane zadanie
    l.r = 0; l.p = 0; l.q = 0;
    Process *PI = Process_Array;                    // <- permutacje zadań
    ProcessPriorityQueue N(100, 'r', false);        // <- zbiór zadań nieuszeregowanych
    ProcessPriorityQueue G(100, 'q', true);         // <- zbiór zadań gotowych do realizacji
    
    for (int i = 1; i <= n; i++) {
        N.Push(PI[i]);
    }
    
    int t = N.Top().r;  // <- chwila czasowa
    
    // krok 3 i 4: ze zbioru zadań jeszcze nieuszeregowanych usuwane są i umieszczane w zbiorze G
    // zadania gotowe do realizacji w chwili t, tj. zadania których termin dostępności ri jest
    // mniejszy lub równy chwili t.
    while ( (G.isEmpty() == false) || (N.isEmpty() == false) ) {
        
        while ( (N.isEmpty() == false) && (N.Top().r <= t) ) {
            
            Process e = N.Top();
            G.Push(e);
            N.Pop();
            
            // krok 5: W kroku tym, za każdy razem gdy do zbioru zadań gotowych dodawane jest zadanie (e),
            // sprawdzane jest czy ma większy czas dostarczenia od zadania (l) aktualnie znajdującego się na
            // maszynie. Jeżeli tak to wykonywanie zadania l natychmiast jest przerywane, a pozostała część
            // zadania tj. o czasie trwania t − re ponownie wkładana jest do zbioru zadań gotowych do realizacji.
            if (e.q > l.q) {
                
                l.p = t - e.r;
                t = e.r;
                
                if (l.p > 0) {
                    
                    G.Push(l);
                }
            }
        }
        
        // krok 6: sprawdzane jest czy zbiór zadań gotowych w chwili t nie jest pusty. Jeżeli jest
        // to chwila czasowa t przesuwana jest do momentu dostępności najwcześniejszego zadania ze
        // zbioru N i wznawiany jest proces aktualizowania zbioru zadań gotowych.
        if (G.isEmpty() == true) {
            
            t = N.Top().r;
        }
        else {
            // krok 8: ze zboru zadań gotowych wyznaczane jest zadanie o największym qi, zadanie
            // to usuwane jest ze zbioru G
            Process e = G.Top();
            G.Pop();
            
            // krok 9: zwiększany jest o jeden indeks k określający pozycję w permutacji PI,
            // zadanie e wstawiane jest na tą pozycję, chwila czasowa t zwiększana jest o czas
            // realizacji zadania, uaktualniany jest najpóźniejszy moment dostarczenia Cmax, oraz
            // uaktualniana jest zmienna l
            PI[k] = e;
            l = e;
            t += PI[k].p;
            
            Cmax = max(Cmax, t + PI[k].q);
            
            k++;
        }
    }
    
    /*
    cout << endl << "------ Wyznaczona permutacja PI Schrage z podziałem ---------" << endl;
    
    for (int i = 1; i <= n; i++) {
        PI[i].Print();
    }
    
    cout << endl << "Cmax wyznaczone za pomoca Schrage z podziałem: " << Cmax << endl;
    */
    return Cmax;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------

int find_b(Process *Process_Array, int n, int C_max) {
    
    int b = 0;
    
    // szukamy najwiekszego b, dlatego od prawej
    for(int j = n; j >= 1; j--) {
        
        if( C_max == Process_Array[j].c + Process_Array[j].q ) {
            
            b = j;
            return b;
        }
    }
    
    return -1; // b nie istnieje
}

int find_a(Process *Process_Array, int n, int C_max, int b) {
    
    int a = 0;
    int sum = 0;
    
    // szukamy najmniejszego a, dlatego od lewej
    for (int j = 1; j <= n; j++) {
        
        sum = 0;
        
        for (int s = j; s <= b; s++) {
            
            sum += Process_Array[s].p;
        }
        
        if ( C_max == (Process_Array[j].r + sum + Process_Array[b].q) ) {
            
            a = j;
            return a;
        }
    }
    
    return -1; // a nie istnieje
}

int find_c(Process *Process_Array, int n, int b, int a) {
    
    int c = 0;
    
    // szukamy największego c, dlatego od prawej
    for (int j = b; j >= a; j--) {
        
        if ( Process_Array[j].q < Process_Array[b].q ) {
            
            c = j;
            return c;
        }
    }
    
    return -1; // c nie istnieje
}

int Carlier(Process *Process_Array, int n, int UB) {
    
    static int iteration = 1;       // <- numer wywolania rekurencyjnego
    
    Process *PI = Process_Array;    // <- permutacje zadań
    
    static int U  = 0;                     // <- wartość funkcji celu
    static int LB = 0;                     // <- dolne oszacowanie
    
    int a = 0;                      // <- numer pierwszego zadania w bloku K
    int b = 0;                      // <- numer ostatniego zadania w bloku K
    int c = 0;                      // <- numer zadania przeszkadzajacęgo
    
    int r_prim = INT_MAX;           // <- nowe r dla zadania c
    int p_prim = 0;                 // <- suma p
    int q_prim = INT_MAX;           // <- nowe q dla zadania c
    
    int r_ref  = 0;                 // <- zapamiętane r zadania c
    int q_ref  = 0;                 // <- zapamiętane q zadania c
    int nr_ref = 0;                 // <- zapamiętany numer zadania c
    
    // krok 1: następuje wyznaczenie permutacji wykonywania zadań algorytmem Schrage
    U = Schrage(Process_Array, n);
    
    // krok 2: uaktualniane jest najlepsze do tej pory znalezione rozwiązanie
    if(U < UB) {
        
        UB = U;
        PI = Process_Array;
    }
    
    // krok 3: wyznaczany jest blok (a,b) oraz pozycja zadania interferencyjnego
    b = find_b(Process_Array, n, U);
    a = find_a(Process_Array, n, U, b);
    c = find_c(Process_Array, n, b, a);
    
    // krok 4: jeżeli tego typu zadanie nie istnieje (alg. Shrage wygenerował rozwiązanie optymalne), następuje powrót z procedury
    if(c == -1) {
        
        return U;
    }
    
    // krok 5: wyznaczany jest najmniejszy z terminów dostępności oraz największy z terminów dostarczenia zadań stojących na pozycjach
    // od c+1 do b, dodatkowo wyznaczana jest suma czasów wykonania zadań
    for (int j = c + 1; j <= b; j++) {
        
        r_prim  = min(r_prim, Process_Array[j].r);
        q_prim  = min(q_prim, Process_Array[j].q);
        p_prim += Process_Array[j].p;
    }
    
    // krok 6: modyfikowany jest termin dostępności zadania referencyjnego (wymuszane jest aby zadanie referencyjne wykonywane było
    // za wszystkimi zadaniami na tych pozycjach)
    Process_Array[c].r = max(Process_Array[c].r, r_prim + p_prim);
    
    // zapamietanie
    nr_ref = Process_Array[c].number;
    r_ref = Process_Array[c].r;
    
    // krok 7: wyznaczane jest dolne ograniczenie dla wszystkich permutacji spełniających to wymaganie
    LB = SchrageWithParity(Process_Array, n);
    
    // krok 8 i 9: rekurencyjnie wywoływane jest rozwiązanie nowego problemu
    if(LB < UB) {
        
        Carlier(Process_Array, n, UB);
    }
    
    // krok 10: po powrocie odtwarzany jest termin dostępności rozwiązania referencyjnego
    for (int j = 1; j <= n; j++) {
        
        if (nr_ref == Process_Array[j].number) {
            
            Process_Array[j].r = r_ref; // odtwórz r zadania c
        }
    }
    
    // krok 11-15: wykonywane są analogiczne czynności, przy czym modyfikowany jest termin dostarczenia zadania referencyjnego
    // (wymuszane jest aby zadanie referencyjne wykonywane było przed wszystkimi zadaniami na pozycjach od c+1 do b)
    Process_Array[c].q = max(Process_Array[c].q, q_prim + p_prim);
    
    // zapamietanie
    nr_ref = Process_Array[c].number;
    q_ref = Process_Array[c].q;
    
    LB = SchrageWithParity(Process_Array, n);
    
    if(LB < UB) {
        
        Carlier(Process_Array, n, UB);
    }
    
    for (int j = 1; j <= n; j++) {
        
        // jeżeli numer zadania zgadza się z zapamiętanym
        if( nr_ref == Process_Array[j].number ) {
            
             Process_Array[j].q = q_ref; // odtwórz q zadania c
        }
    }
    
    cout << iteration << " " << " U: " << U << endl;
    cout << iteration << " " << " UL: " << LB << endl;
    cout << iteration << " " << " UB: " << UB << endl;
    cout << iteration << " " << " a: " << a << endl;
    cout << iteration << " " << " b: " << b << endl;
    cout << iteration << " " << " c: " << c << endl;
    
    if(iteration == 1) {
        
        cout << endl << "------ Wyznaczona permutacja PI Carlier ---------" << endl;
        
        for (int i = 1; i <= n; i++) {
            PI[i].Print();
        }
        
        cout << endl << "Długość uszeregowania wyznaczona algorytmem Carliera: " << U << endl;
        
    }
    
    iteration++;
    
    return U;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------
