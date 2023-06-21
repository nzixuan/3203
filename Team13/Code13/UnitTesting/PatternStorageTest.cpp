#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "PatternStorage.h"
#include "common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(PatternStorageTest) {

        PatternStorage patternStorage = PatternStorage();
        EntityType assignEntity = EntityType::Assign;

        TEST_METHOD_INITIALIZE(initializePatternStorage) {
            //StmtNoSet stmtNos = StmtNoSet({ 1, 2, 3, 4 });
            //patternStorage.addEntityToStmt(assignEntity, stmtNos);
        }

        TEST_METHOD_CLEANUP(destroyPatternStorage) {

        }


    };

};

