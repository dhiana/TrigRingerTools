import sc_globals

progs = {}

progs['rings-on-cells'] = {}
progs['rings-on-cells']['source'] = ['../src/progs/GenerateRingsOnCells.cxx', '../src/progs/RingsOnCells.cxx']
progs['rings-on-cells']['LIBS'] = ['rbuild', 'sys', 'data', 'roiformat'] + sc_globals.rootLibs

progs['ringer'] = {}
progs['ringer']['LIBS'] = ['rbuild', 'sys', 'data', 'roiformat'] + sc_globals.rootLibs

progs['getroi'] = {}
progs['getroi']['LIBS'] = ['roiformat', 'popt', 'sys'] + sc_globals.rootLibs

progs['merge'] = {}
progs['merge']['LIBS'] = ['data', 'popt', 'sys', 'roiformat']

progs['splitter'] = {}
progs['splitter']['LIBS'] = ['data', 'popt', 'sys', 'roiformat']

progs['filter'] = {}
progs['filter']['LIBS'] = ['data', 'roiformat', 'sys'] + sc_globals.rootLibs

progs['xml2text'] = {}
progs['xml2text']['LIBS'] = ['data', 'popt', 'sys', 'roiformat']

progs['mlp-train'] = {}
progs['mlp-train']['LIBS'] = ['network', 'popt', 'config', 'sys', 'data', 'roiformat', 'gsl', 'gslcblas']

progs['mlp-bp-train'] = {}
progs['mlp-bp-train']['LIBS'] = ['network', 'popt', 'sys', 'data', 'roiformat', 'config', 'gsl', 'gslcblas'] + sc_globals.rootLibs

progs['lms-train'] = {}
progs['lms-train']['LIBS'] = ['network', 'popt', 'sys', 'data', 'roiformat', 'config', 'gsl', 'gslcblas']

progs['mlp-run'] = {}
progs['mlp-run']['LIBS'] = ['network', 'popt', 'sys', 'roiformat', 'data']

progs['mlp-relevance'] = {}
progs['mlp-relevance']['LIBS'] = ['network', 'popt', 'sys', 'data', 'roiformat', 'config']

progs['eta-filter'] = {}
progs['eta-filter']['LIBS'] = ['data', 'popt', 'sys', 'roiformat', 'gsl', 'gslcblas']

progs['relevance-filter'] = {}
progs['relevance-filter']['LIBS'] = ['data', 'popt', 'sys', 'roiformat']

progs['xml2dot'] = {}
progs['xml2dot']['LIBS'] = ['network', 'sys', 'roiformat']

progs['ringer-run'] = {}
progs['ringer-run']['LIBS'] = ['network', 'rbuild', 'data', 'sys', 'roiformat'] + sc_globals.rootLibs

progs['lvl1-filter'] = {}
progs['lvl1-filter']['LIBS'] = ['lvl1', 'roiformat', 'sys'] + sc_globals.rootLibs
