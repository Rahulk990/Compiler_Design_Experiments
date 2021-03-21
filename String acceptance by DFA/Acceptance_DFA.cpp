#include <bits/stdc++.h>
using namespace std;

class DFA
{
    vector<vector<int>> dfa; // Stores the DFA
    string input;            // Stores the input string
    int current_state = 0;   // Stores the current state of DFA

public:
    DFA()
    {
        // Initialization of DFA
        dfa = {
            {0, 1, 2},
            {1, 0, 3},
            {2, 3, 0},
            {3, 2, 1}};
    }

    void input_string()
    {
        /*
            Takes the input string which have to be checked against the DFA
        */

        cout << "Enter String: ";
        cin >> input;
    }

    void parse_string()
    {
        /*
            Takes the input character by character and simulates the transitions in DFA
        */

        current_state = 0;
        for (char c : input)
        {
            if (c == '0')
                current_state = dfa[current_state][1];
            else
                current_state = dfa[current_state][2];
        }
    }

    void produce_output()
    {
        /*
            Produces the output based on the current state of the DFA
            If the string is Rejected, it also tells why it is rejected
        */

        if (current_state == 0)
        {
            cout << "String Accepted: ";
            cout << "String has Even number of Zeros and Ones" << endl;
        }
        else if (current_state == 1)
        {
            cout << "String Rejected: ";
            cout << "String has Even number of Ones, but Odd number of Zeros" << endl;
        }
        else if (current_state == 2)
        {
            cout << "String Rejected: ";
            cout << "String has Even number of Zeros, but Odd number of Ones" << endl;
        }
        else
        {
            cout << "String Rejected: ";
            cout << "String has Odd number of Zeros and Ones" << endl;
        }
    }
};

int main()
{
    DFA dfa;
    dfa.input_string();
    dfa.parse_string();
    dfa.produce_output();

    return 0;
}
