# -*- coding: utf-8 -*-

###
### This file is generated automatically by SALOME v7.7.1 with dump python functionality
###

import sys
import salome

salome.salome_init()
theStudy = salome.myStudy

import salome_notebook
notebook = salome_notebook.NoteBook(theStudy)
sys.path.insert( 0, r'/home/mrenda/mentor/micromegas/salome')

###
### GEOM component
###

import GEOM
from salome.geom import geomBuilder
import math
import SALOMEDS

w = 100000
h = 100000

w_cell = 100
h_cell = 100


geompy = geomBuilder.New(theStudy)

O = geompy.MakeVertex(0, 0, 0)
OX = geompy.MakeVectorDXDYDZ(1, 0, 0)
OY = geompy.MakeVectorDXDYDZ(0, 1, 0)
OZ = geompy.MakeVectorDXDYDZ(0, 0, 1)
plate_bottom = geompy.MakeBoxDXDYDZ(w, h, 5)
geompy.TranslateDXDYDZ(plate_bottom, -w/2, -h/2, 0)

plate_middle = geompy.MakeBoxDXDYDZ(w, h, 5)
geompy.TranslateDXDYDZ(plate_middle, -w/2, -h/2, 134.5)

plate_top = geompy.MakeBoxDXDYDZ(w, h, 20)
geompy.TranslateDXDYDZ(plate_top, -w/2, -h/2, 0)
geompy.TranslateDXDYDZ(plate_top, 0, 0, 3136)
Box_2_vertex_21 = geompy.GetSubShape(plate_top, [21])
Box_1_vertex_6 = geompy.GetSubShape(plate_bottom, [6])
gas = geompy.MakeBoxTwoPnt(Box_2_vertex_21, Box_1_vertex_6)

gas_holed = geompy.MakeCutList(gas, [plate_middle])

grp_face_top = geompy.CreateGroup(gas_holed, geompy.ShapeType["FACE"])
geompy.UnionIDs(grp_face_top, [55])
grp_face_bottom = geompy.CreateGroup(gas_holed, geompy.ShapeType["FACE"])
geompy.UnionIDs(grp_face_bottom, [31])
grp_face_lat = geompy.CreateGroup(gas_holed, geompy.ShapeType["FACE"])
geompy.UnionIDs(grp_face_lat, [68, 60, 38, 48])
grp_face_grid = geompy.CreateGroup(gas_holed, geompy.ShapeType["FACE"])


for face in  geompy.SubShapeAllSortedCentres(gas_holed, geompy.ShapeType["FACE"]):
	face_id = geompy.GetSubShapeID(gas_holed, face)
	geompy.UnionIDs(grp_face_grid, [face_id])
	
geompy.DifferenceIDs(grp_face_grid, [55, 31, 68, 60, 38, 48])


geompy.addToStudy( O, 'O' )
geompy.addToStudy( OX, 'OX' )
geompy.addToStudy( OY, 'OY' )
geompy.addToStudy( OZ, 'OZ' )
geompy.addToStudy( plate_bottom, 'plate_bottom' )
geompy.addToStudy( plate_middle, 'plate_middle' )
geompy.addToStudy( plate_top, 'plate_top' )
geompy.addToStudyInFather( plate_top, Box_2_vertex_21, 'Box_2:vertex_21' )
geompy.addToStudyInFather( plate_bottom, Box_1_vertex_6, 'Box_1:vertex_6' )
geompy.addToStudy( gas_holed, 'gas_holed' )
geompy.addToStudyInFather( gas_holed, grp_face_top, 'grp_face_top' )
geompy.addToStudyInFather( gas_holed, grp_face_bottom, 'grp_face_bottom' )
geompy.addToStudyInFather( gas_holed, grp_face_lat, 'grp_face_lat' )
geompy.addToStudyInFather( gas_holed, grp_face_grid, 'grp_face_grid' )

###
### SMESH component
###

import  SMESH, SALOMEDS
from salome.smesh import smeshBuilder

smesh = smeshBuilder.New(theStudy)
mesh_gas = smesh.Mesh(gas_holed)


grp_msh_top = mesh_gas.GroupOnGeom(grp_face_top,'grp_msh_top',SMESH.FACE)
grp_msh_top.SetColor( SALOMEDS.Color( 1, 0.666667, 0 ))
grp_msh_bottom = mesh_gas.GroupOnGeom(grp_face_bottom,'grop_msh_bottom',SMESH.FACE)
grp_msh_bottom.SetColor( SALOMEDS.Color( 1, 0.666667, 0 ))
grp_msh_grid = mesh_gas.GroupOnGeom(grp_face_grid,'grp_mesh_grid',SMESH.FACE)
grp_msh_grid.SetColor( SALOMEDS.Color( 1, 0.666667, 0 ))
grp_msh_bottom.SetName( 'grp_msh_bottom' )
grp_msh_lat = mesh_gas.GroupOnGeom(grp_face_lat,'grp_msh_lat',SMESH.FACE)
grp_msh_lat.SetColor( SALOMEDS.Color( 1, 0.666667, 0 ))
grp_msh_grid.SetName( 'grp_msh_grid' )
grp_msh_vol = mesh_gas.GroupOnGeom(gas_holed,'grp_msh_vol',SMESH.VOLUME)
grp_msh_vol.SetColor( SALOMEDS.Color( 1, 0.666667, 0 ))

NETGEN_2D3D = mesh_gas.Tetrahedron(algo=smeshBuilder.NETGEN_1D2D3D)
NETGEN_3D_Parameters_1 = NETGEN_2D3D.Parameters()
NETGEN_3D_Parameters_1.SetMaxSize( 10000 )
NETGEN_3D_Parameters_1.SetSecondOrder( 0 )
NETGEN_3D_Parameters_1.SetOptimize( 1 )
NETGEN_3D_Parameters_1.SetFineness( 2 )
NETGEN_3D_Parameters_1.SetMinSize( 200 )
NETGEN_3D_Parameters_1.SetUseSurfaceCurvature( 1 )
NETGEN_3D_Parameters_1.SetFuseEdges( 1 )
NETGEN_3D_Parameters_1.SetQuadAllowed( 0 )

# If you are loading in Salome GUI please comment the following two lines, because they are computation heavy
# mesh_gas.Compute()
# mesh_gas.ExportUNV('mesh_gas.unv')

if salome.sg.hasDesktop():
  salome.sg.updateObjBrowser(1)
