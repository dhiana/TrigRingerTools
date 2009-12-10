import sc_globals

pyBindings = {};

pyBindings['roiformat'] = {}
pyBindings['roiformat']['LIBS'] =  sc_globals.rootLibs + ['roiformat']

pyBindings['rbuild'] = {}
pyBindings['rbuild']['LIBS'] =  sc_globals.rootLibs + ['sys', 'rbuild']
