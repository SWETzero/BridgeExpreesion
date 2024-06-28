#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

//这里仅能去除单层括号，是用于桥梁跨径表达式的特殊处理算法
std::vector<std::string> splitExpression(const std::string& expression)
{
	std::vector<std::string> result;
	std::string buffer;
	int level = 0;

	for (char ch : expression)
	{
		if (ch == '(')
		{
			level++;
		}
		else if (ch == ')')
		{
			level--;
		}
		else if (ch == '+' && level == 0)
		{
			result.push_back(buffer);
			buffer.clear();
			continue;
		}
		buffer += ch;
	}
	if (!buffer.empty())
	{
		result.push_back(buffer);
	}

	return result;
}
//
//std::vector<double> calculateExpression(const std::string& expression) 
//{
//	std::istringstream iss(expression);
//	std::vector<double> output;
//	double num1, num2;
//	char op;
//	if (expression.find('*') != std::string::npos)
//	{
//		iss >> num1 >> op >> num2;
//		for (auto it = 0; it < num1; ++it)
//		{
//			output.push_back(num2);
//		}
//	}
//	else
//	{
//		// 如果没有乘号，直接push数字
//		output.push_back(stod(expression));
//	}
//}
//
//std::string multiplyByI(const std::string& expression, int i) 
//{
//	std::smatch matches;
//	// 使用正则表达式匹配表达式中的第一个数字
//
//	if (expression.find('*') != std::string::npos)
//	{
//		std::regex firstNumberRegex(R"(^\d+)");
//		if (std::regex_search(expression, matches, firstNumberRegex))
//		{
//			// 如果找到数字，将其乘以i
//			int firstNumber = std::stoi(matches[0]);
//			std::string multiplied = std::to_string(firstNumber * i);
//			// 替换表达式中的第一个数字为乘以i后的结果
//			return std::regex_replace(expression, firstNumberRegex, multiplied);
//		}
//	}
//	else 
//	{
//		// 如果没有找到数字，直接在字符串前加上i*
//		std::string temp = std::to_string(i) + "*" + expression;
//		return temp;
//	}
//}

void parseAndStore(const std::vector<std::string>& subExpressions, std::vector<double>& array)
{
	std::regex plainExpr("^(\\d+)([*+])?(\\d+)?$"); // 不带括号的表达式
	std::regex bracketExpr("^\\((.*)\\)$"); // 带括号但括号外没有数据的表达式
	std::regex complexExpr("^(\\d+)\\*(\\(.*\\))$"); // 带括号且括号外有数据的表达式

	std::vector<std::string> MoreSub;
	for (const auto& expr : subExpressions)
	{
		std::smatch matches;
		if (std::regex_match(expr, matches, plainExpr))
		{
			MoreSub.push_back(expr);
			MoreSub.push_back("0");
		}
		else
		{
			if (std::regex_match(expr, matches, bracketExpr))
			{
				std::string strings = matches[1];
				std::vector<std::string> substrings = splitExpression(strings);
				for (auto it = substrings.begin(); it != substrings.end(); ++it)
				{
					MoreSub.push_back(*it);
				}
				MoreSub.push_back("0");
			}
			else if (std::regex_match(expr, matches, complexExpr))
			{
				std::string strings = matches[2];
				std::string result = std::regex_replace(strings, std::regex(R"(\(|\))"), "");
				std::vector<std::string> substrings = splitExpression(result);
				for (auto multiply = 0; multiply < stoi(matches[1]); ++multiply)
				{
					for (auto it = substrings.begin(); it != substrings.end(); ++it)
					{
						MoreSub.push_back(*it);
					}	
					MoreSub.push_back("0");
				}
			}
		}
	}
	for (auto it = MoreSub.begin(); it != MoreSub.end(); ++it)
	{
		std::istringstream iss(*it);
		double num1, num2;
		char op;
		if (it->find('*') != std::string::npos)
		{
			iss >> num1 >> op >> num2;
			for (auto it = 0; it < num1; ++it)
			{
				array.push_back(num2);
			}
		}
		else 
		{
			// 如果没有乘号，直接push数字
			array.push_back(stod(*it));
		}
	}

}

//返回每个单跨长度的数组
std::vector<double> PareseBridgeExpression(std::string BrigeExpression)
{
	std::vector<std::string> SubExpression = splitExpression(BrigeExpression);
	std::vector<double> summary;
	parseAndStore(SubExpression, summary);	
	return summary;
}

void main()
{
	std::cout << "Hello World!" << std::endl;
	double MiddleStation = 10000;
	std::string Message("2*(3*50+25+5*30)+(2*20+25)+4*20");
	double sum = 0;
	std::vector<double> spans(PareseBridgeExpression(Message));
	for (auto it = spans.begin(); it != spans.end(); ++it)
	{
		std::cout << "单个跨径长度：" << *it << std::endl;
		sum += *it;
	}
	std::cout << "总跨径：" << sum << std::endl;
}
