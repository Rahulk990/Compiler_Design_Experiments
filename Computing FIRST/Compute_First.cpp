#include <bits/stdc++.h>
using namespace std;

class Symbol
{
    string name;
    bool terminal;
    set<Symbol *> first;
    set<Symbol *> follow;

public:
    Symbol(string name)
    {
        this->name = name;
        terminal = false;
    }

    string getName() { return name; }
    bool isTerminal() { return terminal; }
    set<Symbol *> getFirst() { return first; }
    set<Symbol *> getFollow() { return follow; }
    void setTerminal(bool terminal) { this->terminal = terminal; }
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
            if (c == ' ' || i == production.size() - 1)
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

                    leftSide->setTerminal(true);
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
                addRule(line);
            my_file.close();
        }
    }

    void computeFirst(Symbol *S)
    {
        if (!S->isTerminal())
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

    void createPredictiveParserTable()
    {
        // Computing First
        for (auto symbol : symbols)
            computeFirst(symbol);
    }

    void printRules()
    {
        for (auto productionRule : productionRules)
            productionRule->printRule();
    }

    void printFirst()
    {
        for (auto symbol : symbols)
        {
            if (symbol->getName() != "\u03B5")
                symbol->printFirst();
        }
    }
};

int main()
{
    freopen("output.txt", "w", stdout);

    Grammar grammar;
    grammar.inputData();
    grammar.createPredictiveParserTable();
    grammar.printFirst();

    return 0;
}