#include "model.h"
#include "grid-template.h"

#include <iostream>

class Test
{
public:
    double a;
    void init(void*) {}
};

int main()
{
    using namespace std;
    cout << "Starting modelling" << endl;
    
    Grid<1, Test>::GridDescription descr;
    Grid<1, Test> grid;
    
    grid.constructGrid(descr);
    
    Model model;
    cout << "Modelling done" << endl;
    return 0; 
}
