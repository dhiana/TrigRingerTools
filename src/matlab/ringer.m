%function rings = ringer(RoI, configFile, globalCenter, etaWindow, phiWindow)
%
%Arranges calorimetry cell data (RoIs) in concentric rings of configurable 
%size and center and returns a vector containing the ring'ified information
%for every RoI and their metadata (LVL1 and RoI identifiers and etaxphi 
%information).
%
%Input parameters are:
%RoI -> A vector where each item is a RoI, represented by a struct 
%containing the following fields:
%    - ECells : a vector containing the cell's energy.
%    - DetCells : a vector containin the cell's layer.
%    - PhiCells : a vector containing the cell's phi coordinate.
%    - EtaCells : a vector containing the cell's eta coordinate.
%    - LVL1_Id : The Level 1 ID of the RoI.
%    - RoI_Id : The RoI ID.
%    - LVL1_Eta : The LVL1 Eta coordinate of the RoI.
%    - LVL1_Phi : The LVL1 Phi coordinate of the RoI.
%    The 4 vectors must be organized so that each index correspond to a
%    cell.
%configFile -> The XML file specifying how the rings will be generated.
%globalCenter -> (opt) If true, then the center for all layers will be the one
%                given by layer 2 (default is false). 
%etaWindow -> (opt) The eta window size for peak finding (default is 0.1).
%phiWindow -> (opt) The phi window size for peak finding (default is 0.1).
%
%Returns a vector where each element is an struct with the following
%fields:
%    - LVL1_Id : The Level 1 ID of the original RoI.
%    - RoI_Id : The original RoI ID.
%    - LVL1_Eta : The LVL1 Eta coordinate of the original RoI.
%    - LVL1_Phi : The LVL1 Phi coordinate of the original RoI.
%    - Rings : A vector containing the generated rings.
%

