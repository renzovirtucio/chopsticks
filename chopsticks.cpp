#include "socketstream/socketstream.hh"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
using namespace swoope;

// ------------------------------ LAB 12B -------------------------------
class Hand {
public:
    int chopsticks;
    string type = "Hand";
    bool isAlive = true;
    int upperbound;
    Hand() {
        this->chopsticks = 1;
    }

    void live(string type) {
        if (type == "Human") {
            this->isAlive = 0 <= chopsticks && chopsticks < 5;
            
        }
        if (type == "Alien") {
            this->isAlive =  0 <= chopsticks && chopsticks < 3;
            
        }
        if (type == "Zombie") {
            this->isAlive =  0 < chopsticks && chopsticks < 4;
        }        
        return;
    }
};

class Foot: public Hand {
public:
    Foot() {
        this->type = "Foot";
    }
    void live(string type) {
        if (type == "Human") {
            this->isAlive = 0 < chopsticks && chopsticks < 5;
        }
        if (type == "Alien") {
            this->isAlive = 0 < chopsticks && chopsticks < 2;
        }
        if (type == "Doggo") {
            this->isAlive = 0 < chopsticks && chopsticks < 4;
        }        
        return;
    }
};
//-----------------------------------------------------------------------------
class Player {
public:
    bool status = true;
    int playerNumber;
    char initial;
    bool skip = false;
    bool firstHand;
    string playerType;
    vector<Hand> handVector;
    vector<Foot> footVector;
    
    void isAlive() {
        int numHands = this->handVector.size();
        for(int i = 0; i < numHands; i++) {
            this->handVector[i].live(this->playerType);
            if (this->handVector[i].isAlive == true) {
                this->status = true;
                return;
            }
        }
        int numFeet= this->footVector.size();
        for(int i = 0; i < numFeet; i++) {
            this->footVector[i].live(this->playerType);
            if (this->footVector[i].isAlive == true) {
                this->status = true;
                return;
            }
        }
    
        this->status = false;
        return;
    }
};

class Human: public Player {
public:
    Human(int n) {
        this->initial = 'h';
        this->playerNumber = n;
        this->playerType = "Human";
        for(int i = 0; i < 2; i++) {
            Hand current;
            current.upperbound = 5;
            this->handVector.push_back(current);
        }
        for(int i = 0; i < 2; i++) {
            Foot current;
            current.upperbound = 5;
            this->footVector.push_back(current);
        }
    }
};

class Alien: public Player {
public:
    Alien(int n) {
        this->initial = 'a';
        this->playerNumber = n;
        this->playerType = "Alien";
        for(int i = 0; i < 4; i++) {
            Hand current;
            current.upperbound = 3;
            this->handVector.push_back(current);
        }
        for(int i = 0; i < 2; i++) {
            Foot current;
            current.upperbound = 2;
            this->footVector.push_back(current);
        }   
    }
};

class Zombie: public Player {
public:
    Zombie(int n) {
        this->firstHand = true;
        this->initial = 'z';
        this->playerNumber = n;
        this->playerType = "Zombie";
        for(int i = 0; i < 1; i++) {
            Hand current;
            current.upperbound = 4;
            this->handVector.push_back(current);
        }
    }
};

class Doggo: public Player {
public:
    Doggo(int n) {
        this->initial = 'd';
        this->playerNumber = n;
        this->playerType = "Doggo";
        for(int i = 0; i < 4; i++) {
            Foot current;
            current.upperbound = 4;
            this->footVector.push_back(current);
        }
    }
};
//-----------------------------------------------------------------------------
class Team {
public:
    vector<Player> playerList;
    int teamNumber;

    Team(int n) {
        this->teamNumber = n;
    }

    void addToTeam(Player p) {
        this->playerList.push_back(p);
    }

    bool isAlive() {
        int numPlayers = this->playerList.size();
        int count = 0;
        for (int i = 0; i < numPlayers; i++) {
            this->playerList[i].isAlive();
            if (this->playerList[i].status == true) {
                count++;
            }
        }
        if (count > 0) {
            return true;
        }
        
        return false;
    }
};
//-----------------------------------------------------------------------------
bool add(Hand &side, int to_add) {
    if (side.type == "Hand") {
        if (side.chopsticks == side.upperbound) {
            return false;
        }

        if (side.chopsticks + to_add == side.upperbound) {
            side.chopsticks = side.upperbound;
            return true;
        } else {
            side.chopsticks = (side.chopsticks + to_add) % side.upperbound;
            if (side.chopsticks == 0) {
                side.chopsticks = side.upperbound;
            }
        } 
    } else {
        if (side.chopsticks == side.upperbound) {
            return false;
        }
    
        if (side.chopsticks + to_add >= side.upperbound) {
            side.chopsticks = side.upperbound;
            return true;
        } else {
            side.chopsticks = side.chopsticks + to_add;
        } 
        return false;
    }

    return false;
}

bool tap(Player &p1, Player &p2, string A, string T) {
    char arr[4] = {'A', 'B', 'C', 'D'};
    int indexA = 0, indexT = 0, toAdd = 0;
    
    for (int i = 0; i < 4; i++) {
        if (A[1] == arr[i]) {
            indexA = i;
            break;
        }
        else if (i == 3) {
            return true;
        }
    }

    if (A[0] == 'H') {
        if (p1.playerType == "Doggo") {
            return true;
        }
        
        if (p1.handVector[indexA].isAlive) {
            toAdd = p1.handVector[indexA].chopsticks;
   
        } else{ 
            return true;
        }
    } else {
        if (p1.playerType == "Zombie") {
            return true;
        }
        
        if (p1.footVector[indexA].isAlive) {
            toAdd = p1.footVector[indexA].chopsticks;
        
        } else {
            return true;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (T[1] == arr[i]) {
            indexT = i;
            break;
        }
        else if (i == 3) {
            return true;
        }
        
    }

    if (T[0] == 'H') {
        if (p2.playerType == "Doggo") {
            return true;
        }
        
        if (p2.handVector[indexT].isAlive == false) {
            return true;
        }
        
        if(add(p2.handVector[indexT], toAdd) && p2.playerType == "Zombie" && p2.firstHand) {
            Hand newHand;
            newHand.upperbound = 4;
            p2.handVector.push_back(newHand);
            p2.firstHand = false;
        }    
    } else {
        if (p2.playerType == "Zombie") {
            return true;
        }
        
        if (p2.footVector[indexT].isAlive == false) {
            return true;
        }
        
        if (p2.skip == false) {
            p2.skip = add(p2.footVector[indexT], toAdd);
        } else {
            add(p2.footVector[indexT], toAdd);
        }
        if (p2.playerType == "Alien") {
            p2.skip = false;
        }
    }

    return false;
}

void distribute(Player &p, vector<int> chopstickValues, string pType) {
    int numHands = p.handVector.size();
    for (int i = 0; i < numHands; i++) {
        if (chopstickValues[i] != -1) {
            p.handVector[i].chopsticks = chopstickValues[i];
        }
    }
    return;
}

int checkAllAlive(vector<Team> &sample) {
    int size = sample.size(), numAlive = 0;
    for (int i = 0; i < size; i++) {
        if(sample[i].isAlive()) {
            numAlive++;
        }
    }
    return numAlive;
}

void printStatus(vector<Team> &a, int numberOfPlayers, socketstream clients[]) {
    int size = a.size();
    string willSkipNext = "";
    for (int i = 0; i < size; i++) {
        //cout << "Team " << i+1 << ": ";
        string status = "Team " + to_string(i+1) + ": ";
        int numPlayers = a[i].playerList.size();
        for (int j = 0; j < numPlayers; j++) {
            //cout << "P" << a[i].playerList[j].playerNumber << a[i].playerList[j].initial << " (";
            string initial = "";
            initial += a[i].playerList[j].initial;
            status = status + "P" + to_string(a[i].playerList[j].playerNumber) + initial + " (";
            if (j < numPlayers - 1) {
                int numHands = a[i].playerList[j].handVector.size();
                for (int k = 0; k < numHands; k++) {
                    if (a[i].playerList[j].handVector[k].chopsticks == a[i].playerList[j].handVector[k].upperbound) {
                      //  cout << "X";
                        status = status + "X";
                    } else {
                    //    cout << a[i].playerList[j].handVector[k].chopsticks;
                        status = status + to_string(a[i].playerList[j].handVector[k].chopsticks);
                    }
                }
                //cout << ":";
                status = status + ":";
                int numFeet = a[i].playerList[j].footVector.size();
                for (int k = 0; k < numFeet; k++) {
                    if(a[i].playerList[j].footVector[k].chopsticks == a[i].playerList[j].footVector[k].upperbound) {
                    //    cout << "X";
                        status = status + "X";
                    } else {
                    //    cout << a[i].playerList[j].footVector[k].chopsticks;
                        status = status + to_string(a[i].playerList[j].footVector[k].chopsticks);
                    }
                }
                willSkipNext = a[i].playerList[j].skip ? "(Y)" : "(N)";
                //cout << ") - Skipping next turn: " << willSkipNext << " | ";
                status = status + ") - Skipping next turn: " + willSkipNext + " | ";

            } else {
                int numHands = a[i].playerList[j].handVector.size();
                for (int k = 0; k < numHands; k++) {
                    if (a[i].playerList[j].handVector[k].chopsticks == a[i].playerList[j].handVector[k].upperbound) {
                        //cout << "X";
                        status = status + "X";
                    } else {
                        //cout << a[i].playerList[j].handVector[k].chopsticks;
                        status = status + to_string(a[i].playerList[j].handVector[k].chopsticks);
                    }
                }
                //cout << ":";
                status = status + ":";
                int numFeet = a[i].playerList[j].footVector.size();
                for (int k = 0; k < numFeet; k++) {
                    if(a[i].playerList[j].footVector[k].chopsticks == a[i].playerList[j].footVector[k].upperbound) {
                        //cout << "X";
                        status = status + "X";
                    } else {
                        //cout << a[i].playerList[j].footVector[k].chopsticks;
                        status = status + to_string(a[i].playerList[j].footVector[k].chopsticks);
                    }
                }
                willSkipNext = a[i].playerList[j].skip ? "(Y)" : "(N)";
                //cout << ") - Skipping next turn: " << willSkipNext << "\n";
                status = status + ") - Skipping next turn: " + willSkipNext;
                cout << status << "\n";

                for (int x = 1; x < numberOfPlayers; x++) {
                    clients[x] << status << endl;
                }
            }   
        }
    }
    cout << "\n";
}

// ----------------------------- END -----------------------------------------------

// ------------------- START OF SOCKET PROGRAMMING ---------------------------------

bool inArr(string *arr, string str) {
    for (int i = 0; i < 8; i++) {
        if (arr[i] == str) {
            return true;
        }
    }

    return false;
}

bool isAnInt(string port) {
    string ints = "0123456789";

    for (auto i = 0; i < port.length(); i++) {
        if (ints.find(port[i]) == -1) {
            return false;
        }
    }

    return true;
}

bool inVect(vector<int> vect, int num) {
    int size = vect.size();
    for (int i = 0; i < size; i++) {
        if (vect[i] == num) {
            return true;
        }
    }

    return false;
}

int spaceCounter(string str) {
    int size = str.length(), count = 0;
    for (int i = 0; i < size; i++) {
        if (str[i] == ' ') {
            count++;
        }
    }

    return count;
}

void runServer(string port) {    
    // SERVER SET-UP #1
    // If P is outside the range 1024-65535 or is not an integer, the program exits with
    // an error message stating that the server port must be within the said range
    int idx;
    vector<int> disthandsValues;
    
    if (isAnInt(port) == false) {
        cout << "Port must be an integer within the range 1024 to 65535. Please try again.\n";
        return;
    }

    int intPort = stoi(port);
    if (intPort < 1024 || intPort > 65535) {
        cout << "Port must be an integer within the range 1024 to 65535. Please try again.\n";
        return;
    }

    cout << "You are the SERVER and your port number is: " << intPort << "\n\n";

    // SERVER SET-UP #2
    // The program asks the user for an integer N (number of players) between 2 to 6
    // (inclusive); if N not valid (e.g., string, outside range, etc.), the program must ask
    // again until a valid N is given
    string N;
    int tempo;
    cout << "Please input the number of players: ";
    getline(cin, N);

    while (true) {
        if (N == "2" || N == "3" || N == "4" || N == "5" || N == "6") {
            tempo = stoi(N);
            if (tempo >= 2 && tempo <= 6) {
                break;
            }
        }
        cout << "Number of players must be an integer between 2 to 6. Please try again.\n> ";
        getline(cin, N);
    }

    int numPlayers = tempo;

    // SERVER SET-UP #3
    // Otherwise, the program is now considered as the server ; it is assigned as Player
    // 1 and listens on port P for N − 1 more players
    socketstream clients[numPlayers];
    socketstream server;
    server.open(port, numPlayers);

    cout << "Server is open! Now waiting for players...\n\n";

    for (int i = 1; i < numPlayers; i++) {
        server.accept(clients[i]);
        clients[i] << i << endl;
    }

    for (int i = 1; i < numPlayers; i++) {
        clients[i] << numPlayers << endl;
    }

    // SERVER SET-UP #4
    // After the other N −1 players have connected, the actual game begins with a ready
    // message displayed on the server screen
    cout << "All players connected!\n\n";

    cout << "=========================================\n";
    cout << "======== WELCOME TO CHOPSTICKS! =========\n";
    cout << "=========== You are Player 1! ===========\n";
    cout << "=========================================\n\n";

    // PLAYER TYPE PHASE
    vector<string> playerTypes;
    cout << "Please choose your player type: \n";
    cout << "- human\n";
    cout << "- alien\n";
    cout << "- zombie\n";
    cout << "- doggo\n";
    cout << "\nYour choice: > ";

    string playerType;
    getline(cin, playerType);

    // You are free to choose how the player is asked (e.g., typing the actual player type,
    // providing a menu with integer choices, etc.); however, invalid inputs must not crash the
    // program and instead must be properly handled by asking until a valid player type is
    // selected.
    while (playerType != "human" && playerType != "alien" && playerType != "zombie" && playerType != "doggo") {
        cout << "Invalid player type. Please try again.\n";
        cout << "Your choice: > ";
        getline(cin, playerType);
    }

    playerTypes.push_back(playerType);

    cout << "\nYour player type is <" << playerType << ">\n";

    for (int i = 1; i < numPlayers; i++) {
        getline(clients[i], playerType);
        playerTypes.push_back(playerType);
    }  

    // After all player type preferences have been registered, the game continues by asking
    // all players for an integer from 1 to N that corresponds to the team number they intend
    // to belong to. If a player inputs anything that is not an integer from 1 to N (i.e., an
    // invalid team number), the player must be asked again.
    vector<int> teamNumbers;
    string teamNum;
    cout << "Which team would you like to belong to?\n";
    cout << "> ";
    getline(cin, teamNum);
    
    while (true) {
        if (isAnInt(teamNum) == true) {
            tempo = stoi(teamNum);
            if (tempo >= 1 && tempo <= numPlayers) {
                break;
            }
        }
        cout << "Invalid input. Please enter an integer between 1 and " << numPlayers << "\n> ";
        getline(cin, teamNum);
    }

    int teamNumber, myTeamNumber = tempo;
    teamNumbers.push_back(myTeamNumber);
    
    for (int i = 1; i < numPlayers; i++) {
        clients[i] >> teamNumber;
        clients[i].ignore();
        teamNumbers.push_back(teamNumber);
    }

    vector<int> sorted;

    for (int i = 0; i < numPlayers; i++) {
        if (not inVect(sorted, teamNumbers[i])) {
            sorted.push_back(teamNumbers[i]);
        }
    }

    sort(sorted.begin(), sorted.end());

    bool okTeams = true;
    int size = sorted.size();

    if (size == 1 || inVect(sorted, 1) == false) {
        okTeams = false;
    }

    for (int i = 0; i < size - 1; i++) {
        if (sorted[i] != sorted[i+1] - 1) {
            okTeams = false;
            break;
        }
    }

    for (int i = 1; i < numPlayers; i++) {
        clients[i] << okTeams << endl;
    }

    while (okTeams == false) {
        int counter = 0;
        teamNumbers.clear();
        sorted.clear();
        cout << "Invalid groupings. Please enter a new team number.\n> ";
        getline(cin, teamNum);
    
        while (true) {
            if (isAnInt(teamNum) == true) {
                tempo = stoi(teamNum);
                if (1 <= tempo && tempo <= numPlayers) {
                    break;
                }
            }
            cout << "Please enter an integer between 1 and " << numPlayers << "\n> ";
            getline(cin, teamNum);
        }

        int teamNumber; myTeamNumber = tempo;
        teamNumbers.push_back(myTeamNumber);
        
        for (int i = 1; i < numPlayers; i++) {
            clients[i] >> teamNumber;
            clients[i].ignore();
            teamNumbers.push_back(teamNumber);
        }

        for (int i = 0; i < numPlayers; i++) {
            if (not inVect(sorted, teamNumbers[i])) {
                sorted.push_back(teamNumbers[i]);
            }
        }

        /*cout << "SORTED\n";
        for (auto elem: sorted) {
            cout << elem << " ";
        }
        cout << "\n";*/

        size = sorted.size();
        sort(sorted.begin(), sorted.end());
        
        for (int i = 0; i < size - 1; i++) {
            if (sorted[i] != sorted[i+1] - 1) {
                okTeams = false;
                break;
            }
            else {
                counter++;
            }
        }

        if (sorted[size-2] == sorted[size-1] - 1) {
            counter++;
        }

        if (counter == size && size != 1) {
            okTeams = true;
        }

        for (int j = 1; j < numPlayers; j++) {
            clients[j] << okTeams << endl;
        }
    }

    for (int i = 1; i < numPlayers; i++) {
        clients[i] << teamNumbers[i] << endl;
    }

    cout << "\nYou are part of Team " << myTeamNumber << "!\n";

    cout << "======== TEAMS ========\n";
    for (int i = 0; i < numPlayers; i++) {
        string to_print = "Player " + to_string(i + 1) + " (" + playerTypes[i] + "): Team " + to_string(teamNumbers[i]);
        cout << to_print << "\n";
        for (int j = 1; j < numPlayers; j++) {
            clients[j] << to_print << endl;
        }
    }
    cout << "\n";

    int numTeams = sorted.size();
    vector<Team> participants;
    for (int i = 0; i < numTeams; i++) {
        Team currentTeam = Team(i+1);
        participants.push_back(currentTeam);
    }

    for (int i = 0; i < numPlayers; i++) {
        string pType = playerTypes[i];
        int index = teamNumbers[i] - 1;
        
        if (pType == "human") {
            Human current = Human(i+1);
            participants[index].addToTeam(current);
        } 
        else if (pType == "alien") {
            Alien current = Alien(i+1);
            participants[index].addToTeam(current);
        } 
        else if (pType == "doggo") {
            Doggo current = Doggo(i+1);
            participants[index].addToTeam(current);
        }
        else if (pType == "zombie") {
            Zombie current = Zombie(i+1);
            participants[index].addToTeam(current);
        }
    }

    cout << "Number of Teams: " << numTeams << "\n";

    bool gameOngoing = true;

    int numPlayingTeams = 0, turn = 0;
    vector<int> playerTurnVector;
    for (int i = 0; i < numTeams; i++) {
        playerTurnVector.push_back(0);
    }
    
    for (int i = 1; i < numPlayers; i++) {
        clients[i] << numTeams << endl;
    }
    
    int myPlayerIndex = 0;
    printStatus(participants, numPlayers, clients);
    while (gameOngoing) {
        int teamInTurnIndex = turn % numTeams;
        turn++;

        int playerInTurn = playerTurnVector[teamInTurnIndex];
        playerTurnVector[teamInTurnIndex] = (playerTurnVector[teamInTurnIndex] + 1) % participants[teamInTurnIndex].playerList.size();
       
        int skipCount = 0;
        bool skipWholeTeam = false;
        bool playerAlive = participants[teamInTurnIndex].playerList[playerInTurn].status;

        if (not playerAlive) {
            participants[teamInTurnIndex].playerList[playerInTurn].skip = true;
        }

        bool playerSkip = participants[teamInTurnIndex].playerList[playerInTurn].skip;

        for (int i = 1; i < numPlayers; i++) {
            clients[i] << playerSkip << endl;
        }

        while (playerSkip) {
            if (playerSkip) {
                string playerToSkip = "Player " + to_string(participants[teamInTurnIndex].playerList[playerInTurn].playerNumber) + " will skip.";
                cout << playerToSkip << "\n";
                for (int i = 1; i < numPlayers; i++) {
                    clients[i] << playerToSkip << endl;
                }
            }

            //cout << participants[teamInTurnIndex].playerList[playerInTurn].playerNumber << ": " << participants[teamInTurnIndex].playerList[playerInTurn].status << "\n";

            if (playerAlive) {
                participants[teamInTurnIndex].playerList[playerInTurn].skip = false;
            }
            /*
            else {
                participants[teamInTurnIndex].playerList[playerInTurn].skip = true;
            }*/
            
            skipCount++;
            int teamSize = participants[teamInTurnIndex].playerList.size();
            if (skipCount == teamSize) { 
                skipWholeTeam = true; 
                break; 
            } 
            
            playerInTurn = playerTurnVector[teamInTurnIndex]; 
            playerTurnVector[teamInTurnIndex] = (playerTurnVector[teamInTurnIndex] + 1) % participants[teamInTurnIndex].playerList.size(); //next player in team    
            playerSkip = participants[teamInTurnIndex].playerList[playerInTurn].skip;

            for (int i = 1; i < numPlayers; i++) {
                clients[i] << playerSkip << endl;
            }
        }

        for (int i = 1; i < numPlayers; i++) {
            clients[i] << skipWholeTeam << endl;
        }

        if (skipWholeTeam) { 
            string skipTeam = "Team " + to_string(teamInTurnIndex+1) + " will skip.";
            cout << skipTeam << "\n";
            for (int i = 1; i < numPlayers; i++) {
                clients[i] << skipTeam << endl;
            }
            continue; 
        }

        int currentPlayerIndex = participants[teamInTurnIndex].playerList[playerInTurn].playerNumber - 1;
        cout << "Current player in turn: Player " << currentPlayerIndex + 1 << "\n";
        cout << "Current team in turn: Team " << teamInTurnIndex + 1 << endl;

        int nextTeamInTurnIndex = turn % numTeams, nextPlayerInTurn = playerTurnVector[teamInTurnIndex];
        
        for (int i = 1; i < numPlayers; i++) {
            clients[i] << currentPlayerIndex << endl;
        }

        for (int i = 1; i < numPlayers; i++) {
            clients[i] << teamInTurnIndex + 1 << endl;
        }

        cout << "\nNext Players Per Team\n";
        for (int i = 0; i < numTeams; i++) {
            string current;
            if (participants[i].isAlive() == false) {
                current = "Team " + to_string(i+1) + " is out of the game!";
            } else {
                int index = playerTurnVector[i];
                while (true) {
                    if (participants[i].playerList[index].status == true && participants[i].playerList[index].skip == false) {
                        current = "Team " + to_string(i+1) + ": Player " + to_string(participants[i].playerList[index].playerNumber);
                        break;
                    } else {
                        index = (index + 1) % participants[i].playerList.size();
                    } 
                    bool allPlayerSkips = true;
                    for (int j = 0; j < participants[i].playerList.size(); j++) {
                        if (participants[i].playerList[j].skip == false && participants[i].playerList[j].status == true) {
                            allPlayerSkips = false  ;
                            break;
                        }
                    }

                    if (allPlayerSkips) {
                        index = playerTurnVector[i];
                        current = "Team " + to_string(i+1) + ": Player " + to_string(participants[i].playerList[index].playerNumber);
                        break;
                    }
                }
            }
            cout << current << "\n"; 
            for (int j = 1; j < numPlayers; j++) {
                clients[j] << current << endl;
            }
        }
        cout << "\n";
        
        playerType = playerTypes[currentPlayerIndex];
        int numPlayersInTeam;

        string options[] = {"HA", "HB", "HC", "HD", "FA", "FB", "FC", "FD"};
        string act, action, attackerHand, targetHand, A, B, C, D, tPlayer, tHand;
        int index, targetPlayer, handA = 0, handB = 0, handC = 0, handD = 0;
        bool invalid;

        if (myPlayerIndex == currentPlayerIndex) {
            int numActions = 1;
            if (playerType == "zombie") {
                numActions = 2;
            }

            
            for (int i = 1; i < numPlayers; i++) {
                clients[i] << numActions << endl;
            }

            while (numActions > 0) {
                int count = 0;
                invalid = true;
                while (invalid) {
                    cout << "Actions: " << numActions << "\n";

                    if (count == 0) {
                        cout << "Input your action\n> ";
                    } else {
                        cout << "Invalid action. Please try again.\n> ";
                    }
                    count++;
                    
                    disthandsValues.clear();
                    getline(cin, act);

                    action = act.substr(0, 3);

                    if (action == "tap" && act.length() == 11) {
                        if (spaceCounter(act) == 3) {
                            index = act.find(" ") + 1;
                            attackerHand = act.substr(index, 2);
                            if (inArr(options, attackerHand)) {
                                tPlayer = act.substr(index + 3, 1);
                                if (isAnInt(tPlayer)) {
                                    tempo = stoi(tPlayer);
                                    if (tempo >= 1 && tempo <= numPlayers && tempo != myPlayerIndex + 1) {
                                        targetPlayer = tempo;
                                        tHand = act.substr(index + 5, 2);
                                        if (inArr(options, tHand)) {
                                            targetHand = tHand;
                                            act = action + " " + attackerHand + " " + to_string(targetPlayer) + " " + targetHand;
                                            invalid = false;
                                        }
                                    } 
                                }
                            }
                        }
                    }

                    else if (action == "dis" && act.length() <= 17) {
                        int totalHands;
                        action = act.substr(0, 9);
                        index = act.find(" ") + 1;
                        //cout << action << "\n";
                        if (action == "disthands") {
                            A = act.substr(index, 1);
                            if (isAnInt(A) && act.length() <= 17) {
                                int a = (participants[teamInTurnIndex].playerList[playerInTurn].handVector.size());
                                int numberHands = 0;
                                for (int i = 0; i < a; i++) {
                                    participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].live(playerType);
                                    if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].isAlive) {
                                        numberHands++;
                                    }
                                }
                                if (spaceCounter(act) == numberHands && playerType != "zombie" && playerType != "doggo" && numberHands > 1) {
                                    handA = stoi(A);
                                    if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[0].upperbound >= handA) {
                                        disthandsValues.push_back(handA);
                                        B = act.substr(index + 2, 1);
                                        if (isAnInt(B)) {
                                            handB = stoi(B);
                                            if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[1].upperbound >= handB) {
                                                disthandsValues.push_back(handB);
                                                if (numberHands >= 3) {
                                                    C = act.substr(index + 4, 1);
                                                    if (isAnInt(C)) {
                                                        handC = stoi(C);
                                                        if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[2].upperbound >= handC) {
                                                            disthandsValues.push_back(handC);
                                                            if (numberHands == 4) {
                                                                D = act.substr(index + 6, 1);
                                                                if (isAnInt(D)) {
                                                                    handD = stoi(D);
                                                                    if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[3].upperbound >= handD) {
                                                                        disthandsValues.push_back(handD);
                                                                        invalid = false;
                                                                        act = action + " " + A + " " + B + " " + C + " " + D; 
                                                                    }
                                                                }
                                                            }
                                                            else if (numberHands == 3) {
                                                                invalid = false;
                                                                act = action + " " + A + " " + B + " " + C;
                                                            }
                                                        }
                                                    }
                                                }
                                                else if (numberHands == 2) {
                                                    invalid = false;
                                                    act = action + " " + A + " " + B;
                                                }
                                            }
                                        }
                                    }
                                }
                                /*cout << "disthandsvalues\n";
                                for (auto elem: disthandsValues) {
                                    cout << elem << " ";
                                }
                                cout << "\n";*/
                            }
                        } 
                        if (not invalid) {
                            int handsTotal = 0, disthandsTotal = handA + handB + handC + handD;
                            int playerHands = participants[teamInTurnIndex].playerList[playerInTurn].handVector.size();
                            for (int i = 0; i < playerHands; i++) {
                                if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].chopsticks != participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].upperbound) {
                                    handsTotal += participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].chopsticks;
                                }
                            }
                            //cout << "handstotal: " << handsTotal << " | disthandstotal: " << disthandsTotal << endl;

                            if (handsTotal != disthandsTotal) {
                                invalid = true;
                            }
                        }
                    }

                    if (action == "tap" && not invalid) {
                        for (int i = 0; i < numTeams; i++) {
                            numPlayersInTeam = participants[i].playerList.size();
                            for (int j = 0; j < numPlayersInTeam; j++) {
                                if (targetPlayer == participants[i].playerList[j].playerNumber && i != teamInTurnIndex) {
                                    invalid = tap(participants[teamInTurnIndex].playerList[playerInTurn], participants[i].playerList[j], attackerHand, targetHand);
                                    if (participants[teamInTurnIndex].playerList[playerInTurn].playerType != "Doggo" && participants[i].playerList[j].playerType == "Doggo" && invalid == false) {
                                        participants[teamInTurnIndex].playerList[playerInTurn].skip = true;
                                    }                               
                                }
                                if (targetPlayer == participants[i].playerList[j].playerNumber && i == teamInTurnIndex) {
                                    invalid = true;
                                }
                                
                            }
                        }
                    }

                    else if (action == "disthands" && not invalid) {
                        vector<int> chopstickVals;
                        int numInts = participants[teamInTurnIndex].playerList[playerInTurn].handVector.size();
                        idx = 0;
                        for (int i = 0; i < numInts; i++) {
                            //participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].live(playerType);
                            if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].chopsticks != participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].upperbound) {
                                chopstickVals.push_back(disthandsValues[idx]);
                                idx++;
                            }
                            else {
                                chopstickVals.push_back(-1);
                            }
                        }
                        distribute(participants[teamInTurnIndex].playerList[playerInTurn], chopstickVals, playerType);
                    }
            
                }
                numActions--;
                
                
                cout << "Your action: " << act << endl;

                for (int i = 1; i < numPlayers; i++) {
                    clients[i] << act << endl;
                }

                int check = checkAllAlive(participants);
                for (int i = 1; i < numPlayers; i++) {
                    clients[i] << check << endl;
                }

                if (check == 1) {
                    gameOngoing = false;
                    break;
                }
            }
        }
        
        else {
            cout << "Waiting for Player " << currentPlayerIndex + 1 << "...\n";
            int clientNumActions;
            clients[currentPlayerIndex] >> clientNumActions;
            clients[currentPlayerIndex].ignore();

            string clientType = playerTypes[currentPlayerIndex];

            for (int i = 1; i < numPlayers; i++) {
                if (i != currentPlayerIndex) {
                    clients[i] << clientNumActions << endl;
                }
            }

            while (clientNumActions > 0) {
                string clientAct;
                bool invalid = true;
                while (invalid) {
                    disthandsValues.clear();
                    getline(clients[currentPlayerIndex], act);

                    action = act.substr(0, 3);

                    if (action == "tap" && act.length() == 11) {
                        if (spaceCounter(act) == 3) {
                            index = act.find(" ") + 1;
                            attackerHand = act.substr(index, 2);
                
                            if (inArr(options, attackerHand)) {
                                tPlayer = act.substr(index + 3, 1);
                    
                                if (isAnInt(tPlayer)) {
                            
                                    tempo = stoi(tPlayer);
                            
                                    if (tempo >= 1 && tempo <= numPlayers && tempo != currentPlayerIndex + 1) {
                                        targetPlayer = tempo;
                                        tHand = act.substr(index + 5, 2);
                                    
                                        if (inArr(options, tHand)) {
                                            targetHand = tHand;
                                            act = action + " " + attackerHand + " " + to_string(targetPlayer) + " " + targetHand;
                                            invalid = false;
                                        
                                        }
                                    } 
                                }
                            }
                        }
                    }

                    else if (action == "dis" && act.length() <= 17) {
                        int totalHands;
                        action = act.substr(0, 9);
                        index = act.find(" ") + 1;
                        if (action == "disthands") {
                            A = act.substr(index, 1);
                            if (isAnInt(A) && act.length() <= 17) {
                                int a = (participants[teamInTurnIndex].playerList[playerInTurn].handVector.size());
                                int numberHands = 0;
                                for (int i = 0; i < a; i++) {
                                    //participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].live(playerTypes[currentPlayerIndex]);
                                    if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].chopsticks != participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].upperbound) {
                                        //cout << "chopsticks: " << participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].chopsticks << "\n";
                                        numberHands++;
                                    }
                                }
                                //cout << "alivehands: " << numberHands << endl;
                                if (spaceCounter(act) == numberHands && playerType != "zombie" && playerType != "doggo" && numberHands > 1) {
                                    handA = stoi(A);
                                    if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[0].upperbound >= handA) {
                                        disthandsValues.push_back(handA);
                                        B = act.substr(index + 2, 1);
                                        if (isAnInt(B)) {
                                            handB = stoi(B);
                                            if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[1].upperbound >= handB) {
                                                disthandsValues.push_back(handB);
                                                if (numberHands >= 3) {
                                                    C = act.substr(index + 4, 1);
                                                    if (isAnInt(C)) {
                                                        handC = stoi(C);
                                                        if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[2].upperbound >= handC) {
                                                            disthandsValues.push_back(handC);
                                                            if (numberHands == 4) {
                                                                D = act.substr(index + 6, 1);
                                                                if (isAnInt(D)) {
                                                                    handD = stoi(D);
                                                                    if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[3].upperbound >= handD) {
                                                                        disthandsValues.push_back(handD);
                                                                        invalid = false;
                                                                        act = action + " " + A + " " + B + " " + C + " " + D; 
                                                                    }
                                                                }
                                                            }
                                                            else if (numberHands == 3) {
                                                                invalid = false;
                                                                act = action + " " + A + " " + B + " " + C;
                                                            }
                                                        }
                                                    }
                                                }
                                                else if (numberHands == 2) {
                                                    invalid = false;
                                                    act = action + " " + A + " " + B;
                                                }
                                            }
                                        }
                                    }
                                }
                                /*cout << "disthandsvalues\n";
                                for (auto elem: disthandsValues) {
                                    cout << elem << " ";
                                }
                                cout << "\n";*/
                            }
                        } 
                        if (not invalid) {
                            int handsTotal = 0, disthandsTotal = handA + handB + handC + handD;
                            int playerHands = participants[teamInTurnIndex].playerList[playerInTurn].handVector.size();
                            for (int i = 0; i < playerHands; i++) {
                                if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].chopsticks != participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].upperbound) {
                                    handsTotal += participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].chopsticks;
                                }
                            }
                            //cout << "handstotal: " << handsTotal << " | disthandstotal: " << disthandsTotal << endl;

                            if (handsTotal != disthandsTotal) {
                                invalid = true;
                            }
                        }
                    }
                    if (action == "tap" && not invalid) {
                        for (int i = 0; i < numTeams; i++) {
                            numPlayersInTeam = participants[i].playerList.size();
                            for (int j = 0; j < numPlayersInTeam; j++) {
                                if (targetPlayer == participants[i].playerList[j].playerNumber && i != teamInTurnIndex) {
                                    invalid = tap(participants[teamInTurnIndex].playerList[playerInTurn], participants[i].playerList[j], attackerHand, targetHand);
                                    if (participants[teamInTurnIndex].playerList[playerInTurn].playerType != "Doggo" && participants[i].playerList[j].playerType == "Doggo" && invalid == false) {
                                        participants[teamInTurnIndex].playerList[playerInTurn].skip = true;
                                    }                             
                                }
                                if (targetPlayer == participants[i].playerList[j].playerNumber && i == teamInTurnIndex) {
                                    invalid = true;
                                }
                                
                            }
                        }
                    }

                    else if (action == "disthands" && not invalid) {
                        vector<int> chopstickVals;
                        int numInts = participants[teamInTurnIndex].playerList[playerInTurn].handVector.size();
                        idx = 0;
                        for (int i = 0; i < numInts; i++) {
                            //participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].live(playerType);
                            if (participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].chopsticks != participants[teamInTurnIndex].playerList[playerInTurn].handVector[i].upperbound) {
                                chopstickVals.push_back(disthandsValues[idx]);
                                idx++;
                            }
                            else {
                                chopstickVals.push_back(-1);
                            }
                        }
                        distribute(participants[teamInTurnIndex].playerList[playerInTurn], chopstickVals, playerType);
                    }
                    
                    clients[currentPlayerIndex] << invalid << endl;
                }

               
                clientNumActions--;
                cout << "Player " << currentPlayerIndex+1 <<  "'s action: " << act << endl;

                for (int i = 1; i < numPlayers; i++) {
                    if (i != currentPlayerIndex) {
                        clients[i] << act << endl;
                    }
                }

                int check = checkAllAlive(participants);
                for (int i = 1; i < numPlayers; i++) {
                    clients[i] << check << endl;
                }

                if (check == 1) {
                    gameOngoing = false;
                    break;
                }
            }
        }
        cout << "\nGame Status:\n";
        printStatus(participants, numPlayers, clients);
    }    
    int winningTeam;
    for (int i = 0; i < numTeams; i++) {
        if (participants[i].isAlive()) {
            winningTeam = i + 1;
            break;
        }
    }
    string winner = "Team " + to_string(winningTeam) + " wins!";
    cout << winner << "\n";
    for (int i = 1; i < numPlayers; i++) {
        clients[i] << winner << endl;
    }

    if (winningTeam == myTeamNumber) {
        cout << "Congratulations!\n";
    }
    else {
        cout << "You lose!\n";
    }
    
    for (int i = 1; i < teamNumbers.size(); i++) {
        if (teamNumbers[i] == winningTeam) {
            clients[i] << "Congratulations!" << endl;
        }
        else {
            clients[i] << "You lose!" << endl;
        }
    }
}

void runClient(string ipAdd, string port) {
    // CLIENT SET-UP #1
    // If P is outside the range 1024-65535 or is not an integer, the program exits with
    // an error message stating that the port must be within the said range
    if (isAnInt(port) == false) {
        cout << "Port must be an integer within the range 1024 to 65535. Please try again.\n";
        return;
    }

    int intPort = stoi(port);
    if (intPort < 1024 || intPort > 65535) {
        cout << "Port must be an integer within the range 1024 to 65535. Please try again.\n";
        return;
    }

    cout << "\nYou are a CLIENT connected to IP address " << ipAdd << " and port " << intPort << "\n\n";

    // CLIENT SET-UP #2
    // Without checking the validity of A as an IP address, the program must connect
    // to the server found at the IP address A listening on port P (i.e., if the IP address
    // is invalid, the program is expected to crash and/or exit)
    socketstream server;
    server.open(ipAdd, port);

    // CLIENT SET-UP #3
    //  The program is now considered as one of the clients; it must output its player
    // number and wait for all other clients to connect to the server
    int myPlayerIndex, currentPlayerInTurnIndex, numPlayers;
    server >> myPlayerIndex;
    server.ignore();

    cout << "Waiting for other players...\n\n";

    server >> numPlayers;
    server.ignore();
    //cout << "Number of players: " << numPlayers << "\n";

    // CLIENT SET-UP #4
    // After all other clients have connnected, the actual game begins with a ready message displayed on the client screen
    cout << "=========================================\n";
    cout << "======== WELCOME TO CHOPSTICKS! =========\n";
    cout << "=========== You are Player ";
    cout << myPlayerIndex + 1 << "! ===========\n";
    cout << "=========================================\n\n";

    // PLAYER TYPE PHASE
    vector<string> playerTypes;
    cout << "Please choose your player type: \n";
    cout << "- human\n";
    cout << "- alien\n";
    cout << "- zombie\n";
    cout << "- doggo\n";
    cout << "\nYour choice: > ";

    string playerType;
    getline(cin, playerType);

    // You are free to choose how the player is asked (e.g., typing the actual player type,
    // providing a menu with integer choices, etc.); however, invalid inputs must not crash the
    // program and instead must be properly handled by asking until a valid player type is
    // selected.
    while (playerType != "human" && playerType != "alien" && playerType != "zombie" && playerType != "doggo") {
        cout << "Invalid player type. Please try again.\n";
        cout << "Your choice: > ";
        getline(cin, playerType);
    }

    server << playerType << endl;

    cout << "\nYour player type is <" << playerType << ">\n";

    // After all player type preferences have been registered, the game continues by asking
    // all players for an integer from 1 to N that corresponds to the team number they intend
    // to belong to. If a player inputs anything that is not an integer from 1 to N (i.e., an
    // invalid team number), the player must be asked again.
    int tempo, teamNumber;
    string teamNum;
    cout << "Which team would you like to belong to?\n";
    cout << "> ";
    cin >> teamNum;
    cin.ignore();

    while (true) {
        if (isAnInt(teamNum) == true) {
            tempo = stoi(teamNum);
            if (tempo >= 1 && tempo <= numPlayers) {
                teamNumber = tempo;
                break;
            }
        }
        cout << "Invalid input. Please enter an integer between 1 and " << numPlayers << "\n> ";
        getline(cin, teamNum);
    }

    server << teamNumber << endl;

    bool okTeams;
    server >> okTeams;
    server.ignore();

    while (okTeams == false) {
        cout << "Invalid groupings. Please enter a new team number.\n> ";
        getline(cin, teamNum);

        while (true) {
            if (isAnInt(teamNum) == true) {
                tempo = stoi(teamNum);
                if (tempo >= 1 && tempo <= numPlayers) {
                    teamNumber = tempo;
                    break;
                }
            }
            cout << "Invalid input. Please enter an integer between 1 and " << numPlayers << "\n> ";
            getline(cin, teamNum);
        }

        server << teamNumber << endl;
        server >> okTeams;
        server.ignore();

        if (okTeams == true) {
            break;
        }
    }

    server >> teamNumber;
    cout << "\nYou are part of Team " << teamNumber << "!\n";
    server.ignore();

    cout << "======== TEAMS ========\n";

    for (int i = 0; i < numPlayers; i++) {
        string to_print;
        getline(server, to_print);
        cout << to_print << "\n";
    }
    cout << "\n";
    
    int numTeams;
    server >> numTeams;
    server.ignore();

    string gameState;
    for (int i = 0; i < numTeams; i++) {
        getline(server, gameState);
        cout << gameState << "\n";
    }

    bool gameOngoing = true;
    while (gameOngoing) {
        bool playerSkip;
        server >> playerSkip;
        server.ignore();

        while (playerSkip) {
            string playerToSkip;
            
            getline(server, playerToSkip);
            cout << playerToSkip << "\n";
            server >> playerSkip;
            server.ignore();
        }

        bool skipWholeTeam;
        server >> skipWholeTeam;
        server.ignore();

        if (skipWholeTeam) {
            string skipTeam;
            getline(server, skipTeam);
            cout << skipTeam << "\n";
        }

        int currentPlayerIndex;
        string nextPlayers;
        server >> currentPlayerIndex;
        server.ignore();

        int currentTeamIndex;
        server >> currentTeamIndex;
        server.ignore();
        
        cout << "Current player in turn: Player " << currentPlayerIndex + 1<< "\n";
        cout << "Current team in turn: Team " << currentTeamIndex << "\n";
        cout << "\nNext Players Per Team\n";
        for (int i = 0; i < numTeams; i++) {
            getline(server, nextPlayers);
            cout << nextPlayers << "\n";
        }
        cout << "\n";


        if (currentPlayerIndex != myPlayerIndex) {
            cout << "Waiting for Player " << currentPlayerIndex + 1 << "...\n";
            int numActions;
            server >> numActions;
            server.ignore();
            
            while (numActions > 0) {
                string action;
                getline(server, action);
                cout << "\nCurrent player's action: " << action << "\n\n";
                numActions--;
                
                int numParticipants;
                server >> numParticipants;
                server.ignore();

                if (numParticipants == 1) {
                    gameOngoing = false;
                    break;
                }
            }   
        }

        else {
            int numActions = 1;
            if (playerType == "zombie")  {
                numActions = 2;
            }
            server << numActions << endl;
            
            while (numActions > 0) {
                int count = 0;
                bool invalid = true;
                string act;
                while (invalid) {
                    if (count == 0) {
                        cout << "Actions: " << numActions << "\nInput your action\n> ";
                    } else {
                        cout << "Invalid action. Please try again.\n> ";
                    }
                    count++;
                    getline(cin, act);
                    server << act << endl;
                    
                    server >> invalid;
                    server.ignore();
                }
                cout << "\nYour action: " << act << "\n\n";

                numActions--;

                int numParticipants;
                server >> numParticipants;
                server.ignore();

                if (numParticipants == 1) {
                    gameOngoing = false;
                    break;
                }
            }
            
        }

        for (int i = 0; i < numTeams; i++) {
            getline(server, gameState);
            cout << gameState << "\n";
        }
        
    }

    string winner;
    getline(server, winner);
    cout << winner << "\n";

    string partOfTeam;
    getline(server, partOfTeam);
    cout << partOfTeam << "\n";
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        string port(argv[1]);
        runServer(port);
    }
    else if (argc == 3) {
        string ipAddress(argv[1]);
        string port(argv[2]);
        runClient(ipAddress, port);
    }
    else {
        cout << "Invalid arguments. Please try again.\n";
    }

    return 0;
}