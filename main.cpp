#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>


using namespace std;


class DicesProbability {
  private:
    int nAtt, nDef;
    vector<vector<int>> attackPlays;
    vector<vector<int>> defensePlays;
    vector<vector<int>> threeDicePlays;
    vector<vector<int>> twoDicePlays;
    vector<vector<int>> oneDicePlays;

    void fillDicesPlaysVectors() {
      threeDicePlays = vector<vector<int>> (216, vector<int> (3,0));
      twoDicePlays = vector<vector<int>> (36, vector<int> (3,0));
      oneDicePlays = vector<vector<int>> (6, vector<int> (3,0));
      for(int i=0; i<216;i++) {
        threeDicePlays[i][2] = i%6 + 1;
        threeDicePlays[i][1] = (i/6)%6 + 1;
        threeDicePlays[i][0] = (i/36)%6 + 1;
        sort(threeDicePlays[i].begin(), threeDicePlays[i].end());
      }
      for(int i=0; i<36;i++) {
        twoDicePlays[i][2] = i%6 + 1;
        twoDicePlays[i][1] = (i/6)%6 + 1;
        sort(twoDicePlays[i].begin(), twoDicePlays[i].end());
      }
      for(int i=0; i<6;i++) {
        oneDicePlays[i][2] = i%6 + 1;
      }
    }
  public:
    DicesProbability(int d, int a) {
      nAtt = a;
      nDef = d;

    fillDicesPlaysVectors();

    // Select the dice quantity for the attacker
    if(a>=3)
      attackPlays = threeDicePlays;
    else if(a==2)
      attackPlays = twoDicePlays;
    else if(a==1)
      attackPlays = oneDicePlays;

    // Select the dice quantity for the defender
    if(d>=3)
      defensePlays = threeDicePlays;
    else if(d==2)
      defensePlays = twoDicePlays;
    else if(d==1)
      defensePlays = oneDicePlays;
    }

    double probForRemainingAttackers(int remainA) {
      int count = 0;
      int nValidRounds = 0;
      int nDiceVictory;
      
      for(int i=0; i<defensePlays.size(); i++) {
        for(int j=0; j<attackPlays.size(); j++) {
          nDiceVictory = nAtt >= 3 ? 3 : nAtt;
          if(defensePlays[i][0] >= attackPlays[j][0] && defensePlays[i][0]!=0 && attackPlays[j][0]!=0)
            nDiceVictory-=1;
          if((defensePlays[i][1] >= attackPlays[j][1]) && defensePlays[i][1]!=0 && attackPlays[j][1]!=0)
            nDiceVictory-=1;
          if((defensePlays[i][2] >= attackPlays[j][2]) && defensePlays[i][2]!=0 && attackPlays[j][2]!=0)
            nDiceVictory-=1;

          if(nDiceVictory == remainA)
            nValidRounds++;
      
          count++;
        }
      }
      double prob;

      if(count != 0)
        prob = double(nValidRounds)/count;
      else prob = 0;

      return prob;
    }



};

class Battle {
  private:
    int nAtt, nDef;
    vector<vector<double>> statesTable;

    

    void printStatesTable() {
      cout << setprecision(4) << fixed;
        cout << "D\\A    ";
      for(int j=nAtt-1; j>=0; j--) {
        cout << j+1 << "        ";
      }
      cout << "\n" << endl;
      for(int i=nDef; i>=0; i--) {
        cout << i << " | ";
        for(int j=nAtt-1; j>=0; j--) {
          cout << statesTable[i][j] << "   ";
        }
        cout << endl;
      }
      cout << "\n\n" << endl;
    }

    void incrementProbState(int d, int a, double stateProb) {
      double prob;
      int diagSize, remainAtt, remainA;
      DicesProbability round(d, a);
      
      if(a>3 && d>=3){
        diagSize = 4;
        remainA = 0;
      }
      else if(a > d) {
        diagSize = d+1;
        remainA = 3 - d;
      } else 
        diagSize = a+1;

      for(int i=0; i<diagSize && a>0 && d>0; i++) {
        int nextA, nextD;
        nextA = a-(diagSize-1)+i;
        nextD = d-i;
        if(nextA <= 3)
          remainA = nextA;
        prob = stateProb * round.probForRemainingAttackers(remainA);
        incrementProbState(nextD, nextA, prob);
        remainA++;
      }
      statesTable[d][a] += stateProb;
    }
    
    void fillStatesTable() {
      int auxNAtt = nAtt-1;
      incrementProbState(nDef,auxNAtt,1);
    }
  public:
    Battle(int a, int d) {
      nAtt = a;
      nDef = d;
      statesTable = vector<vector<double>> (d+1, vector<double> (a,0));
    }
    double probAttackWin() {
      fillStatesTable();
      double probSum = 0;
      for(int i=0; i<nAtt; i++) {
        probSum+= statesTable[0][i];
      }
      printStatesTable();
      return probSum;
    }
    double hopeNumSoldiers() {
      return 0;
    }
    double probNumWinnerSoldiers() {
      return 0;
    }
};



int main() {

  Battle bt(4,2);

  cout << bt.probAttackWin() << endl;

  return 0;
}