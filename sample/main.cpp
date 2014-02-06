#include "model.h"
#include "grid-template.h"

#include <iostream>
#include <stdlib.h>

int main()
{
    using namespace std;
    Model *model;
    try {
        cout << "Initialising model... ";
        model = new Model;
        cout << "Done." << endl;
    } catch (const exception& ex) {
        cout << endl << "Exception handled during model initialisation: " << ex.what();
        return EXIT_FAILURE;
    }
    
    try {
        cout << "Starting modelling" << endl;
        const double dt = 0.01;
        while (model->time<5)
            model->iterate(dt);
        cout << "Modelling done." << endl;
    } catch (const exception& ex) {
        cout << "Exception handled during modelling: " << ex.what();
        return EXIT_FAILURE;
    }
    
    delete model;
    return EXIT_SUCCESS;
}
