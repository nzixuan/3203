@echo off
title system test 2 queries

..\..\..\Code13\Debug\AutoTester source-nesting.txt Calls.txt out-Calls.xml
..\..\..\Code13\Debug\AutoTester source-nesting.txt CallsStar.txt out-CallsStar.xml

..\..\..\Code13\Debug\AutoTester source-nesting.txt Follows.txt out-Follows.xml
..\..\..\Code13\Debug\AutoTester source-nesting.txt FollowsStar.txt out-FollowsStar.xml

..\..\..\Code13\Debug\AutoTester source-nesting.txt Modifies.txt out-Modifies.xml
..\..\..\Code13\Debug\AutoTester source-nesting.txt Uses.txt out-Uses.xml

..\..\..\Code13\Debug\AutoTester source-nesting.txt Next.txt out-Next.xml
..\..\..\Code13\Debug\AutoTester source-nesting.txt NextStar.txt out-NextStar.xml

..\..\..\Code13\Debug\AutoTester source-nesting.txt Parent.txt out-Parent.xml
..\..\..\Code13\Debug\AutoTester source-nesting.txt ParentStar.txt out-ParentStar.xml

..\..\..\Code13\Debug\AutoTester source-nesting.txt PatternAssign.txt out-PatternAssign.xml
..\..\..\Code13\Debug\AutoTester source-nesting.txt PatternIf.txt out-PatternIf.xml
..\..\..\Code13\Debug\AutoTester source-nesting.txt PatternWhile.txt out-PatternWhile.xml

..\..\..\Code13\Debug\AutoTester source-nesting.txt Tuples.txt out-Tuples.xml

echo FINISHED
pause