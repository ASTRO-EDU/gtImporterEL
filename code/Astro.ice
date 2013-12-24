module Astro 
{
sequence<float> agileEvt;
const int agileEvtSize = 6;

sequence<double> agileLog;
const int agileLogSize = 13;

struct agileEvtValue
{
float theta;
float phi;
float ra;
float dec;
int energy;
float phearth;
byte evstatus;
};

sequence<agileEvtValue> agileEvtValueSeq;

struct agileEvtKey
{
double time;
double ra;
double dec;
double energy;
double theta;
double evstatus;
};

struct agileLogKey
{
double time;
double livetime;
double logStatus;
double mode;
double phase;
};

};
