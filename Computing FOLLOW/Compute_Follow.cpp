#include <bits/stdc++.h>
using namespace std;

class Symbol
{
    string name;
    bool nonTerminal;
    set<Symbol *> first;
    set<Symbol *> follow;

public:
    Symbol(string name)
    {
        this->name = name;
        nonTerminal = false;
    }

    string getName() { return name; }
    bool isNonTerminal() { return nonTerminal; }
    set<Symbol *> getFirst() { return first; }
    set<Symbol *> getFollow() { return follow; }
    void setNonTerminal(bool nonTerminal) { this->nonTerminal = nonTerminal; }
    void insertFirst(Symbol *symbol) { first.insert(symbol); }
    void insertFollow(Symbol *symbol) { follow.insert(symbol); }

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
    Symbol *leftSide;
    vector<Symbol *> rightSide;

public:
    ProductionRule(Symbol *left) { this->leftSide = left; }
    Symbol *getLeftSide() { return leftSide; }
    vector<Symbol *> getRightSide() { return rightSide; }
    void appendRightSide(Symbol *symbol) { rightSide.push_back(symbol); }

    void printRule()
    {
        string toPrint = "";
        toPrint += leftSide->getName() + " -> ";

        for (auto symbol : rightSide)
            toPrint += symbol->getName();
        cout << toPrint << endl;
    }
};

class Grammar
{
    vector<Symbol *> symbols;
    vector<ProductionRule *> productionRules;

public:
    int installSymbol(string name)
    {
        for (int i = 0; i < symbols.size(); i++)
        {
            if (symbols[i]->getName() == name)
                return i;
        }
        return -1;
    }

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

    void computeFirst(Symbol *S)
    {
        if (!S->isNonTerminal())
        {
            S->insertFirst(S);
            return;
        }

        int epsilon = installSymbol("\u03B5");
        for (auto productionRule : productionRules)
        {
            if (productionRule->getLeftSide() != S)
                continue;

            vector<Symbol *> rightSide = productionRule->getRightSide();
            for (auto symbol : rightSide)
            {
                if (symbol == S)
                    continue;

                computeFirst(symbol);
                set<Symbol *> first = symbol->getFirst();

                for (auto newSymbol : first)
                    S->insertFirst(newSymbol);

                if (first.find(symbols[epsilon]) == first.end())
                    break;
            }
        }
    }

    void computeFollow(Symbol *S)
    {
        if (!S->isNonTerminal())
            return;

        int epsilon = installSymbol("\u03B5");
        for (auto productionRule : productionRules)
        {
            vector<int> foundAt;
            vector<Symbol *> rightSide = productionRule->getRightSide();

            for (int i = 0; i < rightSide.size(); i++)
                if (rightSide[i] == S)
                    foundAt.push_back(i);

            for (int j = 0; j < foundAt.size(); j++)
            {
                bool isEnd = false;
                if (foundAt[j] == rightSide.size() - 1)
                    isEnd = true;
                
                for (int i = foundAt[j] + 1; i < rightSide.size(); i++)
                {
                    auto symbol = rightSide[i];
                    set<Symbol *> first = symbol->getFirst();

                    for (auto newSymbol : first)
                    {
                        if (newSymbol != symbols[epsilon])
                            S->insertFollow(newSymbol);
                    }

                    if (first.find(symbols[epsilon]) == first.end())
                        break;

                    if (i == rightSide.size() - 1)
                        isEnd = true;
                }

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
    }

    void createPredictiveParserTable()
    {
        // Computing First
        for (auto symbol : symbols)
            computeFirst(symbol);

        // Computing Follow
        Symbol *dollar = new Symbol("$");
        symbols[0]->insertFollow(dollar);

        for (auto symbol : symbols)
            computeFollow(symbol);
    }

    void printFollow()
    {
        for (auto symbol : symbols)
        {
            if (symbol->isNonTerminal())
                symbol->printFollow();
        }
    }
};

int main()
{
    freopen("output.txt", "w", stdout);

    Grammar grammar;
    grammar.inputData();
    grammar.createPredictiveParserTable();
    grammar.printFollow();

    return 0;
}
