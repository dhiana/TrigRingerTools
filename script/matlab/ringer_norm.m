%function ringer_norm(rings, ringsDist, secDist, normType) 
%Calculates and applies energy normalization to the rings.
%
%The possible normalizations (4) are:
%
%1) SET: divide each ring by the total energy of the layer it belongs to.
%
%2) EVENT: divide each ring by the total energy of all rings, from all layers.
%
%3) SECTION: divide each ring by the total energy of the section (EM or HAD) it belongs to.
%
%4) SEQUENTIAL: Calculates and applies the sequential normalisation vector that results
%from the following arithmetic:
%N0 = E
%N1 = E - E0
%N2 = E - E0 - E1 = N1 - E1
%N3 = E - E0 - E1 - E2 = N2 - E2
%...
%NX = N(X-1) - E(X-1)
%
%Where Ni are the normalisation factors, E is the total layer energy and Ei
%are the energy values for each ring. 
%
%Parameters are:
%  rings: is the matrix where the rings are and each ring is a column.
%  ringsDist: is a vector which tells how many rings per layer (detector) 
%             we have.
%  secDist: A cell vector containing the strings 'em' or 'had', specifying,
%           for each layer, whether it belongs to an EM section or HAD section (ex:
%           {'em' 'em' 'em' 'had', 'had'}.
%  normType: A string containing the normalization type (set, event, section or sequential)
%
%The function returns a new matrix, with the normalized rings.
%
