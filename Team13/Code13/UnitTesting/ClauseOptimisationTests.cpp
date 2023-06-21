#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(ClauseOptimisationTests)
	{
	public:
		Tokenizer tokenizer;
		ClauseManager actualCm;
		ClauseManager expectedCm;
		ClauseArgs ca;
		TEST_METHOD_INITIALIZE(initializeClauses) {
			// Reinitalize the query after every method
			actualCm = ClauseManager{};
			expectedCm = ClauseManager{};
			ca = ClauseArgs{};
		}

		TEST_METHOD(testClausesSplitting) {
			/*
			* We start with a single group
			* 1. Clause with no synonym
			* 2. Clause with unselected synonym
			* 3. Clause with unselected synonym
			* 3. Clause with selected synonym
			*/

			// Clauses are listed in order which they should be run
			// with 1 = 2
			ca.addRelArg(number);
			ca.addRelArg(number2);
			With clause1{ ca, WithRefType::Integer };

			ca = ClauseArgs{};
			ca.addRelArg(readSyn);
			ca.addRelArg(wildcard);
			// Modifies(_,v)
			SuchThat clause2{ ca, RelRefType::Modifies };
			ca = ClauseArgs{};
			ca.addRelArg(assignSyn);
			ca.addRelArg(assignSyn);
			// Follows(r,_)
			SuchThat clause3{ ca, RelRefType::Follows };
			ca = ClauseArgs{};
			ca.addRelArg(wildcard);
			ca.addRelArg(varSyn);
			// Affects*(a,a)
			SuchThat clause4{ ca, RelRefType::AffectsT };

			expectedCm.addClause(clause1);
			expectedCm.addClause(clause2);
			expectedCm.addClause(clause3);
			expectedCm.addClause(clause4);
			expectedCm.finalize({ assignSyn });

			actualCm.addClause(clause4);
			actualCm.addClause(clause3);
			actualCm.addClause(clause2);
			actualCm.addClause(clause1);
			actualCm.finalize({ assignSyn });

			Assert::IsTrue(actualCm == expectedCm);

			// 3 separate groups
			for (int i = 0; i < 4; i++) {
				actualCm.nextGroup();
			}
			Assert::IsTrue(actualCm.hasNoClauses());
		}
		TEST_METHOD(testClausesNoSyn) {
			/* 
			* 1. prioritise with clause
			* 2. prioritise low results: Follows
			* 3. pattern = transitive Follows*
			* 4. Affects is last
			*/
			
			// Clauses are listed in order which they should be run
			// with 1 = 2
			ca.addRelArg(number);
			ca.addRelArg(number2);
			With clause1{ca, WithRefType::Integer};

			ca = ClauseArgs{};
			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			// Follows(_,_)
			SuchThat clause2{ca, RelRefType::Follows};
			// Follows*(_,_)
			SuchThat clause3{ca, RelRefType::FollowsT};
			// Affects*(_,_)
			SuchThat clause4{ca, RelRefType::AffectsT};
		
			actualCm.addClause(clause4);
			actualCm.addClause(clause1);
			actualCm.addClause(clause2);
			actualCm.addClause(clause3);
			actualCm.finalize({ assignSyn });

			Group expectedGroup;
			expectedGroup.addClause(clause1);
			expectedGroup.addClause(clause2);
			expectedGroup.addClause(clause3);
			expectedGroup.addClause(clause4);

			// Should have a single group only
			Assert::IsTrue(actualCm.nextGroup() == expectedGroup);
			Assert::IsTrue(actualCm.hasNoClauses());
		}

		TEST_METHOD(testClausesSynAllSameGroup) {
			/*
			* All clauses with synonyms all share a synonym with at least one other clause
			* 1. No synonym
			* 2. Connected with 3 and 4
			* 3. Connected with 5
			* 4. Connected with 2
			* 5. Connected with 3
			*/

			// Clauses are listed in order which they should be run
			ca.addRelArg(number);
			ca.addRelArg(number2);
			With clause1{ ca, WithRefType::Integer };

			ca = ClauseArgs{};
			ca.addRelArg(AttrRef{ readSyn, AttrName::StmtNo });
			ca.addRelArg(number);
			With clause2{ ca, WithRefType::Integer };
			ca = ClauseArgs{};
			ca.addRelArg(readSyn);
			ca.addRelArg(SourceArg{ "var" });
			SuchThat clause3{ ca, RelRefType::Modifies };
			ca = ClauseArgs{};
			ca.addRelArg(readSyn);
			ca.addRelArg(assignSyn);
			SuchThat clause4{ ca, RelRefType::Follows };
			ca = ClauseArgs{};
			ca.addRelArg(assignSyn);
			ca.addRelArg(assignSyn);
			SuchThat clause5{ ca, RelRefType::AffectsT };

			actualCm.addClause(clause3);
			actualCm.addClause(clause5);
			actualCm.addClause(clause1);
			actualCm.addClause(clause4);
			actualCm.addClause(clause2);
			actualCm.finalize({ assignSyn });

			Group group1;
			Group group2;

			group1.addClause(clause1);
			group2.addClause(clause2);
			group2.addClause(clause3);
			group2.addClause(clause4);
			group2.addClause(clause5);

			std::vector<Group> expectedGroups{ group1, group2 };
			// 2 groups expected.
			for (int i = 0; i < 2; i++) {
				Assert::IsTrue(actualCm.nextGroup() == expectedGroups[i]);
			}
			Assert::IsTrue(actualCm.hasNoClauses());
		}

		TEST_METHOD(testClausesSynNotAllSameGroup) {
			/*
			* Two separate groups of groups with synonyms
			* 1. No synonym
			* 2. Connected with 3 
			* 3. Connected with 2
			* 4. Connected with 5
			* 5. Connected with 4
			* Only synonym in 2,3 appears in select
			*/

			// Clauses are listed in order which they should be run
			ca.addRelArg(number);
			ca.addRelArg(number2);
			With clause1{ ca, WithRefType::Integer };

			ca = ClauseArgs{};
			ca.addRelArg(assignSyn);
			ca.addRelArg(SourceArg{ "var" });
			SuchThat clause2{ ca, RelRefType::Modifies };
			ca = ClauseArgs{};
			ca.addRelArg(assignSyn);
			ca.addRelArg(assignSyn);
			SuchThat clause3{ ca, RelRefType::AffectsT };
			ca = ClauseArgs{};
			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			Pattern clause4{ ca, ifSyn };
			ca = ClauseArgs{};
			ca.addRelArg(ifSyn);
			ca.addRelArg(printSyn);
			SuchThat clause5{ ca, RelRefType::Parent };

			actualCm.addClause(clause5);
			actualCm.addClause(clause2);
			actualCm.addClause(clause1);
			actualCm.addClause(clause3);
			actualCm.addClause(clause4);
			actualCm.finalize({ assignSyn });

			Group group1;
			Group group2;
			Group group3;

			group1.addClause(clause1);
			group2.addClause(clause2);
			group2.addClause(clause3);
			group3.addClause(clause4);
			group3.addClause(clause5);

			std::vector<Group> expectedGroups{ group1, group2, group3 };
			// 3 groups expected.
			for (int i = 0; i < 3; i++) {
				Assert::IsTrue(actualCm.nextGroup() == expectedGroups[i]);
			}
			Assert::IsTrue(actualCm.hasNoClauses());
		}

		TEST_METHOD(testClausesLectureOptimisation) {
			/*
			* Uses the example provided in lecture.
			* 1. No synonym
			* 2. Connected with 3
			* 3. Connected with 2
			* 4. Connected with 5
			* 5. Connected with 4
			* Only synonym in 2,3 appears in select
			*/
			Synonym a{ "a", EntityType::Assign };
			Synonym a1{ "a1", EntityType::Assign };
			Synonym a2{ "a2", EntityType::Assign };
			Synonym s1{ "s1", EntityType::Stmt };
			Synonym s2{ "s2", EntityType::Stmt };
			Synonym s3{ "s3", EntityType::Stmt };
			Synonym v1{ "v1", EntityType::Variable };
			Synonym v2{ "v2", EntityType::Variable };

			// Clauses are listed in order which they should be run
			// Uses(5, "y")
			ca = ClauseArgs{};
			ca.addRelArg(Number{"5"});
			ca.addRelArg(SourceArg{"y"});
			SuchThat clause1{ ca, RelRefType::Uses };

			// Uses(s1, v1)
			ca = ClauseArgs{};
			ca.addRelArg(s1);
			ca.addRelArg(v1);
			SuchThat clause2{ ca, RelRefType::Uses };

			// Affects(a1, a2)
			ca = ClauseArgs{};
			ca.addRelArg(a1);
			ca.addRelArg(a2);
			SuchThat clause3{ ca, RelRefType::Affects };

			// with a1.stmt# = 20
			ca = ClauseArgs{};
			ca.addRelArg(AttrRef{a1, AttrName::StmtNo});
			ca.addRelArg(Number{"20"});
			With clause4{ ca, WithRefType::Integer };

			// Parent(s1, s2)
			ca = ClauseArgs{};
			ca.addRelArg(s1);
			ca.addRelArg(s2);
			SuchThat clause5{ ca, RelRefType::Parent };

			// pattern a2("a", _)
			ca = ClauseArgs{};
			ca.addRelArg(SourceArg{ "a" });
			ca.addRelArg(wildcard);
			Pattern clause6{ ca, a2 };

			// Next(s2, s3)
			ca = ClauseArgs{};
			ca.addRelArg(s2);
			ca.addRelArg(s3);
			SuchThat clause7{ ca, RelRefType::Next };

			// Modifies(s1, "x")
			ca = ClauseArgs{};
			ca.addRelArg(s1);
			ca.addRelArg(SourceArg{"x"});
			SuchThat clause8{ ca, RelRefType::Modifies };

			// Modifies(a, v2)
			ca = ClauseArgs{};
			ca.addRelArg(a);
			ca.addRelArg(v2);
			SuchThat clause9{ ca, RelRefType::Modifies };

			// pattern a(v1, "x")
			ca = ClauseArgs{};
			ca.addRelArg(v1);
			ca.addRelArg(SourceArg{ "x" });
			Pattern clause10{ ca, a };
			
			// Edited from lecture. Only one of the groups has these select syns.
			std::set<Synonym> selectSyns{ a1, a2};

			actualCm.addClause(clause1);
			actualCm.addClause(clause2);
			actualCm.addClause(clause3);
			actualCm.addClause(clause4);
			actualCm.addClause(clause5);
			actualCm.addClause(clause6);
			actualCm.addClause(clause7);
			actualCm.addClause(clause8);
			actualCm.addClause(clause9);
			actualCm.addClause(clause10);
			actualCm.finalize(selectSyns);

			Group group1;
			Group group2;
			Group group3;

			group1.addClause(clause1);
			group2.addClause(clause4);
			group2.addClause(clause3);
			group2.addClause(clause6);
			group3.addClause(clause8);
			group3.addClause(clause2);
			group3.addClause(clause10);
			group3.addClause(clause9);
			group3.addClause(clause5);
			group3.addClause(clause7);

			std::vector<Group> expectedGroups{ group1, group2, group3 };
			// 3 groups expected.
			for (int i = 0; i < 3; i++) {
				Assert::IsTrue(actualCm.nextGroup() == expectedGroups[i]);
			}
			Assert::IsTrue(actualCm.hasNoClauses());
		}
	};
}