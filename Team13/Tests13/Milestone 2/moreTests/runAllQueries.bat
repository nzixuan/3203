@echo off
title other tests

..\..\..\Code13\Debug\AutoTester boolean_source.txt boolean_queries.txt out-boolean.xml

..\..\..\Code13\Debug\AutoTester calls_source.txt calls_queries.txt out-calls.xml
..\..\..\Code13\Debug\AutoTester complex_calls_graph_source.txt complex_calls_graph_queries.txt out-complex_calls_graph.xml
..\..\..\Code13\Debug\AutoTester complex_callst_graph_source.txt complex_callst_graph_queries.txt out-complex_callst_graph.xml

..\..\..\Code13\Debug\AutoTester multiClauses_source.txt multiClauses_queries.txt out-multiClauses.xml

..\..\..\Code13\Debug\AutoTester newFeatures_source.txt newFeatures_queries.txt out-newFeatures.xml

..\..\..\Code13\Debug\AutoTester next_source.txt next_queries.txt out-next.xml
..\..\..\Code13\Debug\AutoTester nextt_source.txt nextt_queries.txt out-nextt.xml

..\..\..\Code13\Debug\AutoTester patternAssign_source.txt patternAssign_queries.txt out-patternAssign.xml

echo FINISHED
pause