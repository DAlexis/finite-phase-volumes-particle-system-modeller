#include "model.h"
#include "grid-template.h"

#include <iostream>

int main()
{
    using namespace std;
    cout << "Starting modelling" << endl;
    
    GridDescription<1> descr;
    Grid<1, double> grid;
    
    grid.constructGrid(descr);
    
    Model model;
    cout << "Modelling done" << endl;
    return 0; 
}
