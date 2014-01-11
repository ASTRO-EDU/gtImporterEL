/***************************************************************************
                          main.cpp
                             -------------------
    copyright            : (C) 2013 Montanati, Paganelli, Bulgarelli
    email                : bulgarelli@iasfbo.inaf.it, ,

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define SIMPLE_KEY

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <iomanip>
#include <string>
#include "InputFileFITS.h"

#include "AstroMap.h"
#include "DBAstro.h"
#include "Astro.h"
#include <Freeze/Freeze.h>

//file types
#define EVT 1
#define LOG 2

//columns of EVT file
#define EVT_TIME 0
#define EVT_PHI 2
#define EVT_RA 3
#define EVT_DEC 4
#define EVT_ENERGY 5
#define EVT_PH_EARTH 14
#define EVT_THETA 1
#define EVT_PHASE 15
#define EVT_EVSTATUS 13

//columsn of LOG file
#define LOG_TIME 0
#define LOG_PHASE 5
#define LOG_LIVETIME 39
#define LOG_LOG_STATUS 40
#define LOG_MODE 4
#define LOG_ATTITUDE_RA_Y 13
#define LOG_ATTITUDE_DEC_Y 14
#define LOG_EARTH_RA 24
#define LOG_EARTH_DEC 25
#define LOG_Q1 9
#define LOG_Q2 10
#define LOG_Q3 11
#define LOG_Q4 12

using namespace std;
using namespace qlbase;

///Import AGILE LOG and EVT files into Ice/Freeze/BDB
int main(int argc, char** argv) {

	cout << "gtImporterELice" << endl;

	if(argc == 1) {
		cerr << "Please, provide (1) the fits file to import (2 optional) the last line to be read" << endl;
		return 0;
	}

	string filename = argv[1];
	InputFileFITS* inputFF;


	//open input file FITS
	try{
		inputFF = new InputFileFITS();
		inputFF->open(filename);
		inputFF->moveToHeader(1); //CHECK base index AZ


		//check input file fits type: EVT or LOG
		int ncols = inputFF->getNCols();
		int nrows_start = 0;
		int nrows_end = inputFF->getNRows();
		if(argc == 3) {
			nrows_end = atoi(argv[2]);
		}
		int type;
		cout << nrows_end << endl;

		if(ncols == 19) type = EVT;
		if(ncols == 41) type = LOG;

		Ice::InitializationData initData;
		initData.properties = Ice::createProperties();
		initData.properties->load("config");

		// Initialize the Communicator.
		Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv, initData);

		// Create a Freeze database connection.
		Freeze::ConnectionPtr connection = Freeze::createConnection(communicator, "dbagilesimple");

		if(type == EVT) {
			//read all columns
			cout << "Read EVT file " << endl;
			std::vector<double> time = inputFF->read64f(EVT_TIME, nrows_start, nrows_end-1);
			std::vector<float> phi = inputFF->read32f(EVT_PHI, nrows_start, nrows_end-1);
			std::vector<float> ra = inputFF->read32f(EVT_RA, nrows_start, nrows_end-1);
			std::vector<float> dec = inputFF->read32f(EVT_DEC, nrows_start, nrows_end-1);
			std::vector<float> energy = inputFF->read32f(EVT_ENERGY, nrows_start, nrows_end-1);
			std::vector<float> ph_earth = inputFF->read32f(EVT_PH_EARTH, nrows_start, nrows_end-1);
			std::vector<float> theta = inputFF->read32f(EVT_THETA, nrows_start, nrows_end-1);
			std::vector<int16_t> phase = inputFF->read16i(EVT_PHASE, nrows_start, nrows_end-1);
			std::vector< std::vector<char> > status = inputFF->readString(EVT_EVSTATUS, nrows_start, nrows_end-1, 1);
			std::vector<int16_t> status2(nrows_end-nrows_start+1);

			for(uint32_t i  = 0; i<nrows_end; i++) {
				std::string evt;
				evt = &(status[i])[0];
				//cout << evt << endl;
				if(evt.compare("G") == 0) status2[i] = 0;
				if(evt.compare("L") == 0) status2[i] = 1;
				if(evt.compare("S") == 0) status2[i] = 2;

			}
			//write data into BDB


#ifdef COMPOSITE_KEY
			//Create the vector to store into BDB
			cout << "<agileEvtKey, double>
			Astro::agileEvt evt;
			Astro::agileEvtKey evtKey;
			AgileEvtMapComplex dbEvt(connection, "DBAgileEvtComplex");
#endif
#ifdef SIMPLE_KEY
			//Create the map
//			AgileEvtMap dbEvt(connection, "DBAgileEvt");
			DBAgileEvt dbEvt(connection, "DBAgileEvt");
			//Create the vector to store into BDB
			Astro::agileEvt evt;
#endif
#ifdef STRUCT_VALUE
			cout << "<double, agileEvtStruct>" << endl;
			AgileEvtMapStruct dbEvt(connection, "DBAgileEvtStruct");

#endif
			cout << "Start write into BDB" << endl;

			for(uint32_t i  = 0; i<nrows_end; i++) {
				//cout << setiosflags(ios::fixed) << std::setprecision(6) << (double) time[i] << " " << status2[i] << endl;

#ifdef COMPOSITE_KEY
				//Populate the vector
				evt.clear();
				evt.push_back((Ice::Float) status2[i]);
				evt.push_back((Ice::Float) phase[i]);
				evt.push_back((Ice::Float) theta[i]);
				evt.push_back((Ice::Float) ph_earth[i]);
				evt.push_back((Ice::Float) energy[i]);
				evt.push_back((Ice::Float) dec[i]);
				evt.push_back((Ice::Float) ra[i]);
				evt.push_back((Ice::Float) phi[i]);
				//evt.push_back((Ice::Double) time[i]);
				//Populate the key
				evtKey.time = time[i];
				evtKey.ra = ra[i];
				evtKey.dec = dec[i];
				evtKey.energy = energy[i];
				evtKey.theta = theta[i];
				evtKey.evstatus = status2[i];

				//Execute write
				dbEvt.insert(make_pair(evtKey, evt));
#endif

#ifdef SIMPLE_KEY
				evt.clear();
                                evt.push_back((Ice::Float) status2[i]);
                                evt.push_back((Ice::Float) phase[i]);
                                evt.push_back((Ice::Float) theta[i]);
                                evt.push_back((Ice::Float) ph_earth[i]);
                                evt.push_back((Ice::Float) energy[i]);
                                evt.push_back((Ice::Float) dec[i]);
                                evt.push_back((Ice::Float) ra[i]);
                                evt.push_back((Ice::Float) phi[i]);
				//Execute write
				dbEvt.insert(make_pair(time[i],evt));
				//cout << i << " ";
#endif

#ifdef STRUCT_VALUE
				Astro::agileEvtValue evt;
				evt.theta = theta[i];
				evt.phi = phi[i];
				evt.ra = ra[i];
				evt.dec = dec[i];
				evt.energy = energy[i];
				evt.phearth = ph_earth[i];
				evt.evstatus = status2[i];
				//Execute write
				dbEvt.insert(make_pair(time[i],evt));
				cout << i << " ";
#endif

			}
			cout << endl;

		}
		if(type == LOG) {
			//read all columns
			cout << "Read LOG file " << nrows_end << endl;
			std::vector<double> time = inputFF->read64f(LOG_TIME, nrows_start, nrows_end-1);
			std::vector<int16_t> phase = inputFF->read16i(LOG_PHASE, nrows_start, nrows_end-1);
			std::vector<float> livetime = inputFF->read32f(LOG_LIVETIME, nrows_start, nrows_end-1);
			std::vector<int16_t> log_status = inputFF->read16i(LOG_LOG_STATUS, nrows_start, nrows_end-1);
			std::vector<int16_t> mode = inputFF->read16i(LOG_MODE, nrows_start, nrows_end-1);
			std::vector<double> attitude_ra_y = inputFF->read64f(LOG_ATTITUDE_RA_Y, nrows_start, nrows_end-1);
			std::vector<double> attitude_dec_y = inputFF->read64f(LOG_ATTITUDE_DEC_Y, nrows_start, nrows_end-1);
			std::vector<double> log_earth_ra = inputFF->read64f(LOG_EARTH_RA, nrows_start, nrows_end-1);
			std::vector<double> log_earth_dec = inputFF->read64f(LOG_EARTH_DEC, nrows_start, nrows_end-1);
			std::vector<float> q1 = inputFF->read32f(LOG_Q1, nrows_start, nrows_end-1);
			std::vector<float> q2 = inputFF->read32f(LOG_Q2, nrows_start, nrows_end-1);
			std::vector<float> q3 = inputFF->read32f(LOG_Q3, nrows_start, nrows_end-1);
			std::vector<float> q4 = inputFF->read32f(LOG_Q4, nrows_start, nrows_end-1);

			//write data into BDB
			uint32_t count = 0;

			//Create the vector to store into BDB
			Astro::agileLog agileLog;

			//Create key
			Astro::agileLogKey key;

			//Create the map
			AgileLogMap DBLog(connection, "DBAgileLog");

			cout << "Start write into BDB" << endl;

			for(uint32_t i = 0; i<nrows_end; i++) {
				cout << setiosflags(ios::fixed) << std::setprecision(6) << (double) time[i] << " ";
				cout << attitude_ra_y[i] << endl;
				if(attitude_ra_y[i] != 0) count++;

				//Populate the vector
				agileLog.clear();
				agileLog.push_back((Ice::Double) q4[i]);
				agileLog.push_back((Ice::Double) q3[i]);
				agileLog.push_back((Ice::Double) q2[i]);
				agileLog.push_back((Ice::Double) q1[i]);
				agileLog.push_back((Ice::Double) log_earth_dec[i]);
				agileLog.push_back((Ice::Double) log_earth_ra[i]);
				agileLog.push_back((Ice::Double) attitude_dec_y[i]);
				agileLog.push_back((Ice::Double) attitude_ra_y[i]);
				agileLog.push_back((Ice::Double) mode[i]);
				agileLog.push_back((Ice::Double) log_status[i]);
				agileLog.push_back((Ice::Double) livetime[i]);
				agileLog.push_back((Ice::Double) phase[i]);
				agileLog.push_back((Ice::Double) time[i]);

				//Populate the key
				key.time = time[i];
				key.livetime = livetime[i];
				key.logStatus = log_status[i];
				key.mode = mode[i];
				key.phase = phase[i];

				//Execute write
				DBLog.insert(make_pair(key, agileLog));


			}
			cout << count << endl;
		}
		connection->close();
    	communicator->destroy();
	} catch(IOException* e) {
		cout << e->getErrorCode() << ": " << e->what() << endl;
		return e->getErrorCode();
	}
}
