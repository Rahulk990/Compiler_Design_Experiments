#include <bits/stdc++.h>
using namespace std;

// Initializing the list of predefined Keywords
vector<string> keywords = {"int", "main", "print", "if", "for", "return"};

class State
{
    map<char, int> transition; // Store the transitions from current state
    string token_name;         // Stores the Type of token produced by the state, if any

public:
    State() {}

    State(vector<char> symbol, vector<int> next_state, string token = "")
    {
        for (int i = 0; i < symbol.size(); i++)
            transition[symbol[i]] = next_state[i];

        token_name = token;
    }

    bool is_final_state()
    {
        /*
            Returns whether this state is a final state or not
            A token should be produced if it is a final state
        */

        if (token_name.size())
            return true;
        return false;
    }

    void produce_token(string lexeme)
    {
        /*
            Produces a token based on the lexeme
            Format: <Token_type, Token_value>
        */

        if (token_name == "Identifier" && count(keywords.begin(), keywords.end(), lexeme))
        {
            string token = "<Keyword, " + lexeme + ">";
            cout << token << " ";
            return;
        }

        string token = "<" + token_name + ", " + lexeme + ">";
        cout << token << " ";
    }

    int next_state(char character)
    {
        /*
            Takes the next input symbols and returns the next state after Transition
        */

        if (transition.count('L') && isalpha(character))
            return transition['L'];
        else if (transition.count('D') && isdigit(character))
            return transition['D'];
        else if (transition.count(character))
            return transition[character];
        return transition['O'];
    }
};

class Lexical_Analyzer
{
    string input_program; // Stores the input program
    vector<State> states; // Stores all the states of LA
    State current_state;  // Stores the current state
    string lexeme;        // Stores the currently parsed lexeme

public:
    Lexical_Analyzer()
    {
        input_program = "";

        // Initializing the states of Lexical Analyzer
        states.push_back(State({'<', '>', '=', ':', '+', '-', '*', ';', '(', ')', 'L', '"', '/', 'D', ' '}, {1, 7, 11, 13, 17, 19, 21, 23, 25, 27, 29, 31, 34, 39, 48}));
        states.push_back(State({'O', '=', '>'}, {2, 3, 5}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'O'}, {4}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'O'}, {5}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'=', 'O'}, {8, 10}));
        states.push_back(State({'O'}, {9}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'O'}, {12}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'=', 'O'}, {14, 16}));
        states.push_back(State({'O'}, {15}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'O'}, {18}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'O'}, {20}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'O'}, {22}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'O'}, {24}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'O'}, {26}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'O'}, {28}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'L', 'D', 'O'}, {29, 29, 30}));
        states.push_back(State({}, {}, "Identifier"));
        states.push_back(State({'O', '"'}, {31, 32}));
        states.push_back(State({'O'}, {33}));
        states.push_back(State({}, {}, "Literal"));
        states.push_back(State({'O', '*'}, {38, 35}));
        states.push_back(State({'O', '*'}, {35, 36}));
        states.push_back(State({'/', 'O'}, {37, 35}));
        states.push_back(State({'O'}, {0}));
        states.push_back(State({}, {}, "Operator"));
        states.push_back(State({'D', '.', 'E', 'O'}, {39, 41, 44, 40}));
        states.push_back(State({}, {}, "Constant"));
        states.push_back(State({'D'}, {42}));
        states.push_back(State({'D', 'E', 'O'}, {42, 44, 43}));
        states.push_back(State({}, {}, "Constant"));
        states.push_back(State({'+', '-'}, {45}));
        states.push_back(State({'D'}, {46}));
        states.push_back(State({'D', 'O'}, {46, 47}));
        states.push_back(State({}, {}, "Constant"));
        states.push_back(State({' ', 'O'}, {48, 0}));

        current_state = states[0];
        lexeme = "";
    }

    void input_string()
    {
        /*
            Takes the input program for which the tokens have to be produced by the LA
        */

        ifstream my_file("input.txt");
        if (my_file.is_open())
        {
            string line;
            while (getline(my_file, line))
                input_program += line;

            my_file.close();
        }
    }

    void tokenize()
    {
        /*
            Takes the lexemes character by character and simulates the transitions in LA
        */

        for (char c : input_program)
        {
            if (current_state.next_state(c) == 0)
            {
                current_state = states[0];
                lexeme = "";
            }

            current_state = states[current_state.next_state(c)];

            if (current_state.is_final_state())
            {
                current_state.produce_token(lexeme);
                current_state = states[0];

                lexeme = "";
                current_state = states[current_state.next_state(c)];
            }

            lexeme += c;
        }
    }
};

int main()
{
    freopen("output.txt", "w", stdout);

    Lexical_Analyzer LA;
    LA.input_string();
    LA.tokenize();

    return 0;
}