#include <iostream>
#include "Multiset.h"

int main()
{
    Multiset multiSet(10, 3);

    //// Perform 'add' operations to increment occurrence counts
    //multiSet.add(1); // Increment occurrence count for element 1
    //multiSet.add(1); // Increment occurrence count for element 2
    //multiSet.add(2); // Increment occurrence count for element 2 (again)
    //multiSet.add(1);

    //multiSet.add(5); 
    //multiSet.add(5);
    //multiSet.add(5);

    //multiSet.add(7);
    //multiSet.add(7);//???
    //multiSet.add(7);
    //multiSet.add(7);

    multiSet.add(0);

    multiSet.add(1);
    multiSet.add(2);
    
    multiSet.add(3);
    multiSet.add(4);
    multiSet.add(5);
    multiSet.add(5);
    multiSet.add(6);

    multiSet.add(6);
    multiSet.add(8);
    multiSet.add(7);
    multiSet.add(9);
 
    multiSet.add(10);

    // Print the current state of the counts vector
    multiSet.printCounts();
    std::cout << std::endl;

    multiSet.print();
    std::cout << std::endl;

    multiSet.printInMemory();
}