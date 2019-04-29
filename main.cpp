#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

#define HEAPSIZE 10000

using namespace std;

class Process {
public:
    int r, p, q, number;
    
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
 * k – pozycja w permutacji π
 * l - aktualne wykonywane zadanie
 * N – zbiór zadań nieuszeregowanych
 * G – zbiór zadań gotowych do realizacji
 */
int SchrageWithParity(Process *Process_Array, int n);

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
    
    while( s != "data.008:" ) {
        data >> s;
    }
    
    data >> N;
    
    Process *Process_Array = new Process[N];
    
    cout << "---- Wczytane zadania -----" << endl;
    for(int i = 1; i <= N; i++) {
        
        Process temp_process;
        
        temp_process.number = i;
        data >> temp_process.r >> temp_process.p >> temp_process.q;
        
        Process_Array[i] = temp_process;
        Process_Array[i].Print();
    }
    
    Schrage(Process_Array,N);
    SchrageWithParity(Process_Array,N);
    
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
            
            k++;
        }
    }
    
    cout << endl << "------ Wyznaczona permutacja PI Schrage bez podziału  ---------" << endl;
    for (int i = 1; i <= n; i++) {
        PI[i].Print();
    }
    
    cout << endl << "Cmax wyznaczone za pomoca Schrage bez podziału: " << Cmax << endl;
    
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
    
    cout << endl << "------ Wyznaczona permutacja PI Schrage z podziałem ---------" << endl;
    
    for (int i = 1; i <= n; i++) {
        PI[i].Print();
    }
    
    cout << endl << "Cmax wyznaczone za pomoca Schrage z podziałem: " << Cmax << endl;
    
    return Cmax;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------

