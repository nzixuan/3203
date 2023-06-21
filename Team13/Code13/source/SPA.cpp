#include "SPA.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "DesignExtractor.h"
#include "QueryParser.h"
#include "SimpleParser.h"
#include "Tokenizer.h"

SPA::SPA(){};

void SPA::parseSIMPLEFile(std::string filename) {
  try {
    std::ifstream sourceFile;
    sourceFile.open(filename);
    std::istream& filestream = sourceFile;

    const auto tokens = tokenizer.tokenize(filestream);

    ASTParser* ASTParse = new ASTParser(tokens);

    ASTParse->generateAST();

    CFGParser* CFGParse = new CFGParser(tokens);

    CFGParse->generateCFG();

    DesignExtractor::extract(ASTParse->returnASTTree(),
                             CFGParse->returnCFGList(), &pkb);

    pkb.storeCFG(CFGParse->returnCFGList());
  } catch (std::logic_error& e) {
    std::cout << e.what() << std::endl;
    exit(0);
  }
}

void SPA::parsePQL(std::string query, std::list<std::string>& results) {
  try {
    std::stringstream ss;
    ss << query;
    const auto tokens = tokenizer.tokenize(ss);
    QueryParser queryParser = QueryParser{tokens};
    Query query = queryParser.parseQuery();
    QueryEvaluator evaluator = QueryEvaluator(query, &pkb, results);
    evaluator.evaluate();
    pkb.clearQuerierCache();
  } catch (std::exception& e) {
    // Print statement only for testing purposes.
    results.push_back(e.what());
  }
}
