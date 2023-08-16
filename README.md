# rv-sample-gens
This is a repo for the code used to generate samples meant to mimic polymorphic malware behavior. These samples serve as the basis for testing against a DTrace monitor generated using graphviz2dtrace.

There are 2 Files:
- generator.py: a python script which uses the virus.cpp file to generate different variations of the sample file
- malware.cpp: the actual virus body, which performs a sequence of actions common to malicious software.