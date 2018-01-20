#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

struct Warrior {

    string weapon;
    int power;
};


//Feel free to modify the parameters if you need other values
bool skirmish(Warrior*** protectors, Warrior*** invaders, int skirmish_row,
		      int rows, int columns, int &reserves, ofstream &output){

    // check if they are out of bounds; need >= because it's index vs length
    if (skirmish_row >= rows) { 

        // skip to next skirmish
        return false; 
    } 

    // iterate through all warriors fighting
    for (int i=0; i < rows; i++) {

        // if there's no invader, there is no skirmish
        if (invaders[skirmish_row][i]->power == -1) {
            
            output << "No Assault" << endl;

            // skip to next skirmish
            continue; 
        }

        // there's an invader but no protector --> walls breached
        if (protectors[i][skirmish_row]->power == -1) {
            
            // invaders WIN entirely, discontinue skirmishes
            return true;
        } 
    
        // now there must be an existing invader/protector, so begin fight!

        // invader wins 
        if ( ((invaders[skirmish_row][i]->weapon == "axe") && 
            (protectors[i][skirmish_row]->weapon == "sword"))
            || ((invaders[skirmish_row][i]->weapon == 
            protectors[i][skirmish_row]->weapon) &&
            (invaders[skirmish_row][i]->power > 
            protectors[i][skirmish_row]->power))) {
            
            // bool hasDefected = false;

            // check if there's open spots in the invaders team
            for (int u=0; u < rows; u++) {

                for (int j=0; j < rows; j++) {

                    if (invaders[u][j]->power == -1) {

                        // defect: "replace" the empty spot with the protector 
                        invaders[skirmish_row][j]->power = 
                            protectors[i][skirmish_row]->power;
                        invaders[skirmish_row][j]->weapon = 
                            protectors[i][skirmish_row]->weapon;

                        // stop checking for empty spots once one defects
                        output << "Protector defected" << endl;
                        // hasDefected = true;

                        if (reserves > 0) {

                            reserves--;
                            protectors[i][skirmish_row]->power = 100;
                            protectors[i][skirmish_row]->weapon = "axe";
                        }

                    } else {
                        output << "Protector killed" << endl;
                        protectors[i][skirmish_row]->power = -1;
                    }
                }   
            }
        }

        // protector wins
        if ( (invaders[skirmish_row][i]->weapon == "sword" && 
            protectors[i][skirmish_row]->weapon == "axe")
            || (invaders[skirmish_row][i]->weapon == 
            protectors[i][skirmish_row]->weapon && 
            invaders[skirmish_row][i]->power < 
            protectors[i][skirmish_row]->power) ) {
            
            // protector wins and position is left empty
            output << "Invader killed" << endl; 
            invaders[skirmish_row][i]->power = -1;
        }

        // same weapon and same power --> official draw
        if (invaders[skirmish_row][i]->weapon == 
            protectors[i][skirmish_row]->weapon) {

            if (invaders[skirmish_row][i]->power == 
                protectors[i][skirmish_row]->power) {
                
                output << "Duel ends in draw" << endl;
            } 
        }
    }
    
    // after all skirmishes, invaders have not breached the walls
    // --> protectors are declared the winner
    return false;
}


int main(int argc, char* argv[]) {

    if (argc < 3) {
	   cerr << "Please provide an input and output file" << endl;
	   return -1;
    }

    ifstream input(argv[1]);
    ofstream output(argv[2]);

    //read the input file and initialize the values here->

    int reserve;
    int skirmishes;
    int rows;
    int cols;

    input >> rows; // rows of protectors and columns of invaders
    input >> cols; // columns of protectors and rows of invaders
    input >> reserve; // protectors in reserve
    input >> skirmishes; // num of skirmishes

    //initialize the protectors and invaders here->

    // allocate memory for protector array
    Warrior ***protectors;
    protectors = new Warrior**[rows];

    for (int m=0; m < rows; m++) {

        protectors[m] = new Warrior*[cols];
    }

    // fill in the array with new Protectectors
    for (int i=0; i < rows; i++) {

        for (int j=0; j < cols; j++) {

            protectors[i][j] = new Warrior;

            // even row --> axe
            if (i % 2 == 0) {

                protectors[i][j]->weapon = "axe";

            } else {

                protectors[i][j]->weapon = "sword";
            }

            // set power
            protectors[i][j]->power = i*10+(j+1)*10;
        }
    }


    // allocate memory for invader array
    Warrior ***invaders;
    invaders = new Warrior**[cols];

    for (int n=0; n < cols; n++) {

        invaders[n] = new Warrior*[rows];
    }


    // fill in the array with new invaders
    for (int k=0; k < cols; k++) {

        for (int l=0; l < rows; l++) {

            invaders[k][l] = new Warrior;

            // odd row --> axe
            if (k % 2 == 0) {

                invaders[k][l]->weapon = "sword";

            } else {

                invaders[k][l]->weapon = "axe";
            }

            // set power
            invaders[k][l]->power = k*10+(l+1)*10;
        }
    }


    // iterate through skirmishes
    for (int p=1; p <= skirmishes; p++){
        
        // read in skirmish row
        int skirmish_row;
        input >> skirmish_row;

        bool end = skirmish(protectors, invaders, skirmish_row, rows, 
            cols, reserve, output);

        if (end) {

            output << "Winner: Invaders" << endl;
            return 0;
        }
    }

    output << "Winner: Protectors" << endl;


    // deallocate protectors
    for (int q=0; q < rows; q++) {

        for (int r=0; r < cols; r++){

            // delete pointers inside the array
            delete protectors[q][r];
        }

        // delete inner array
        delete [] protectors[q];
    }

    // delete outer array
    delete [] protectors;



    // deallocate invaders
    for (int s=0; s < cols; s++) {

        for (int t=0; t < rows; t++) {

            // delete pointers inside the array
            delete invaders[s][t];
        }

        // delete inner array
        delete [] invaders[s];
    }

    // delete outer array
    delete [] invaders; 


    return 0;
}
