#include "model.h"
#include "grid-template.h"

#include <iostream>

int main()
{
    using namespace std;
    cout << "Starting modelling" << endl;
    
    Grid<1, double>::GridDescription descr;
    Grid<1, double> grid;
    
    grid.constructGrid(descr);
    
    Model model;
    cout << "Modelling done" << endl;
    return 0; 
}
