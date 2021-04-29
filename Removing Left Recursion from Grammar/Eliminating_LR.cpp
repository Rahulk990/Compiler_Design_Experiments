#include <bits/stdc++.h>
using namespace std;

class NonTerminal
{
	string name;						// Stores the Head of production rule
	vector<string> productionRules;		// Stores the body of production rules

public:
	NonTerminal(string name)
	{
		this->name = name;
	}

	// Returns the head of the production rule
	string getName()
	{
		return name;
	}

	// Returns the body of the production rules
	void setRules(vector<string> rules)
	{
		productionRules.clear();
		for (auto rule : rules)
			productionRules.push_back(rule);
	}

	vector<string> getRules()
	{
		return productionRules;
	}

	void addRule(string rule)
	{
		productionRules.push_back(rule);
	}

	// Prints the production rules
	void printRule()
	{
		string toPrint = "";
		toPrint += name + " =>";

		for (string s : productionRules)
			toPrint += " " + s + " |";

		toPrint.pop_back();
		cout << toPrint << endl;
	}
};

class Grammar
{
	vector<NonTerminal> nonTerminals;

public:

	// Add rules to the grammar
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
					NonTerminal newNonTerminal(parse);
					nonTerminals.push_back(newNonTerminal);

					nt = 1;
					parse = "";
				}
				else if (parse.size())
				{
					nonTerminals.back().addRule(parse);
					parse = "";
				}
			}
			else if (c != '|' && c != '=' && c != '>')
				parse += c;
		}

		if (parse.size())
			nonTerminals.back().addRule(parse);
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

	// Algorithm for eliminating the non-Immediate Left Recursion
	void solveNonImmediateLR(NonTerminal &A, NonTerminal &B)
	{
		string nameA = A.getName();
		string nameB = B.getName();

		vector<string> rulesA, rulesB, newRulesA;
		rulesA = A.getRules();
		rulesB = B.getRules();

		for (auto rule : rulesA)
		{
			if (rule.substr(0, nameB.size()) == nameB)
			{
				for (auto rule1 : rulesB)
					newRulesA.push_back(rule1 + rule.substr(nameB.size()));
			}
			else
				newRulesA.push_back(rule);
		}

		A.setRules(newRulesA);
	}

	// Algorithm for eliminating Immediate Left Recursion
	void solveImmediateLR(NonTerminal &A)
	{
		string name = A.getName();
		string newName = name + "'";

		vector<string> alphas, betas, rules, newRulesA, newRulesA1;
		rules = A.getRules();

		// Checks if there is left recursion or not
		for (auto rule : rules)
		{
			if (rule.substr(0, name.size()) == name)
				alphas.push_back(rule.substr(name.size()));
			else
				betas.push_back(rule);
		}

		// If no left recursion, exit
		if (!alphas.size())
			return;

		if (!betas.size())
			newRulesA.push_back(newName);

		for (auto beta : betas)
			newRulesA.push_back(beta + newName);

		for (auto alpha : alphas)
			newRulesA1.push_back(alpha + newName);

		// Amends the original rule
		A.setRules(newRulesA);
		newRulesA1.push_back("\u03B5");

		// Adds new production rule
		NonTerminal newNonTerminal(newName);
		newNonTerminal.setRules(newRulesA1);
		nonTerminals.push_back(newNonTerminal);
	}

	// Eliminates left recursion
	void applyAlgorithm()
	{
		int size = nonTerminals.size();
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < i; j++)
				solveNonImmediateLR(nonTerminals[i], nonTerminals[j]);
			solveImmediateLR(nonTerminals[i]);
		}
	}

	// Print all the rules of grammar
	void printRules()
	{
		for (auto nonTerminal : nonTerminals)
			nonTerminal.printRule();
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