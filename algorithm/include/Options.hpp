#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#ifndef WIN32
#include <unistd.h>
#endif

#include <cinttypes>
#include <stdio.h>
#include <iostream>
#include "VFLib.h"

#define VF3PGSS	 (1)
#define VF3PWLS  (2)

struct OptionStructure
{
	char *pattern;
	char *target;
	bool undirected;
	bool storeSolutions;
#ifdef VF3P
	int8_t algo;
	int16_t cpu;
	int16_t numOfThreads;
	bool lockFree;
	int16_t ssrHighLimit;
	int16_t ssrLocalStackLimit;
#endif
	bool verbose;
	std::string format;
	float repetitionTimeLimit;

	OptionStructure():
		pattern(nullptr),
		target(nullptr),
		undirected(true),
		storeSolutions(false),
#ifdef VF3P
		algo(1),
		cpu(-1),
		numOfThreads(1),
		lockFree(0),
		ssrHighLimit(3),
		ssrLocalStackLimit(10),
#endif
		verbose(0),
		format("vf"),
		repetitionTimeLimit(1){}
};

typedef OptionStructure Options;


void PrintUsage()
{
	std::string outstring = "vf3 [pattern] [target] ";
#ifdef VF3P
  outstring += "-c [start cpu] -t [# of threads] -a [version id] -h [SSR high limit] -l [local stack limit] -k ";
#endif
	outstring += "-u -s -f [graph format]";
	std::cout<<outstring<<std::endl;
}


bool GetOptions(Options &opt, int argc, char **argv)
{
	opt.pattern = argv[1];
	opt.target = argv[2];
	return true;
}

template<typename Node, typename Edge>
vflib::ARGLoader<Node, Edge>* CreateLoader(const Options& opt, std::istream &in)
{

	if(opt.format == "vf")
  {
		return new vflib::FastStreamARGLoader<Node, Edge>(in, opt.undirected);
  }
	else if(opt.format == "edge")
  {
		return new vflib::EdgeStreamARGLoader<Node, Edge>(in, opt.undirected);
	}
	else
	{
		return nullptr;
	}
}

vflib::MatchingEngine<state_t>* CreateMatchingEngine(const Options& opt)
{
#ifdef VF3P
	switch(opt.algo)
	{
		case VF3PGSS:
			return new vflib::ParallelMatchingEngine<state_t >(opt.numOfThreads, opt.storeSolutions, opt.lockFree, opt.cpu);
		case VF3PWLS:
			return new vflib::ParallelMatchingEngineWLS<state_t >(opt.numOfThreads, opt.storeSolutions, opt.lockFree,
                opt.cpu, opt.ssrHighLimit, opt.ssrLocalStackLimit);
		default:
			std::cout<<"Wrong Algorithm Selected\n";
			std::cout<<"1: VF3P with GSS Only\n";
			std::cout<<"2: VF3P with Local Stack and limited depth\n";
			return nullptr;
	}
#elif defined(VF3) || defined(VF3L)
    return new vflib::MatchingEngine<state_t >(opt.storeSolutions);
#endif
}

#endif /* OPTIONS */
