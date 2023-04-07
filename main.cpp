#include <iostream>
#include <vector>
#include <algorithm>

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
      for(int i=0; i<216;i++) {
        threeDicePlays[i][2] = i%6 + 1;
        threeDicePlays[i][1] = (i/6)%6 + 1;
        threeDicePlays[i][0] = (i/36)%6 + 1;
        sort(threeDicePlays.begin(), threeDicePlays.end());
      }
      for(int i=0; i<36;i++) {
        twoDicePlays[i][2] = i%6 + 1;
        twoDicePlays[i][1] = (i/6)%6 + 1;
        sort(twoDicePlays.begin(), twoDicePlays.end());
      }
      for(int i=0; i<6;i++) {
        oneDicePlays[i][2] = i%6 + 1;
      }
    }
  public:
    DicesProbability(int a, int d) {
      nAtt = a;
      nDef = d;

      fillDicesPlaysVectors();

      // Select the dice quantity for the attacker
      if(a>3)
        attackPlays = vector<vector<int>> (216, vector<int> (3,0));
      else if(a==3)
        attackPlays = vector<vector<int>> (36, vector<int> (3,0));
      else if(a==2)
        attackPlays = vector<vector<int>> (6, vector<int> (3,0));

      // Select the dice quantity for the defender
      if(d>=3)
        defensePlays = vector<vector<int>> (216, vector<int> (3,0));
      else if(d==2)
        defensePlays = vector<vector<int>> (36, vector<int> (3,0));
      else if(d==1)
        defensePlays = vector<vector<int>> (6, vector<int> (3,0));
    }

    double probForRemainingAttackers(int remainA) {
      int count = 0;
      int nValidRounds = 0;
      int nDiceVictory = nAtt > 3 ? 3 : nAtt;
      
      for(int i=0; i<defensePlays.size(); i++) {
        for(int j=0; j<attackPlays.size(); j++) {
          if(defensePlays[i][0] >= attackPlays[j][0])
            nDiceVictory-=1;
          if((defensePlays[i][1] >= attackPlays[j][1]) && defensePlays[i][1]!=0 && attackPlays[j][1]!=0)
            nDiceVictory-=1;
          if((defensePlays[i][2] >= attackPlays[j][2]) && defensePlays[i][2]!=0 && attackPlays[j][2]!=0)
            nDiceVictory-=1;

          if(nDiceVictory == remainA)
            nValidRounds++;
          
          nDiceVictory = nAtt > 3 ? 3 : nAtt;
          count++;
        }
      }
      
      return double(nValidRounds)/count;
    }



};

class Battle {
  private:
    int nAtt, nDef;
    vector<vector<double>> statesTable;

    double probForRemainingAttackers(int d, int a, int remainA) {
      // vector<vector<int>> attackPlays;
      // vector<vector<int>> defensePlays;
      // vector<vector<int>> threeDicePlays(216, vector<int> (3,0));
      // for(int i=0; i<216;i++) {
      //   threeDicePlays[i][2] = i%6 + 1;
      //   threeDicePlays[i][1] = (i/6)%6 + 1;
      //   threeDicePlays[i][0] = (i/36)%6 + 1;
      //   sort(threeDicePlays.begin(), threeDicePlays.end());
      // }
      // vector<vector<int>> twoDicePlays(36, vector<int> (3,0));
      // for(int i=0; i<36;i++) {
      //   twoDicePlays[i][2] = i%6 + 1;
      //   twoDicePlays[i][1] = (i/6)%6 + 1;
      //   sort(twoDicePlays.begin(), twoDicePlays.end());
      // }
      // vector<vector<int>> oneDicePlays(6, vector<int> (3,0));
      // for(int i=0; i<6;i++) {
      //   oneDicePlays[i][2] = i%6 + 1;
      // }

      // // Select the dice quantity for the attacker
      // if(a>3)
      //   attackPlays = vector<vector<int>> (216, vector<int> (3,0));
      // else if(a==3)
      //   attackPlays = vector<vector<int>> (36, vector<int> (3,0));
      // else if(a==2)
      //   attackPlays = vector<vector<int>> (6, vector<int> (3,0));

      // // Select the dice quantity for the defender
      // if(d>=3)
      //   defensePlays = vector<vector<int>> (216, vector<int> (3,0));
      // else if(d==2)
      //   defensePlays = vector<vector<int>> (36, vector<int> (3,0));
      // else if(d==1)
      //   defensePlays = vector<vector<int>> (6, vector<int> (3,0));

      // int count = 0;
      // int nValidRounds = 0;
      // int nDiceVictory = a > 3 ? 3 : a;
      
      // for(int i=0; i<defensePlays.size(); i++) {
      //   for(int j=0; j<attackPlays.size(); j++) {
      //     if(defensePlays[i][0] >= attackPlays[j][0])
      //       nDiceVictory-=1;
      //     if((defensePlays[i][1] >= attackPlays[j][1]) && defensePlays[i][1]!=0 && attackPlays[j][1]!=0)
      //       nDiceVictory-=1;
      //     if((defensePlays[i][2] >= attackPlays[j][2]) && defensePlays[i][2]!=0 && attackPlays[j][2]!=0)
      //       nDiceVictory-=1;

      //     if(nDiceVictory == remainA)
      //       nValidRounds++;
          
      //     nDiceVictory = a > 3 ? 3 : a;
      //     count++;
      //   }
      // }
      
      // return double(nValidRounds)/count;
    }
    void incrementProbState(int d, int a, double stateProb) {
      double prob;
     
      for(int i=0; i<4; i++) {
        if((a-3+i)>0 && (d-i)>=0) {
          prob = stateProb * probForRemainingAttackers(d-i, a-3+i, i);
          incrementProbState(d-i, a-3+i, prob);
        }
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

  Battle bt(14,3);

  cout << bt.probAttackWin() << endl;

  return 0;
}