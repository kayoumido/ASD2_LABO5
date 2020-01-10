#include <iostream>
#include <STLDictionary.h>

using namespace std;

int main() {

    STLDictionary dict("data/small_dict.txt");

    cout << dict.find("afterward");
    return 0;
}
