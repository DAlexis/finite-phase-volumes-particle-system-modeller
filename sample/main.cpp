#include "model.h"
#include "grid-template.h"

#include <iostream>

int main()
{
    using namespace std;
    cout << "Starting modelling" << endl;
    
    GridDescription<1> descr;
    Grid<1, 1> grid;
    
    Model model;
    cout << "Modelling done" << endl;
    return 0; 
}
