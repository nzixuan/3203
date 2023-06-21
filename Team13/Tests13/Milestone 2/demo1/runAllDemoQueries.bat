@echo off
title Sprint 3 demo

..\..\..\Code13\Debug\AutoTester demoModifiesUsesSource.txt demoModifiesUsesQuery.txt demo-modifies-uses-for-procedure.xml
..\..\..\Code13\Debug\AutoTester demoCallsSource.txt demoCallsQuery.txt demo-calls.xml
..\..\..\Code13\Debug\AutoTester demoBugfixSource.txt demoBugfixQuery.txt demo-bugfix.xml

echo FINISHED
