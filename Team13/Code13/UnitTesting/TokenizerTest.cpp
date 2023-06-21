#include <list>
#include <sstream>
#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "Tokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{

	TEST_CLASS(TokenizerTest)
	{
	public:

		Tokenizer tokenizer;

		TEST_METHOD(testIdentifyIdentifier)
		{
			std::string exampleIdentifier = "identifier123";

			Token tokenStub = { TokenType::Identifier, exampleIdentifier };

			std::stringstream ss;
			ss << exampleIdentifier;
			

			std::list<Token> tokens = tokenizer.tokenize(ss);

			Assert::IsTrue(tokenStub == tokens.front());
		}

		
		TEST_METHOD(testIdentifyNumber)
		{
			std::string exampleNumber = "123";

			Token tokenStub = { TokenType::Number, exampleNumber };

			std::stringstream ss;
			ss << exampleNumber;

			const auto tokens = tokenizer.tokenize(ss);

			Assert::IsTrue(tokenStub == tokens.front());
		}

		TEST_METHOD(testIdentifyDelimiters)
		{
			std::string exampleDelimiters = "\"(){};,_.#";

			std::stringstream ss;
			ss << exampleDelimiters;


			const auto tokens = tokenizer.tokenize(ss);

			for (auto token = tokens.begin(); token != tokens.end(); token++) {
				Assert::IsTrue(token->tokenType == TokenType::Delimiter);
			}

			auto token = tokens.begin();

			Assert::IsTrue((token++)->val == "\"");
			Assert::IsTrue((token++)->val == "(");
			Assert::IsTrue((token++)->val == ")");
			Assert::IsTrue((token++)->val == "{");
			Assert::IsTrue((token++)->val == "}");
			Assert::IsTrue((token++)->val == ";");
			Assert::IsTrue((token++)->val == ",");
			Assert::IsTrue((token++)->val == "_");
			Assert::IsTrue((token++)->val == ".");
			Assert::IsTrue((token++)->val == "#");
		}

		TEST_METHOD(testIdentifyOperators)
		{
			std::string exampleOperators = "+-*%/=";

			std::stringstream ss;
			ss << exampleOperators;


			const auto tokens = tokenizer.tokenize(ss);

			for (auto token = tokens.begin(); token != tokens.end(); token++) {
				Assert::IsTrue(token->tokenType == TokenType::Operator);
			}

			auto token = tokens.begin();
			Assert::IsTrue(token++->val == "+");
			Assert::IsTrue((token++)->val == "-");
			Assert::IsTrue((token++)->val == "*");
			Assert::IsTrue((token++)->val == "%");
			Assert::IsTrue((token++)->val == "/");
			Assert::IsTrue((token++)->val == "=");

			
		}

		TEST_METHOD(testIdentifyComparisonOperators)
		{
			std::string exampleOperators = "< > <= >= != ==";

			std::stringstream ss;
			ss << exampleOperators;


			const auto tokens = tokenizer.tokenize(ss);

			for (auto token = tokens.begin(); token != tokens.end(); token++) {
				Assert::IsTrue(token->tokenType == TokenType::ComparisonOperator);
			}

			auto token = tokens.begin();
			Assert::IsTrue((token++)->val == "<");
			Assert::IsTrue((token++)->val == ">");
			Assert::IsTrue((token++)->val == "<=");
			Assert::IsTrue((token++)->val == ">=");
			Assert::IsTrue((token++)->val == "!=");
			Assert::IsTrue((token++)->val == "==");
		}
		
		TEST_METHOD(testIdentifyBooleanOperators)
		{
			std::string exampleOperators = "! && ||";

			std::stringstream ss;
			ss << exampleOperators;

			const auto tokens = tokenizer.tokenize(ss);

			for (auto token = tokens.begin(); token != tokens.end(); token++) {
				Assert::IsTrue(token->tokenType == TokenType::BooleanOperator);
			}

			auto token = tokens.begin();

			Assert::IsTrue((token++)->val == "!");
			Assert::IsTrue((token++)->val == "&&");
			Assert::IsTrue((token++)->val == "||");
		}


		TEST_METHOD(testSkipSpaces)
		{
			std::string exampleOperator = "    ";


			std::stringstream ss;
			ss << exampleOperator;

			const auto tokens = tokenizer.tokenize(ss);

			Assert::IsTrue(tokens.size() == 0);
		}

		TEST_METHOD(testUnknownPunctuation)
		{
			
			auto func = [this] { 
				std::string exampleOperator = "@";
				std::stringstream ss;
				ss << exampleOperator;
				const auto tokens = tokenizer.tokenize(ss);
			};

			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testUnknownAndOperator)
		{
			auto func = [this] {
				std::string exampleOperator = "&1";
				std::stringstream ss;
				ss << exampleOperator;
				const auto tokens = tokenizer.tokenize(ss);
			};

			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testUnknownOrOperator)
		{

			auto func = [this] {
				std::string exampleOperator = "|1";
				std::stringstream ss;
				ss << exampleOperator;
				const auto tokens = tokenizer.tokenize(ss);
			};
			Assert::ExpectException<std::logic_error>(func);
		}
		
	};
}