#include "core.hpp"
#include <cassert>
int main(){sumi::Funnel f({"view","signup","purchase"},2);assert(!f.ingest("a","purchase",1));assert(f.ingest("a","view",2));assert(!f.ingest("a","view",3));assert(f.ingest("a","signup",4));assert(!f.ingest("a","purchase",2));assert(f.ingest("a","purchase",5));assert(f.ingest("b","view",1));assert(!f.ingest("c","view",1));assert(f.counts()[0]==2&&f.counts()[1]==1&&f.counts()[2]==1);}
