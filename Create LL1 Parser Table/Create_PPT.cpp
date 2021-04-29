#include <bits/stdc++.h>
using namespace std;

class Symbol
{
    string name;            // Stores the grammar symbol
    bool nonTerminal;       // Whether it is terminal or not
    set<Symbol *> first;    // Stores the FIRST of the symbol
    set<Symbol *> follow;   // Stores the FOLLOW of the symbol

public:
    Symbol(string name)
    {
        this->name = name;
        nonTerminal = false;
    }

    // Basic Methods
    string getName() { return name; }
    bool isNonTerminal() { return nonTerminal; }
    set<Symbol *> getFirst() { return first; }
    set<Symbol *> getFollow() { return follow; }
    void setNonTerminal(bool nonTerminal) { this->nonTerminal = nonTerminal; }
    void insertFirst(Symbol *symbol) { first.insert(symbol); }
    void insertFollow(Symbol *symbol) { follow.insert(symbol); }

    // Prints the FIRST of the symbol
    void printFirst()
    {
        string toPrint = "";
        toPrint += "FIRST(" + name + "): {";

        for (auto symbol : first)
            toPrint += symbol->getName() + ", ";

        toPrint.pop_back();
        toPrint.pop_back();
        toPrint += "}";

        cout << toPrint << endl;
    }

    // Prints the FOLLOW of the symbol
    void printFollow()
    {
        string toPrint = "";
        toPrint += "FOLLOW(" + name + "): {";

        for (auto symbol : follow)
            toPrint += symbol->getName() + ", ";

        toPrint.pop_back();
        toPrint.pop_back();
        toPrint += "}";

        cout << toPrint << endl;
    }
};

class ProductionRule
{
    Symbol *leftSide;               // Stores the head symbol of the production rule
    vector<Symbol *> rightSide;     // Stores the body of the production rule in form of symbols

public:
    ProductionRule(Symbol *left) { this->leftSide = left; }

    // Basic Methods
    Symbol *getLeftSide() { return leftSide; }
    vector<Symbol *> getRightSide() { return rightSide; }
    void appendRightSide(Symbol *symbol) { rightSide.push_back(symbol); }

    // Prints the production rule
    void printRule()
    {
        string toPrint = "";
        toPrint += leftSide->getName() + " -> ";

        for (auto symbol : rightSide)
            toPrint += symbol->getName();
        cout << toPrint;
    }
};

class Grammar
{
    vector<Symbol *> symbols;                                   // Stores all the symbols of the grammar
    vector<ProductionRule *> productionRules;                   // Stores all the production rules of the grammar

    vector<Symbol *> terminals, nonTerminals;                   // Stores the terminals and nonTerminals separately
    vector<vector<ProductionRule *>> predictiveParserTable;     // Stores the predictive parser table for the grammar

public:

    // Checks for second occurrence of the symbol
    // Inserts one, if not already present
    int installSymbol(string name)
    {
        for (int i = 0; i < symbols.size(); i++)
        {
            if (symbols[i]->getName() == name)
                return i;
        }
        return -1;
    }

    // Adds rule to the grammar
    void addRule(string production)
    {
        bool flag = false;
        string parse = "";

        Symbol *leftSide, *newSymbol;
        ProductionRule *newProductionRule;

        for (int i = 0; i < production.size(); i++)
        {
            char c = production[i];
            if (c == ' ')
            {
                if (!flag)
                {
                    int index = installSymbol(parse);
                    if (index == -1)
                    {
                        leftSide = new Symbol(parse);
                        symbols.push_back(leftSide);
                    }
                    else
                        leftSide = symbols[index];

                    i += 3;
                    parse = "";
                    flag = true;

                    leftSide->setNonTerminal(true);
                    newProductionRule = new ProductionRule(leftSide);
                }
                else
                {
                    int index = installSymbol(parse);
                    if (index == -1)
                    {
                        newSymbol = new Symbol(parse);
                        symbols.push_back(newSymbol);
                    }
                    else
                        newSymbol = symbols[index];

                    parse = "";
                    if (i == production.size() - 1 || production[i + 1] == '|')
                    {
                        i += 2;
                        newProductionRule->appendRightSide(newSymbol);

                        productionRules.push_back(newProductionRule);
                        newProductionRule = new ProductionRule(leftSide);
                    }
                    else
                        newProductionRule->appendRightSide(newSymbol);
                }
            }
            else
                parse += c;
        }
    }

    // Takes input from the input file 
    void inputData()
    {
        ifstream my_file("input.txt");
        if (my_file.is_open())
        {
            string line;
            while (getline(my_file, line))
                addRule(line + " ");
            my_file.close();
        }
    }

    // Compute First of any Substring of symbols
    set<Symbol *> computeFirstRightSide(vector<Symbol *> rightSide)
    {
        set<Symbol *> newFirst;
        int epsilon = installSymbol("\u03B5");

        for (auto symbol : rightSide)
        {
            set<Symbol *> first = symbol->getFirst();
            for (auto newSymbol : first)
                newFirst.insert(newSymbol);

            if (first.find(symbols[epsilon]) == first.end())
                break;
        }

        return newFirst;
    }

    // Compute First for any given symbol
    void computeFirst(Symbol *S)
    {   
        // If the symbol is a terminal, it is the FIRST itself
        if (!S->isNonTerminal())
        {
            S->insertFirst(S);
            return;
        }

        int epsilon = installSymbol("\u03B5");
        for (auto productionRule : productionRules)
        {
            // Traverse over all productions and check if the head is required symbol or not
            if (productionRule->getLeftSide() != S)
                continue;

            // Inserts the symbols in the first of given symbol, according to the rules
            vector<Symbol *> rightSide = productionRule->getRightSide();
            for (auto symbol : rightSide)
            {
                if (symbol == S)
                    continue;

                // Compute first of the leftmost symbol
                computeFirst(symbol);
                set<Symbol *> first = symbol->getFirst();

                for (auto newSymbol : first)
                    S->insertFirst(newSymbol);

                // If it doesn't contain epislon, exits
                if (first.find(symbols[epsilon]) == first.end())
                    break;
            }
        }
    }

    // Compute Follow for the given nonTerminals
    void computeFollow(Symbol *S)
    {
        // If symbol is a terminal, exits
        if (!S->isNonTerminal())
            return;
        
        int epsilon = installSymbol("\u03B5");
        for (auto productionRule : productionRules)
        {
            bool isFound = false, isEnd = false;
            vector<Symbol *> rightSide = productionRule->getRightSide();

            // Traverse over all productions and check if body contains required symbol or not
            for (int i = 0; i < rightSide.size(); i++)
            {
                auto symbol = rightSide[i];
                if (symbol == S)
                {
                    isFound = true;
                    if (i == rightSide.size() - 1)
                        isEnd = true;
                }

                if (symbol == S || !isFound)
                    continue;

                // Inserts the symbols in the follow of given symbol, according to the rules
                set<Symbol *> first = symbol->getFirst();
                for (auto newSymbol : first)
                {
                    if (newSymbol != symbols[epsilon])
                        S->insertFollow(newSymbol);
                }

                // If the first of next symbol doesn't contain epislon, exits
                if (first.find(symbols[epsilon]) == first.end())
                    break;

                if (i == rightSide.size() - 1)
                    isEnd = true;
            }

            // If the first of complete right side contains epsilon
            // Adds the Follow of head to teh Follow of given symbol 
            auto leftSide = productionRule->getLeftSide();
            if (isEnd && S != leftSide)
            {
                computeFollow(leftSide);
                set<Symbol *> follow = leftSide->getFollow();
                for (auto newSymbol : follow)
                    S->insertFollow(newSymbol);
            }
        }
    }

    // Separates the Symbols into terminals and non-terminals
    void separateSymbols()
    {
        for (auto symbol : symbols)
        {
            if (symbol->getName() == "\u03B5")
                continue;

            if (symbol->isNonTerminal())
                nonTerminals.push_back(symbol);
            else
                terminals.push_back(symbol);
        }
    }

    // Creates the Predictive Parsing Table for the grammar
    void createTable()
    {
        int rows = nonTerminals.size();
        int columns = terminals.size();

        int epsilon = installSymbol("\u03B5");
        predictiveParserTable.resize(rows, vector<ProductionRule *>(columns, NULL));

        // Traverse all the production rules and fill the corresponding entries in the table using rules
        for (auto productionRule : productionRules)
        {
            auto leftSide = productionRule->getLeftSide();
            set<Symbol *> follow = leftSide->getFollow();
            set<Symbol *> firstRightSide = computeFirstRightSide(productionRule->getRightSide());
            bool flag = firstRightSide.find(symbols[epsilon]) != firstRightSide.end();

            for (int i = 0; i < rows; i++)
            {
                if (nonTerminals[i] != leftSide)
                    continue;

                for (int j = 0; j < columns; j++)
                {
                    if (firstRightSide.find(terminals[j]) != firstRightSide.end())
                        predictiveParserTable[i][j] = productionRule;

                    if (flag && follow.find(terminals[j]) != follow.end())
                        predictiveParserTable[i][j] = productionRule;
                }
            }
        }
    }

    void createPredictiveParserTable()
    {
        // Computing First
        for (auto symbol : symbols)
            computeFirst(symbol);

        // Computing Follow
        Symbol *dollar = new Symbol("$");
        symbols.push_back(dollar);
        symbols[0]->insertFollow(dollar);

        for (auto symbol : symbols)
            computeFollow(symbol);

        // Creating Table
        separateSymbols();
        createTable();
    }

    // Print all rules of the grammar
    void printRules()
    {
        for (auto productionRule : productionRules)
        {
            productionRule->printRule();
            cout<<endl;
        }
    }

    // Prints the FIRST of all the symbols in the grammar
    void printFirst()
    {
        for (auto symbol : symbols)
        {
            string name = symbol->getName();
            if (name != "\u03B5" && name != "$")
                symbol->printFirst();
        }
    }

    // Prints the FOLLOW of all the symbols in the grammar 
    void printFollow()
    {
        for (auto symbol : symbols)
        {
            if (symbol->isNonTerminal())
                symbol->printFollow();
        }
    }

    // Prints the Predictive Parser Table for the grammar
    void printTable()
    {
        int rows = nonTerminals.size();
        int columns = terminals.size();

        for(int j = 0; j < columns; j++)
            cout<<","<<terminals[j]->getName();

        cout<<endl;
        for (int i = 0; i < rows; i++)
        {
            cout<<nonTerminals[i]->getName();
            for (int j = 0; j < columns; j++)
            {
                cout<<",";
                if(predictiveParserTable[i][j])
                    predictiveParserTable[i][j]->printRule();
            }
            cout<<endl;
        }
    }
};

int main()
{
    freopen("output.txt", "w", stdout);

    Grammar grammar;
    grammar.inputData();
    grammar.createPredictiveParserTable();
    grammar.printTable();

    return 0;
}