cimport cython
import numpy
cimport numpy
from libcpp.string cimport string
import re

cdef extern from "cxyz.h":
    ctypedef float coordinate[3]
    ctypedef char element[2]

    void _get_fpos(string file , unsigned long long * pos, int n_atoms, int n_frames)
    void _get_coords(string file, const unsigned long long & pos, coordinate * coords,  int & n_atoms)

cdef class XYZReader(object):
    cdef public string xyz_fdir
    cdef public int n_atoms
    cdef public int n_frames 
    cdef public numpy.ndarray f_pos
    cdef public numpy.ndarray coords
    cdef public numpy.ndarray elements
    cdef public unsigned long long _curr_pos

    def __init__(self, xyz_fdir, n_atoms=None, n_frames=None, f_pos=None):

        self.xyz_fdir = <string> xyz_fdir.encode('utf-8')

        if f_pos is not None:
            if type(f_pos) is numpy.ndarray:
                self.f_pos = f_pos
            else:
                raise ValueError("if not None, f_pos shall be ndarray")
        else:
            # '150000' should be enough for approx 5 ns traj 
            self.f_pos = numpy.zeros((150000), dtype=numpy.ulonglong)

        if n_atoms is not None:
            if type(n_atoms) is int:
                self.n_atoms = n_atoms
                self.coords = numpy.zeros((self.n_atoms, 3), dtype=numpy.float32)
                self.elements = numpy.zeros((self.n_atoms), dtype=str)

        if n_frames is not None:
            if type(n_frames) is int:
                self.n_frames = n_atoms

    @staticmethod
    def get_fpos(string f, numpy.ndarray pos):
        cdef int n_atoms, n_frames
        # convert python str to str
        n_atoms, n_frames = 0, 0
        _get_fpos(f , <unsigned long long*> pos.data, 
                n_atoms, n_frames)
        return n_atoms, n_frames
    
    @staticmethod
    def get_coords(string f, unsigned long long pos, 
                   numpy.ndarray coords, int n_atoms):
        _get_coords(f, pos, <coordinate*> coords.data, n_atoms)
    
    def run_get_fpos(self):
        self.n_atoms, self.n_frames = self.get_fpos(self.xyz_fdir,
                                                    self.f_pos)
        self.f_pos = self.f_pos[:self.n_frames]
        self.coords = numpy.zeros((self.n_atoms, 3), dtype=numpy.float32)
        self.elements = numpy.zeros((self.n_atoms), dtype=str)
    
    def get_frame_coords(self, frame_idx):
        self._curr_pos = self.f_pos[frame_idx]
        self.get_coords(self.xyz_fdir, self._curr_pos, self.coords, self.n_atoms)
        return self.coords

    def get_elements(self):
        """
        parse elements in a pythonic way
        """
        with open(self.xyz_fdir) as xyzfile:
            head = [next(xyzfile) for x in range(self.n_atoms+2)][2:]
            for ii in range(self.n_atoms):
                self.elements[ii] = re.split(' +', head[ii].strip())[0]
