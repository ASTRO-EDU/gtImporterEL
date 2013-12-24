// **********************************************************************
//
// Copyright (c) 2003-2013 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.5.0
//
// <auto-generated>
//
// Generated from file `Astro.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#ifndef __code_Astro_h__
#define __code_Astro_h__

#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/StreamHelpers.h>
#include <IceUtil/ScopedArray.h>
#include <IceUtil/Optional.h>
#include <Ice/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 305
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 0
#       error Ice patch level mismatch!
#   endif
#endif

namespace Astro
{

typedef ::std::vector< ::Ice::Float> agileEvt;

const ::Ice::Int agileEvtSize = 6;

typedef ::std::vector< ::Ice::Double> agileLog;

const ::Ice::Int agileLogSize = 13;

struct agileEvtValue
{
    ::Ice::Float theta;
    ::Ice::Float phi;
    ::Ice::Float ra;
    ::Ice::Float dec;
    ::Ice::Int energy;
    ::Ice::Float phearth;
    ::Ice::Byte evstatus;
};

typedef ::std::vector< ::Astro::agileEvtValue> agileEvtValueSeq;

struct agileEvtKey
{
    ::Ice::Double time;
    ::Ice::Double ra;
    ::Ice::Double dec;
    ::Ice::Double energy;
    ::Ice::Double theta;
    ::Ice::Double evstatus;
};

struct agileLogKey
{
    ::Ice::Double time;
    ::Ice::Double livetime;
    ::Ice::Double logStatus;
    ::Ice::Double mode;
    ::Ice::Double phase;
};

}

namespace Ice
{
template<>
struct StreamableTraits< ::Astro::agileEvtValue>
{
    static const StreamHelperCategory helper = StreamHelperCategoryStruct;
    static const int minWireSize = 25;
    static const bool fixedLength = true;
};

template<class S>
struct StreamWriter< ::Astro::agileEvtValue, S>
{
    static void write(S* __os, const ::Astro::agileEvtValue& v)
    {
        __os->write(v.theta);
        __os->write(v.phi);
        __os->write(v.ra);
        __os->write(v.dec);
        __os->write(v.energy);
        __os->write(v.phearth);
        __os->write(v.evstatus);
    }
};

template<class S>
struct StreamReader< ::Astro::agileEvtValue, S>
{
    static void read(S* __is, ::Astro::agileEvtValue& v)
    {
        __is->read(v.theta);
        __is->read(v.phi);
        __is->read(v.ra);
        __is->read(v.dec);
        __is->read(v.energy);
        __is->read(v.phearth);
        __is->read(v.evstatus);
    }
};

template<>
struct StreamableTraits< ::Astro::agileEvtKey>
{
    static const StreamHelperCategory helper = StreamHelperCategoryStruct;
    static const int minWireSize = 48;
    static const bool fixedLength = true;
};

template<class S>
struct StreamWriter< ::Astro::agileEvtKey, S>
{
    static void write(S* __os, const ::Astro::agileEvtKey& v)
    {
        __os->write(v.time);
        __os->write(v.ra);
        __os->write(v.dec);
        __os->write(v.energy);
        __os->write(v.theta);
        __os->write(v.evstatus);
    }
};

template<class S>
struct StreamReader< ::Astro::agileEvtKey, S>
{
    static void read(S* __is, ::Astro::agileEvtKey& v)
    {
        __is->read(v.time);
        __is->read(v.ra);
        __is->read(v.dec);
        __is->read(v.energy);
        __is->read(v.theta);
        __is->read(v.evstatus);
    }
};

template<>
struct StreamableTraits< ::Astro::agileLogKey>
{
    static const StreamHelperCategory helper = StreamHelperCategoryStruct;
    static const int minWireSize = 40;
    static const bool fixedLength = true;
};

template<class S>
struct StreamWriter< ::Astro::agileLogKey, S>
{
    static void write(S* __os, const ::Astro::agileLogKey& v)
    {
        __os->write(v.time);
        __os->write(v.livetime);
        __os->write(v.logStatus);
        __os->write(v.mode);
        __os->write(v.phase);
    }
};

template<class S>
struct StreamReader< ::Astro::agileLogKey, S>
{
    static void read(S* __is, ::Astro::agileLogKey& v)
    {
        __is->read(v.time);
        __is->read(v.livetime);
        __is->read(v.logStatus);
        __is->read(v.mode);
        __is->read(v.phase);
    }
};

}

#endif
