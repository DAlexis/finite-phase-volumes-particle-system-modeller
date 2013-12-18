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
    
    for (double t=0; t<0.1; t+=dt)
        model.iterate(dt);
        
    cout << "Modelling done" << endl;
    return 0; 
}
