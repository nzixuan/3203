@echo off
title affects test

..\..\..\Code13\Debug\AutoTester affects_source.txt affects_queries.txt out-affects-1.xml
..\..\..\Code13\Debug\AutoTester source-nesting.txt affects_queries_2.txt out-affects-2.xml
..\..\..\Code13\Debug\AutoTester source-nesting.txt affects_star_queries.txt out-affects-star.xml

echo FINISHED
pause