#include <bits/stdc++.h>
using namespace std;

class NonTerminal
{
	string name;
	vector<string> productionRules;

public:
	NonTerminal(string name)
	{
		this->name = name;
	}

	string getName()
	{
		return name;
	}

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

	void solveImmediateLR(NonTerminal &A)
	{
		string name = A.getName();
		string newName = name + "'";

		vector<string> alphas, betas, rules, newRulesA, newRulesA1;
		rules = A.getRules();

		for (auto rule : rules)
		{
			if (rule.substr(0, name.size()) == name)
				alphas.push_back(rule.substr(name.size()));
			else
				betas.push_back(rule);
		}

		if (!alphas.size())
			return;

		if (!betas.size())
			newRulesA.push_back(newName);

		for (auto beta : betas)
			newRulesA.push_back(beta + newName);

		for (auto alpha : alphas)
			newRulesA1.push_back(alpha + newName);

		A.setRules(newRulesA);
		newRulesA1.push_back("\u03B5");

		NonTerminal newNonTerminal(newName);
		newNonTerminal.setRules(newRulesA1);
		nonTerminals.push_back(newNonTerminal);
	}

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