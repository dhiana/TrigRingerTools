%function ntuple = load_roi(rootFileName)
%
%Reads a ROOT NTuple generated by the TrigNtCalo/CBNTAA_TrigRinger package
% and returns its roi contents in Matlab format.
%
%Input parameters are:
%rootFileName -> The NTuple file name (str). Wildcards (ex: *.aan.root) are accepted. 
%
%Returns a structure matrix with the following fields:
%  lvl1_id : The LVL1 Id. 
%  roi_id : The RoI Id.
%  lvl2_eta : The refined (LVL2) Eta coordinate for the RoI.
%  lvl2_phi : The refined (LVL2) Phi coordinate for the RoI.
%  rings : The generated rings.
%  detectors : The detector ID of each cell.
%  eta : The eta coordinate of each cell.
%  phi : The phi coordinate of each cell.
%  energy : The energy value (Mev) of each cell.
%

