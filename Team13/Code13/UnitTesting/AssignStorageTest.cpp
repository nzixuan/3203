#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "AssignStorage.h"
#include "common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(AssignStorageTest) {

        AssignStorage assignStorage = AssignStorage();
        EntityType assignEntity = EntityType::Assign;

        TEST_METHOD_INITIALIZE(initializeAssignStorage) {
         
        }

        TEST_METHOD_CLEANUP(destroyAssignStorage) {
            assignStorage.clear();
        }

        TEST_METHOD(getPatternAssignLhsSynRhsWildcardTest) {
            std::string lhs = "x";
            std::string rhs = "y + 1";
            assignStorage.storeFullExpAssignMap(1, lhs, rhs);

            std::list <PKBEntityPair> lst = assignStorage.getPatternAssignLhsSynRhsWildcard();
            auto l_front = lst.begin();

            PKBEntityPair p = *l_front;
            Assert::IsTrue(p.lhs.val == "1");
            Assert::IsFalse(p.lhs.val == "y + 1");
            Assert::IsTrue(p.rhs.val == "x");
            Assert::IsFalse(p.rhs.val == "1");
        };

        TEST_METHOD(getPatternAssignLhsSynRhsExpTest) {
            // Store FullExp
            std::string lhs = "x";
            std::string rhs = "v + x * y + z * t";
            assignStorage.storeFullExpAssignMap(1, lhs, rhs);

            // Store SubExps
            std::unordered_set<std::string> subExps = { "x", "y", "z", "t", "v", "x * y", "z * t",
               "v + x * y", "v + x * y + z * t" };

            for (const auto& elem : subExps) {
                assignStorage.storeSubExpsAssignMap(1, lhs, elem);
            }

            std::list <PKBEntityPair> lst = assignStorage.getPatternAssignLhsSynRhsExp("v + x * y + z * t", true);
            auto l_front = lst.begin();

            PKBEntityPair p = *l_front;
            Assert::IsTrue(p.lhs.val == "1");
            Assert::IsFalse(p.lhs.val == "v + x * y + z * t");
            Assert::IsTrue(p.rhs.val == "x");
            Assert::IsFalse(p.rhs.val == "1");

            std::list <PKBEntityPair> lst2 = assignStorage.getPatternAssignLhsSynRhsExp("k", true);
            Assert::IsTrue(lst2.empty());
            
            std::list <PKBEntityPair> lst3 = assignStorage.getPatternAssignLhsSynRhsExp("v + x * y", false);

            auto l_front2 = lst3.begin();

            PKBEntityPair p2 = *l_front2;
            Assert::IsTrue(p2.lhs.val == "1");
            Assert::IsFalse(p2.lhs.val == "v + x * y");
            Assert::IsTrue(p2.rhs.val == "x");
            Assert::IsFalse(p2.rhs.val == "1");

            std::list <PKBEntityPair> lst4 = assignStorage.getPatternAssignLhsSynRhsExp("k", false);
            Assert::IsTrue(lst4.empty());

            std::list <PKBEntityPair> lst5 = assignStorage.getPatternAssignLhsSynRhsExp("x * y + z * t", false);
            Assert::IsTrue(lst5.empty());
        };

        TEST_METHOD(getPatternAssignLhsSynRhsExpMultipleTest) {
            // Store FullExp
            std::string lhs = "x";
            std::string rhs = "v + x * y + z * t";
            assignStorage.storeFullExpAssignMap(1, lhs, rhs);

            std::string lhs2 = "x";
            std::string rhs2 = "y + u * t";
            assignStorage.storeFullExpAssignMap(2, lhs2, rhs2);

            // Store SubExps
            std::unordered_set<std::string> subExps = { "x", "y", "z", "t", "v", "x * y", "z * t",
               "v + x * y", "v + x * y + z * t" };
            for (const auto& elem : subExps) {
                assignStorage.storeSubExpsAssignMap(1, lhs, elem);
            }

            std::unordered_set<std::string> subExps2 = { "y", "u", "t", "u * t", "y + u * t" };
            for (const auto& elem2 : subExps2) {
                assignStorage.storeSubExpsAssignMap(2, lhs, elem2);
            }
            
            std::list <PKBEntityPair> lst = assignStorage.getPatternAssignLhsSynRhsExp("v + x * y + z * t", true);
            Assert::IsTrue(lst.size() == 1);

            lst = assignStorage.getPatternAssignLhsSynRhsExp("v + x * y", false);
            Assert::IsTrue(lst.size() == 1);
            
            lst = assignStorage.getPatternAssignLhsSynRhsExp("z * t", true);
            Assert::IsTrue(lst.size() == 0);

            lst = assignStorage.getPatternAssignLhsSynRhsExp("y + u * t", true);
            Assert::IsTrue(lst.size() == 1);

            lst = assignStorage.getPatternAssignLhsSynRhsExp("x", false);
            Assert::IsTrue(lst.size() == 1);

            lst = assignStorage.getPatternAssignLhsSynRhsExp("t", false);
            Assert::IsTrue(lst.size() == 2);

        };


        TEST_METHOD(getPatternAssignLhsWildcardRhsWildcardTest) {
            std::string lhs1 = "x";
            std::string lhs2 = "y";
            std::string lhs3 = "z";
            std::string rhs1 = "v + x * y + z * t";
            std::string rhs2 = "v + x * y + z * t";
            std::string rhs3 = "v + x * y + z * t";
            assignStorage.storeFullExpAssignMap(1, lhs1, rhs1);
            assignStorage.storeFullExpAssignMap(2, lhs2, rhs2);
            assignStorage.storeFullExpAssignMap(3, lhs3, rhs3);

            std::list <PKBEntity> lst = assignStorage.getPatternAssignLhsWildcardRhsWildcard();

            auto l_front = lst.begin();
            PKBEntity p = *l_front;

            Assert::IsTrue(lst.size() == 3);
            Assert::IsFalse(lst.size() == 0);
        };

        TEST_METHOD(getPatternAssignLhsWildcardRhsExpTest) {
            // Store FullExp
            std::string lhs = "x";
            std::string rhs = "v + x * y + z * t";
            assignStorage.storeFullExpAssignMap(1, lhs, rhs);

            // Store SubExps
            std::unordered_set<std::string> subExps = { "x", "y", "z", "t", "v", "x * y", "z * t",
               "v + x * y", "v + x * y + z * t" };
            for (const auto& elem : subExps) {
                assignStorage.storeSubExpsAssignMap(1, lhs, elem);
            }

            std::list <PKBEntity> lst = assignStorage.getPatternAssignLhsWildcardRhsExp("v", false);
            Assert::IsTrue(lst.size() == 1);

            std::list <PKBEntity> lst4 = assignStorage.getPatternAssignLhsWildcardRhsExp("v", true);
            Assert::IsTrue(lst4.size() == 0);

            std::list <PKBEntity> lst2 = assignStorage.getPatternAssignLhsWildcardRhsExp("x", true);
            Assert::IsTrue(lst2.empty());

            std::list <PKBEntity> lst3 = assignStorage.getPatternAssignLhsWildcardRhsExp("v + x * y + z * t", true);
            Assert::IsTrue(lst3.size() == 1);
        };

        TEST_METHOD(getPatternAssignLhsConstRhsWildcardTest) {
            // Store FullExp
            std::string lhs = "x";
            std::string rhs = "v + x * y + z * t";
            std::string rhs2 = "x + 2";

            assignStorage.storeFullExpAssignMap(1, lhs, rhs);
            assignStorage.storeFullExpAssignMap(2, lhs, rhs2);

            std::list <PKBEntity> lst = assignStorage.getPatternAssignLhsConstRhsWildcard("x");
            auto l_front = lst.begin();

            PKBEntity p = *l_front;
            Assert::IsTrue(p.val == "1");
            Assert::IsTrue(lst.size() == 2);
        };

        TEST_METHOD(getPatternAssignLhsConstRhsExpTest) {
            // Store FullExp
            std::string lhs = "x";
            std::string rhs = "v + x * y + z * t";
            assignStorage.storeFullExpAssignMap(1, lhs, rhs);

            // Store SubExps
            std::unordered_set<std::string> subExps = { "x", "y", "z", "t", "v", "x * y", "z * t",
               "v + x * y", "v + x * y + z * t" };
            for (const auto& elem : subExps) {
                assignStorage.storeSubExpsAssignMap(1, lhs, elem);
            }

            std::list <PKBEntity> lst = assignStorage.getPatternAssignLhsConstRhsExp("x", "v + x * y", false);
            Assert::IsTrue(lst.size() == 1);

            std::list <PKBEntity> lst2 = assignStorage.getPatternAssignLhsConstRhsExp("x", "v", true);
            Assert::IsTrue(lst2.empty());

            std::list <PKBEntity> lst3 = assignStorage.getPatternAssignLhsConstRhsExp("x", "v + x * y + z * t", true);
            Assert::IsTrue(lst3.size() == 1);

            std::list <PKBEntity> lst4 = assignStorage.getPatternAssignLhsConstRhsExp("x", "v", false);
            Assert::IsTrue(lst4.size() == 1);
        };
        
        TEST_METHOD(getPatternAssignLhsConstRhsExpMultipleTest) {
            // Store FullExp
            std::string lhs = "x";
            std::string rhs = "v + x * y + z * t";
            assignStorage.storeFullExpAssignMap(1, lhs, rhs);

            std::string lhs2 = "y";
            std::string rhs2 = "y + u * t";
            assignStorage.storeFullExpAssignMap(2, lhs2, rhs2);

            // Store SubExps
            std::unordered_set<std::string> subExps = { "x", "y", "z", "t", "v", "x * y", "z * t",
               "v + x * y", "v + x * y + z * t" };
            for (const auto& elem : subExps) {
                assignStorage.storeSubExpsAssignMap(1, lhs, elem);
            }

            std::unordered_set<std::string> subExps2 = { "y", "u", "t", "u * t", "y + u * t" };
            for (const auto& elem2 : subExps2) {
                assignStorage.storeSubExpsAssignMap(2, lhs2, elem2);
            }

            std::list <PKBEntity> lst = assignStorage.getPatternAssignLhsConstRhsExp("x", "v + x * y + z * t", true);
            Assert::IsTrue(lst.size() == 1);

            lst = assignStorage.getPatternAssignLhsConstRhsExp("x", "v + x * y", false);
            Assert::IsTrue(lst.size() == 1);

            lst = assignStorage.getPatternAssignLhsConstRhsExp("x", "z * t", true);
            Assert::IsTrue(lst.size() == 0);

            lst = assignStorage.getPatternAssignLhsConstRhsExp("y", "y + u * t", true);
            Assert::IsTrue(lst.size() == 1);

            lst = assignStorage.getPatternAssignLhsConstRhsExp("x", "x", false);
            Assert::IsTrue(lst.size() == 1);

            lst = assignStorage.getPatternAssignLhsConstRhsExp("y", "t", false);
            Assert::IsTrue(lst.size() == 1);

            lst = assignStorage.getPatternAssignLhsConstRhsExp("x", "t", false);
            Assert::IsTrue(lst.size() == 1);
        };
    };

};

