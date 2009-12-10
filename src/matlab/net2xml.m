%function net2xml(net, outFile, mapStdStr)
%Saves a neural network in the file format used in online (Athena)
%operation. net is the neural network structure, as returned by newff.
%outFile is the name of the output file, and mapStdStr is an OPTIONAL 
%normalizing structure, which should contain the fields 'xmean' and xstd, 
%which are respectively the subtracting and dividing value to be applied
%to an input to be fed to the network, one example of use of this parameter
%is to spherize the input data using mapstd, and pass the spherizing
%structure to this function.
%