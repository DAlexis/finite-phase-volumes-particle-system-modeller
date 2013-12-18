#include "model.h"
#include "grid-template.h"

#include <iostream>

int main()
{
    using namespace std;
    cout << "Starting modelling" << endl;
    Model model;
    model.iterate(0.01);
    cout << "Modelling done" << endl;
    return 0; 
}
