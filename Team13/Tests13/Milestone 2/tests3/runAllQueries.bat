@echo off
title system test 3 queries

..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt Calls.txt out-Calls.xml
..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt CallsStar.txt out-CallsStar.xml

..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt Follows.txt out-Follows.xml
..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt FollowsStar.txt out-FollowsStar.xml

..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt Modifies.txt out-Modifies.xml
..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt Uses.txt out-Uses.xml

..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt Next.txt out-Next.xml
..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt NextStar.txt out-NextStar.xml

..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt NoCondition.txt out-NoCondition.xml

..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt Parent.txt out-Parent.xml
..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt ParentStar.txt out-ParentStar.xml

..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt Boolean.txt out-Boolean.xml

..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt PatternWhile.txt out-PatternWhile.xml
..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt PatternIf.txt out-PatternIf.xml
..\..\..\Code13\Debug\AutoTester sourceAlphanumeric.txt PatternAssign.txt out-PatternAssign.xml

echo FINISHED