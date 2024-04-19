#include <iostream>
#include "Multiset.h"

int main()
{
    try {
        Multiset multiSet(5, 3);

        multiSet.add(1);

        multiSet.add(2);
        multiSet.add(2);

        /*multiSet.add(3);
        multiSet.add(3);
        multiSet.add(3);*/

        multiSet.add(4);
        multiSet.add(4);
        multiSet.add(4);
        multiSet.add(4);

        multiSet.add(5);
        multiSet.add(5);
        multiSet.add(5);
        multiSet.add(5);
        multiSet.add(5);
        multiSet.add(5);
        multiSet.add(5);

        multiSet.printCounts();
        std::cout << std::endl;

        multiSet.print();
        std::cout << std::endl;

        multiSet.printInMemory();

        std::cout << "From file -----------------------------" << std::endl;

        Multiset multiSetFile(10, 8);
        multiSetFile.readFromBinaryFile("multiculti.txt");

        multiSetFile.printCounts();
        std::cout << std::endl;

        multiSetFile.print();
        std::cout << std::endl;

        multiSetFile.printInMemory();

        std::cout << "Union -----------------------------" << std::endl;

        Multiset unionM = multisetsUnion(multiSet, multiSetFile);

        unionM.printCounts();
        std::cout << std::endl;

        unionM.print();
        std::cout << std::endl;

        unionM.printInMemory();

        std::cout << "Intersection -----------------------------" << std::endl;

        Multiset intersection = multisetsIntersection(multiSet, multiSetFile);

        intersection.printCounts();
        std::cout << std::endl;

        intersection.print();
        std::cout << std::endl;

        intersection.printInMemory();

        std::cout << "Difference -----------------------------" << std::endl;

        Multiset diff = multisetsDifference(multiSet, multiSetFile);

        diff.printCounts();
        std::cout << std::endl;

        diff.print();
        std::cout << std::endl;

        diff.printInMemory();

        std::cout << "Addition -----------------------------" << std::endl;

        Multiset addition = multisetsAddition(multiSet);

        addition.printCounts();
        std::cout << std::endl;

        addition.print();
        std::cout << std::endl;

        addition.printInMemory();
    }
    catch (std::logic_error& e) {
        std::cout << e.what() << std::endl; // съобщението на грешката
    }
}