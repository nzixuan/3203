@echo off
title final all test

@REM ..\..\..\Code13\Debug\AutoTester affects_source.txt affects_query.txt .\results\out-affects.xml

..\..\..\Code13\Debug\AutoTester source_nesting.txt Follows_1.txt .\results\out-follows-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt FollowsStar_1.txt .\results\out-follows-star-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt Parent_1.txt .\results\out-parent-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt ParentStar_1.txt .\results\out-parent-star-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt Modifies_1.txt .\results\out-modifies-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt Uses_1.txt .\results\out-uses-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt Calls_1.txt .\results\out-calls-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt CallsStar_1.txt .\results\out-calls-star-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt PatternAssign_1.txt .\results\out-pattern-assign-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt PatternIf_1.txt .\results\out-pattern-if-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt PatternWhile_1.txt .\results\out-pattern-while-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt Affects_1.txt .\results\out-affects-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt AffectsStar_1.txt .\results\out-affects-star-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt Next_1.txt .\results\out-next-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt NextStar_1.txt .\results\out-next-star-1.xml
..\..\..\Code13\Debug\AutoTester source_nesting.txt with_query_1.txt .\results\out-with-1.xml

..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt Follows_2.txt .\results\out-follows-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt FollowsStar_2.txt .\results\out-follows-star-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt Parent_2.txt .\results\out-parent-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt ParentStar_2.txt .\results\out-parent-star-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt Modifies_2.txt .\results\out-modifies-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt Uses_2.txt .\results\out-uses-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt Calls_2.txt .\results\out-calls-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt CallsStar_2.txt .\results\out-calls-star-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt NoCondition_2.txt .\results\out-no-condition-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt PatternAssign_2.txt .\results\out-pattern-assign-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt PatternIf_2.txt .\results\out-pattern-if-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt PatternWhile_2.txt .\results\out-pattern-while-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt Affects_2.txt .\results\out-affects-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt AffectsStar_2.txt .\results\out-affects-star-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt Next_2.txt .\results\out-next-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt NextStar_2.txt .\results\out-next-star-2.xml
..\..\..\Code13\Debug\AutoTester source_alphanumeric.txt with_query_2.txt .\results\out-with-2.xml


echo FINISHED
pause