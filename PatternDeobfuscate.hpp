#pragma once
#include <hexrays.hpp>

struct ObfCompilerOptimizer : public optinsn_t
{
public:
	int idaapi func(mblock_t* blk, minsn_t* ins, int optflags);
	int pat_LogicAnd1(minsn_t* ins);
	int ObfCompilerOptimizer::Optimize(minsn_t* ins);
};
