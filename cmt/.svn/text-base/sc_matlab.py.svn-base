import commands

import sc_globals

def matlabBuild(target, source, env, for_signature):
	if commands.getstatusoutput('mex -help')[0] != 0:
		print "\nWARINING: MATLAB Compiler not found! Skiping matlab modules...\n"
		return ''
	
	incPaths = ' '.join(['-I%s' % i for i in env['CPPPATH']])
	libPaths = ' '.join(['-L%s' % i for i in env['LIBPATH']])
	libList = ' '.join(['-l%s' % i for i in env['LIBS']])
	compFlags = ' '.join(env['CCFLAGS'])
	return 'mex -cxx %s %s %s %s -o %s %s' % (compFlags, incPaths, libPaths, libList, target[0], source[0]);


def getMatlabSuffix():
	retCode, ext = commands.getstatusoutput('mexext');
	if retCode != 0: return 'mex';
	else: return ext;


matlab = {}

matlab['ringer'] = {}
matlab['ringer']['LIBS'] = ['rbuild', 'sys', 'data', 'roiformat'] + sc_globals.rootLibs

matlab['load_rings'] = {}
matlab['load_rings']['LIBS'] = ['sys'] + sc_globals.rootLibs

matlab['load_roi'] = {}
matlab['load_roi']['LIBS'] = ['sys'] + sc_globals.rootLibs

matlab['ringer_norm'] = {}
matlab['ringer_norm']['LIBS'] = ['rbuild', 'sys', 'data'] + sc_globals.rootLibs

matlab['net2xml'] = {}
matlab['net2xml']['LIBS'] = ['sys', 'config']
