#include <bits/stdc++.h>
using namespace std;

class ProductionRule
{
    string LeftHandSide;
    vector<string> productions;

public:
    ProductionRule(string leftHandSide)
    {
        this->LeftHandSide = leftHandSide;
    }

    string getLHS()
    {
        return LeftHandSide;
    }

    void setProductions(vector<string> productions)
    {
        this->productions.clear();
        for (auto production : productions)
            this->productions.push_back(production);
    }

    vector<string> getProductions()
    {
        return productions;
    }

    void addProduction(string production)
    {
        productions.push_back(production);
    }

    void printRule()
    {
        string toPrint = "";
        toPrint += LeftHandSide + " =>";

        for (string s : productions)
            toPrint += " " + s + " |";

        toPrint.pop_back();
        cout << toPrint << endl;
    }
};

class Grammar
{
    vector<ProductionRule> productionRules;

public:
    void addRule(string rule)
    {
        bool nt = 0;
        string parse = "";

        for (char c : rule)
        {
            if (c == ' ')
            {
                if (!nt)
                {
                    ProductionRule newProductionRule(parse);
                    productionRules.push_back(newProductionRule);

                    nt = 1;
                    parse = "";
                }
                else if (parse.size())
                {
                    productionRules.back().addProduction(parse);
                    parse = "";
                }
            }
            else if (c != '|' && c != '=' && c != '>')
                parse += c;
        }

        if (parse.size())
            productionRules.back().addProduction(parse);
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

    ProductionRule leftFactoring(ProductionRule A)
    {
        string LHS = A.getLHS();

        vector<string> productions, newProductionsA;
        productions = A.getProductions();

        int newRulesCreated = 0;
        vector<int> checked(productions.size(), 0);
        for (int i = 0; i < productions.size(); i++)
        {
            if (checked[i] != 0)
                continue;

            // Finding all productions that start with P[i][0]
            vector<int> temp;
            for (int j = 0; j < productions.size(); j++)
            {
                if (checked[j] == 0 && productions[i][0] == productions[j][0])
                    temp.push_back(j);
            }

            if (temp.size() == 1)
            {
                checked[i] = 1;
                newProductionsA.push_back(productions[i]);
                continue;
            }

            int commonPrefixLength = 0;
            string commonPrefix = "";
            while (true)
            {
                int brk = 0;
                char x = productions[i][commonPrefixLength];
                for (auto tp : temp)
                {
                    if (commonPrefixLength >= productions[tp].size())
                    {
                        brk = 1;
                        break;
                    }

                    if (x != productions[tp][commonPrefixLength])
                    {
                        brk = 1;
                        break;
                    }
                }

                if (brk)
                    break;
                else
                {
                    commonPrefix += x;
                    commonPrefixLength += 1;
                }
            }

            newRulesCreated += 1;
            string newLHS = LHS + char(newRulesCreated + '0');
            vector<string> newProductionsA1;

            newProductionsA.push_back(commonPrefix + newLHS);
            for (auto tp : temp)
            {
                string suffix = productions[tp].substr(commonPrefixLength);
                if (suffix.size() == 0)
                    newProductionsA1.push_back("\u03B5");
                else
                    newProductionsA1.push_back(suffix);
                checked[tp] = 1;
            }

            ProductionRule newProductionRule(newLHS);
            newProductionRule.setProductions(newProductionsA1);
            productionRules.push_back(newProductionRule);
        }

        ProductionRule newProductionRule(LHS);
        newProductionRule.setProductions(newProductionsA);
        return newProductionRule;
    }

    void applyAlgorithm()
    {
        for (int i = 0; i < productionRules.size(); i++)
        {
            productionRules[i] = leftFactoring(productionRules[i]);
        }
    }

    void printRules()
    {
        for (auto productionRule : productionRules)
            productionRule.printRule();
    }
};

int main()
{
    freopen("output.txt", "w", stdout);

    Grammar grammar;
    grammar.inputData();
    grammar.applyAlgorithm();
    grammar.printRules();

    return 0;
}