#include "model.h"
#include "grid-template.h"

#include <iostream>

int main()
{
    using namespace std;
    cout << "Initialising model... ";
    
    Model model;
    cout << "Done" << endl;
    
    cout << "Starting modelling" << endl;
    
    const double dt = 0.01;
    
    while (model.time<5)
        model.iterate(dt);
        
    cout << "Modelling done" << endl;
    return 0; 
}
