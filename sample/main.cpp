#include "model.h"
#include "grid-template.h"

#include <iostream>

int main()
{
    using namespace std;
    cout << "Initialising model... ";
    
    //Fraction1Space testSpace(NULL);
//    FractionsPool pool;
    //static_cast<FractionsPoolBase*>(&pool)->createFractions();
    
    Model model;
    cout << "Done" << endl;
    /*
    cout << "Starting modelling" << endl;
    
    const double dt = 0.01;
    
    for (double t=0; t<5; t+=dt)
        model.iterate(dt);
        
    cout << "Modelling done" << endl;*/
    return 0; 
}
