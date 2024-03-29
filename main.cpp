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
      cout << setprecision(2) << fixed;
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
      cout << endl;
    }
    
    void fillStatesTable() {
      int auxNAtt = nAtt-1;
      double nextStateProb;
      int diagSize, remainA;

      statesTable[nDef][auxNAtt] = 1;

      for(int d=nDef; d>=0; d--) {
        for(int a=auxNAtt; a>=0; a--) {
          DicesProbability round(d, a);
          if(a>3 && d>=3){
            diagSize = 4;
            remainA = 0;
          }
          else if(a>d) {
            diagSize =d+1;
            remainA = 3 - d;
          } else {
            diagSize = a+1;
          }
          for(int i=0; i<diagSize && a>0 && d>0 && statesTable[d][a]!=0 ; i++) {
            int nextA, nextD;
            nextA = a-(diagSize-1)+i;
            nextD = d-i;
            if(a <= 3)
              remainA = nextA;
            nextStateProb = statesTable[d][a] * round.probForRemainingAttackers(remainA);
            statesTable[nextD][nextA]+=nextStateProb;
            remainA++;
          }
      }
    }
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
      // printStatesTable();
      return probSum;
    }
    double hopeNumSoldiers() {
      double hope = 0;
      for(int i=0; i<nAtt; i++)
        hope+= (i+1)*statesTable[0][i];
      return hope;
    }
};



int main() {

  int nAtt, nDef;

  cout << "Insira Nº ataque Nº defesa: ";
  cin >> nAtt >> nDef;



  Battle bt(nAtt,nDef);
  cout << setprecision(3) << fixed;
  cout << "\nBatalha " << nAtt << " vs " << nDef << "\nProbabilidade de vitória: " << bt.probAttackWin() << endl;
  cout << setprecision(1) << fixed;
  cout << "Número de soldados restantes: " << bt.hopeNumSoldiers() << endl;

  return 0;
}