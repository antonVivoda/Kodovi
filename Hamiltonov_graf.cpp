#include <iostream>
using namespace std;

template <class T> class Stack {
    private:

        int increment = 100;
        size_t size = 100;
        T *stack;
        int top = -1;

    public:

        Stack(){
            stack = (T*)malloc(size * sizeof(T));
        }
        Stack(size_t size) : size(size) {
            stack = (T*)malloc(this.size * sizeof(T));
        }
        
        bool push(T item){
            if (top >= (int)(size - 1)){
                T *tmp = (T*)realloc(stack, (size + increment) * sizeof(T));
                if (!tmp) return false;
                stack = tmp;
                size += increment;
            }
            stack[++top] = item;
            return true;
        }

        bool pop(T &item){
            if (top < 0) return false;
            item = stack[top--];
            if (top != -1 && top < (int)(size - increment)){
                T *tmp = (T*)realloc(stack, (size - increment) * sizeof(T));
                if (!tmp) return false;
                stack = tmp;
                size -= increment;
            }
            return true;
        }
        
};

void matSusjedstva(int *A, int n,  int k_[]){
    int i,j,k;
    bool check;
    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++){
            check = false;
            for (k = 0; k < 4; k++){
                if (abs(i - j) == k_[k]){
                    A[i * n + j] = 1;
                    check = true;
                    break;
                }
            }
            if (!check){
                A[i * n + j] = 0;
            }
        }
    }
    return;

}

bool provjeriPovezanost(int *mat, int n){
    if (n < 1) return false;
    Stack<int> toVisit;
    bool visited[n] = {};
    if (n == 1) return true;
    visited[0] = true;
    toVisit.push(0);

    int vrh;
    while (toVisit.pop(vrh)){
        for (int j = 0; j < n; j++){
            if (mat[vrh * n + j] == 1){
                if (visited[j] == false){
                    visited[j] = true;
                    toVisit.push(j);
                }
            }
        }
    }

    bool result = true;
    for (int i = 0; i < n; i++){
        if (visited[i] == false){
            result = false;
            break;
        }
    }

    return result;
}

bool provjeriVrh(int V, int *mat, int n, int path[], int pos){
    if (mat[path[pos - 1] * n + V] == 1){
        bool check = true;
        for (int j = 1; j < pos; j++){
            if (path[j] == V){
                check = false;
            }
        }
        return check;
    }
    return false;
}

bool provjeriHamCiklus(int *mat, int n, int path[], int pos){
    if (pos == n){
        if (mat[path[n - 1] * n + path[0]] == 1){
            return true;
        } else {
            return false;
        }
    }

    for (int i = 1; i < n; i++){
        if (provjeriVrh(i, mat, n, path, pos)){
            path[pos] = i;
            if (provjeriHamCiklus(mat, n, path, pos + 1)){
                return true;
            } else{
                path[pos] = -1;
            }
        }
    }
    return false;
}

bool hamiltonovski(int *mat, int n){
    if (n < 3) return false;
    bool dirac = true;
    int sum = 0;
    for (int i = 0; i < n; i++){
         sum = 0;
         for(int j = 0; j < n; j++){
            sum += mat[i * n + j];
        }
        if (sum < (n / 2.)){
            dirac = false;
            break;
        }
    }
    if (dirac) return true;

    int path[n];
    path[0] = 0;
    for (int i = 1; i < n; i++){
        path[i] = -1;
    }

    if (provjeriHamCiklus(mat, n, path, 1)){
        return true;
    } 
    return false;
}

int main(void) {
    int n, k_[4];
    cout << "Unesite prirodan broj n: ";
    cin >> n;
    for (int i = 0; i < 4; i++){
        cout << "Unesite vrijednost prirodnog broja k_" << i + 1 << ": ";
        cin >> k_[i];
    }
    cout << "\n";

    //matrica susjedstva
    int susjedstvo[n][n];
    matSusjedstva(susjedstvo[0], n, k_);
    
    
    //povezanost
    bool povezanost = provjeriPovezanost(susjedstvo[0], n);

    if (!povezanost) {
        cout << "Graf G nije povezan graf\n";
        cout << "Graf G nije hamiltonovski graf";
    } else {
        if (hamiltonovski(susjedstvo[0], n) == true){
            cout << "Graf G je povezan graf\n";
            cout << "Graf G je hamiltonovski graf\n";
            cout << "Duljina ciklusa: " << n;
        } else {
            cout << "Graf G je povezan graf\n";
            cout << "Graf G nije hamiltonovski graf";
        }
    }

    return 0;
}