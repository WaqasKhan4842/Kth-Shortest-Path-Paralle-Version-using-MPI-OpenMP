#include <iostream>
#include "Preprocessing.h"
#include "GraphProcessor.h"


int main()
{
    GraphProcessor processor;
    int choice = menu();
    int sub_choice; // For selecting from options inside the dataset.
    switch (choice)
    {
    case 1:
        sub_choice = choose_from_SNAP();
        processor.read_file_SNAP(sub_choice);
        break;
    case 2:
        sub_choice = choose_from_Doctor_Who();
        processor.read_file_DoctorWho(sub_choice);
        break;
    }
}
