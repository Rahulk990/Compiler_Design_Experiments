#include <bits/stdc++.h>
using namespace std;

class NFA_to_DFA_Converter
{
    int num_states;  // Number of States in NFA
    int num_symbols; // Number of Symbols

    int initial_state; // Bit-mask to store the Initial States
    int final_states;  // Bit-mask to store the Final States

    vector<vector<int>> NFA; // Stores the NFA
    vector<vector<int>> DFA; // Stores the DFA

public:
    int encrypt_state(string transition)
    {
        /*
            Converts the String state to corresponding bit-masked integer
            Input: ABD
            Output: 11 (1011)
        */

        if (transition == "-")
            return 0;

        int encrypted = 0;
        for (char c : transition)
            encrypted |= (1 << (c - 'A'));
        return encrypted;
    }

    string decrypt_state(int transition)
    {
        /*
            Converts the bit-masked integer to corresponding string state
            Input: 20 (10100)
            Output: CE
        */

        if (transition == 0)
            return "\u03A6";

        string decrypted = "";
        for (int i = 0; i < 26; i++)
        {
            if (transition & (1 << i))
                decrypted += (i + 'A');
        }
        return decrypted;
    }

    void input_NFA()
    {
        /*
            Inputs the NFA from the input file
            Input Format:
            -> Number of States
            -> Number of Input Symbols
            For each State S:
                For each Symbol Sm:
                    -> Destination states on transition from S on getting Sm 
            -> Initial States
            -> Final States
        */

        cin >> num_states;
        cin >> num_symbols;

        NFA.resize(num_states, vector<int>(num_symbols + 1));
        for (int i = 0; i < num_states; i++)
        {
            NFA[i][0] = encrypt_state({char(i + 'A')});
            for (int j = 0; j < num_symbols; j++)
            {
                string transition;
                cin >> transition;
                NFA[i][j + 1] = encrypt_state(transition);
            }
        }

        string initial_state_input, final_states_input;
        cin >> initial_state_input;
        cin >> final_states_input;

        initial_state = encrypt_state(initial_state_input);
        final_states = encrypt_state(final_states_input);
    }

    void print_NFA()
    {
        /*
            Prints the NFA
            Output Format:
            State   (Transitions on Various Symbols)
        */

        cout << "NFA" << endl;

        cout << "State";
        for (int j = 0; j < num_symbols; j++)
            cout << "," << j;
        cout << endl;

        for (int i = 0; i < num_states; i++)
        {
            cout << decrypt_state(NFA[i][0]);

            if (NFA[i][0] == initial_state)
                cout << "(I)";

            if (NFA[i][0] & final_states)
                cout << "(F)";

            for (int j = 0; j < num_symbols; j++)
                cout << "," << decrypt_state(NFA[i][j + 1]);
            cout << endl;
        }
    }

    void convert_to_DFA()
    {
        /*
            Converts the NFA to DFA using the Bitmasking
        */

        vector<bool> visited((1 << num_states), 0);
        queue<int> waiting_states;

        waiting_states.push(initial_state);
        visited[initial_state] = 1;

        while (!waiting_states.empty())
        {
            int current_state = waiting_states.front();
            waiting_states.pop();

            vector<int> mapping;
            mapping.push_back(current_state);

            for (int j = 0; j < num_symbols; j++)
            {
                int destination = 0;
                for (int i = 0; i < num_states; i++)
                {
                    if (NFA[i][0] & current_state)
                        destination |= NFA[i][j + 1];
                }

                if (!visited[destination])
                {
                    visited[destination] = 1;
                    waiting_states.push(destination);
                }

                mapping.push_back(destination);
            }

            DFA.push_back(mapping);
        }

        sort(DFA.begin(), DFA.end());
    }

    void print_DFA()
    {
        /*
            Prints the DFA
            Output Format:
            State   (Transitions on Various Symbols)
        */

        cout << "DFA" << endl;

        cout << "State";
        for (int j = 0; j < num_symbols; j++)
            cout << "," << j;
        cout << endl;

        for (int i = 0; i < DFA.size(); i++)
        {
            cout << decrypt_state(DFA[i][0]);

            if (DFA[i][0] == initial_state)
                cout << "(I)";

            if (DFA[i][0] & final_states)
                cout << "(F)";

            for (int j = 0; j < num_symbols; j++)
                cout << "," << decrypt_state(DFA[i][j + 1]);
            cout << endl;
        }
    }
};

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    NFA_to_DFA_Converter convertor;
    convertor.input_NFA();
    convertor.print_NFA();

    convertor.convert_to_DFA();
    convertor.print_DFA();

    return 0;
}