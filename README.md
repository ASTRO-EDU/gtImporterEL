gtImporterEL
============
The AGILE EVT and LOG files importer into the ZeroC Ice/Freeze/BDB.

This project covers the GammaTouch/Milestone2 project for EVT and LOG files.

Run:

make CPPFLAGS=-DSIMPLE_KEY simple

to compile the project and make it works with the only time as the key of BDB


make CPPFLAGS=-DCOMPOSITE_KEY composite

to compile the project and make it works with the composite key

Use

./gtImporterEL ~/Projects/GammaTouch/data/M2/ag-128692735_STD0P_GO.LOG.gz <end_row>

./gtImporterEL ~/Projects/GammaTouch/data/M2/ag0709151200_0709161200_STD0P_GO_FM.EVT.gz <end_row>

where

<end_row> is an optional parameter that indicates the last line to be read.
