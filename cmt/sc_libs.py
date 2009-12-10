import sc_globals

libs = {};

libs['sys'] = {}
libs['sys']['LIBS'] = ['popt', 'xml2'] + sc_globals.rootLibs

libs['data'] = {}
libs['data']['LIBS'] = ['sys', 'gsl', 'gslcblas'] + sc_globals.rootLibs

libs['config'] = {}
libs['config']['LIBS'] = ['sys', 'data']

libs['roiformat'] = {}
libs['roiformat']['LIBS'] = ['sys'] + sc_globals.rootLibs

libs['lvl1'] = {}
libs['lvl1']['LIBS'] = ['roiformat', 'sys'] + sc_globals.rootLibs

libs['network'] = {}
libs['network']['LIBS'] = ['data', 'sys', 'config']

libs['rbuild'] = {}
libs['rbuild']['LIBS'] = ['data', 'sys', 'roiformat'] + sc_globals.rootLibs
