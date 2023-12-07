#pragma once

#include <iostream>
#include <vector>

#define T1N 0
#define T2N 1
#define T3N 2
#define T1P1 3
#define T1P2 4
#define T1P3 5
#define T1P4 6
#define T1P5 7
#define T1P6 8
#define T2P1 9
#define T2P2 10
#define T2P3 11
#define T2P4 12
#define T2P5 13
#define T2P6 14
#define T3P1 15
#define T3P2 16
#define T3P3 17
#define T3P4 18
#define T3P5 19
#define T3P6 20
#define T1T1 21
#define T1T2 22
#define T1T3 23
#define T2T2 24
#define T2T3 25
#define T3T3 26
#define AN 27
#define AP1 28
#define AP2 29
#define AP3 30
#define AP4 31
#define AP5 32
#define AP6 33
#define AT1 34
#define AT2 35
#define AT3 36
#define AA 37

bool hasA(int first, int second, int third, int fourth);

bool hasT1(int first, int second, int third, int fourth);

bool hasT2(int first, int second, int third, int fourth);

bool hasT3(int first, int second, int third, int fourth);

bool hasN(int first, int second, int third, int fourth);

bool hasP1(int first, int second, int third, int fourth);

bool hasP2(int first, int second, int third, int fourth);

bool hasP3(int first, int second, int third, int fourth);

bool hasP4(int first, int second, int third, int fourth);

bool hasP5(int first, int second, int third, int fourth);

bool hasP6(int first, int second, int third, int fourth);

int determine(int first, int second, int third, int fourth);

std::string convert(long long int number);

std::vector<int> getExample(int number);

std::string convertRepresentation(long long int representation);
