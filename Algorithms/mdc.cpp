#include <iostream>
using namespace std;
int MDC(int a, int b) { 
    int c;
    while (b != 0) {
        c = a % b;
        a = b;
        b = c;
    }
    return a;
}

int main() {
    int mdc = MDC(199, 17);
    cout << "MDC: "<<mdc<<endl;
    return 0;
}
