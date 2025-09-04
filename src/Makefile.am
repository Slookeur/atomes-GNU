
# To prepare the OpenGL testing utility:
SUBDIRS = startup-testing

# Setup the name of the program to be generated, using:
bin_PROGRAMS = atomes

atomes_LDADD = $(LGTK_LIBS) $(LIBXML2_LIBS) $(PANGOFT2_LIBS) $(FFMPEG_LIBS) $(GLU_LIBS) $(EPOXY_LIBS) $(INTLLIBS)
if WINDOWS
  atomes_LDADD += win_atomes.o

win_atomes.o: ../misc/win/atomes.rc
	$(WINDRES) $< $@		
endif

# Create a variable to store the CFLAGS for all the required libraries
LIBS_CFLAGS = $(LGTK_CFLAGS) $(LIBXML2_CFLAGS) $(PANGOFT2_CFLAGS) $(FFMPEG_CFLAGS) $(GLU_CFLAGS) $(EPOXY_CFLAGS)

# To turn OpenMP on/off
if OPENMP
OpenMP_FLAGS = -DOPENMP $(OPENMP_CFLAGS)
else
OpenMP_FLAGS = 
endif

# Use AM_LDFLAGS to define additional linker flags:
AM_LDFLAGS = $(OpenMP_FLAGS)

if GTK3
  GTK_VERSION=-DGTK3
else
  GTK_VERSION=-DGTK4
endif

# Use AM_CPPFLAGS to define additional preprocessor flags:
if WINDOWS
AM_CPPFLAGS = $(GTK_VERSION) -DGTKGLAREA -DMENU_ICONS $(OpenMP_FLAGS) 
else
PACKAGE_CPPFLAGS = -DPACKAGE_PREFIX=\""$(datadir)/atomes"\" -DPACKAGE_LIBEXEC=\""$(libexecdir)"\"
if LINUX
AM_CPPFLAGS = -DLINUX $(GTK_VERSION) -DGTKGLAREA -DMENU_ICONS $(OpenMP_FLAGS) $(PACKAGE_CPPFLAGS)
else
AM_CPPFLAGS = -DOSX $(GTK_VERSION) -DGTKGLAREA -DMENU_ICONS $(OpenMP_FLAGS) $(PACKAGE_CPPFLAGS)
endif
endif

# Use AM_FFLAGS to define additional Fortran 90 flags:
AM_FFLAGS = $(OpenMP_FLAGS)

for = fortran/
gui = gui/
work = workspace/
proj = project/
read = project/readers/
calc = calc/
dlpoly = calc/dl_poly/
lammps = calc/lammps/
fields = calc/force_fields/
cpmd = calc/cpmd/
cp2k = calc/cp2k/
curve = curve/
ogl = opengl/
glwin = opengl/win/
gldraw = opengl/draw/
gledit = opengl/edit/

inc = -I$(gui) -I$(work) -I$(proj) -I$(read) -I$(calc) -I$(dlpoly) -I$(lammps) -I$(fields) -I$(cpmd) -I$(cp2k) -I$(curve) -I$(glwin) -I$(gledit) -I$(gldraw) -I$(ogl) -I.
includes = $(inc) -DGTK_DISABLE_DEPRECATED

# Use AM_CFLAGS to define additional C flags:
AM_CFLAGS = $(includes) $(OpenMP_FLAGS) $(LIBS_CFLAGS)

atomes_fortran_modules = $(for)parameters.F90 \
			 $(for)mendeleiev.F90

atomes_fortran_files = $(for)allocbonds.F90 \
			$(for)allochem.F90 \
			$(for)allocmsd.F90 \
			$(for)angles.F90 \
			$(for)bonds.F90 \
			$(for)c3d.F90 \
			$(for)chains.F90 \
			$(for)chains_ogl.F90 \
			$(for)chemistry.F90 \
			$(for)clean.F90 \
			$(for)cqvf.F90 \
			$(for)dmtx.F90 \
			$(for)dvtb.F90 \
			$(for)escs.F90 \
			$(for)fzbt.F90 \
			$(for)gr.F90 \
			$(for)grfft.F90 \
			$(for)initchains.F90 \
			$(for)initrings.F90 \
			$(for)lattice.F90 \
			$(for)molecules.F90 \
			$(for)msd.F90 \
			$(for)pdb.F90 \
			$(for)prepdata.F90 \
			$(for)resrings.F90 \
			$(for)rings-guttman.F90 \
			$(for)rings-king.F90 \
			$(for)rings_ogl.F90 \
			$(for)rings-primitive.F90 \
			$(for)sk.F90 \
			$(for)spherical.F90 \
			$(for)sq.F90 \
			$(for)threads.F90 \
			$(for)trj.F90 \
			$(for)utils.F90 \
			$(for)vas.F90 \
			$(for)writedata.F90 \
			$(for)xyz.F90

# Rules to ensure that Fortran modules are compiled before Fortran files
atomes_fortran = $(atomes_fortran_modules) $(atomes_fortran_files)
$(patsubst %.F90,%.o,$(atomes_fortran_files)): $(patsubst %.F90,%.o,$(atomes_fortran_modules))

atomes_gui = $(gui)bdcall.c \
	     $(gui)calc_menu.c \
	     $(gui)callbacks.c \
	     $(gui)chainscall.c \
	     $(gui)edit_menu.c \
	     $(gui)grcall.c \
	     $(gui)gtk-misc.c \
	     $(gui)gui.c \
	     $(gui)initc.c \
	     $(gui)preferences.c \
	     $(gui)interface.c \
	     $(gui)main.c \
	     $(gui)msdcall.c \
	     $(gui)ringscall.c \
	     $(gui)spcall.c \
	     $(gui)sqcall.c \
	     $(gui)tools.c \
	     $(gui)work_menu.c 

atomes_workspace = $(work)modelinfo.c \
		   $(work)workinfo.c \
		   $(work)workspace.c

atomes_readers = $(read)read_c3d.c \
		 $(read)read_cif.c \
		 $(read)read_coord.c \
		 $(read)read_hist.c \
		 $(read)read_isaacs.c \
		 $(read)read_npt.c \
		 $(read)read_pdb.c \
		 $(read)read_trj.c \
		 $(read)read_vas.c \
		 $(read)read_xyz.c 

atomes_project = $(atomes_readers) \
		 $(proj)close_p.c \
		 $(proj)debugio.c \
		 $(proj)init_p.c \
		 $(proj)open_p.c \
		 $(proj)project.c \
		 $(proj)read_bond.c \
		 $(proj)read_curve.c \
		 $(proj)read_field.c \
		 $(proj)read_mol.c \
		 $(proj)read_opengl.c \
		 $(proj)read_qm.c \
		 $(proj)save_bond.c \
		 $(proj)save_curve.c \
		 $(proj)save_field.c \
		 $(proj)save_mol.c \
		 $(proj)save_opengl.c \
		 $(proj)save_p.c \
		 $(proj)save_qm.c \
		 $(proj)update_p.c

atomes_cp2k = $(cp2k)cp2k_basis.c \
	      $(cp2k)cp2k_files.c \
	      $(cp2k)cp2k_gth-basis.c \
	      $(cp2k)cp2k_init.c \
	      $(cp2k)cp2k_mol.c \
	      $(cp2k)cp2k_molopt-basis.c \
	      $(cp2k)cp2k_pot.c \
	      $(cp2k)cp2k_print.c

atomes_cpmd = $(cpmd)cpmd_atoms.c \
	      $(cpmd)cpmd_init.c \
	      $(cpmd)cpmd_nose.c \
	      $(cpmd)cpmd_print.c \
	      $(cpmd)cpmd_restart.c

atomes_dlpoly = $(dlpoly)dlp_active.c \
		$(dlpoly)dlp_atom.c \
		$(dlpoly)dlp_comp.c \
		$(dlpoly)dlp_control.c \
		$(dlpoly)dlp_copy.c \
		$(dlpoly)dlp_edit.c \
		$(dlpoly)dlp_ff_match.c \
		$(dlpoly)dlp_field.c \
		$(dlpoly)dlp_init.c \
		$(dlpoly)dlp_mol.c \
		$(dlpoly)dlp_print.c \
		$(dlpoly)dlp_viz.c

atomes_lammps = $(lammps)la_print.c

atomes_calc = $(calc)calc.c $(atomes_cp2k) $(atomes_cpmd) $(atomes_dlpoly) $(atomes_lammps) $(fields)force_fields.c 

atomes_curve = $(curve)cedit.c \
	       $(curve)curve.c \
	       $(curve)cwidget.c \
	       $(curve)datab.c \
	       $(curve)draw.c \
	       $(curve)frame.c \
	       $(curve)glyph.c \
	       $(curve)labels.c \
	       $(curve)legend.c \
	       $(curve)m_curve.c \
	       $(curve)show.c \
	       $(curve)tab-1.c \
	       $(curve)tab-2.c \
	       $(curve)tab-3.c \
	       $(curve)tab-4.c \
	       $(curve)title.c \
	       $(curve)w_curve.c \
	       $(curve)w_data.c \
	       $(curve)w_img.c \
	       $(curve)xaxis.c \
	       $(curve)yaxis.c

atomes_glwin = $(glwin)color_box.c \
	       $(glwin)glwindow.c \
	       $(glwin)initchain.c \
	       $(glwin)initcoord.c \
	       $(glwin)initmol.c \
	       $(glwin)initring.c \
	       $(glwin)m_anim.c \
	       $(glwin)m_atoms.c \
	       $(glwin)m_axis.c \
	       $(glwin)m_back.c \
	       $(glwin)m_bonds.c \
	       $(glwin)m_box.c \
	       $(glwin)m_clones.c \
	       $(glwin)m_coord.c \
	       $(glwin)m_edit.c \
	       $(glwin)menu_bar.c \
	       $(glwin)m_map.c \
	       $(glwin)m_poly.c \
	       $(glwin)m_proj.c \
	       $(glwin)m_quality.c \
	       $(glwin)m_render.c \
	       $(glwin)m_rep.c \
	       $(glwin)m_style.c \
	       $(glwin)m_tools.c \
	       $(glwin)popup.c \
	       $(glwin)w_advance.c \
	       $(glwin)w_atoms.c \
	       $(glwin)w_axis.c \
	       $(glwin)w_bonds.c \
	       $(glwin)w_box.c \
	       $(glwin)w_chains.c \
	       $(glwin)w_colors.c \
	       $(glwin)w_coord.c \
	       $(glwin)w_cutoffs.c \
	       $(glwin)w_encode.c \
	       $(glwin)w_labels.c \
	       $(glwin)w_library.c \
	       $(glwin)w_measures.c \
	       $(glwin)w_periodic.c \
	       $(glwin)w_record.c \
	       $(glwin)w_rings.c \
	       $(glwin)w_search.c \
	       $(glwin)w_sequencer.c \
	       $(glwin)w_spiner.c \
	       $(glwin)w_volumes.c

atomes_gldraw = $(gldraw)d_atoms.c \
		$(gldraw)d_axis.c \
		$(gldraw)d_bonds.c \
		$(gldraw)d_box.c \
		$(gldraw)d_label.c \
		$(gldraw)d_measures.c \
		$(gldraw)d_poly.c \
		$(gldraw)d_rings.c \
		$(gldraw)d_selection.c \
		$(gldraw)d_background.c \
		$(gldraw)image.c \
		$(gldraw)movie.c \
		$(gldraw)ogl_text.c

atomes_gledit = $(gledit)atom_action.c \
		$(gledit)atom_coord.c \
		$(gledit)atom_edit.c \
		$(gledit)atom_geo.c \
		$(gledit)atom_insert.c \
		$(gledit)atom_move.c \
		$(gledit)atom_object.c \
		$(gledit)atom_remove.c \
		$(gledit)atom_search.c \
		$(gledit)atom_species.c \
		$(gledit)cbuild_action.c \
		$(gledit)cbuild_edit.c \
		$(gledit)cbuild_info.c \
		$(gledit)cbuild_sg.c \
		$(gledit)cell_cut.c \
		$(gledit)cell_density.c \
		$(gledit)cell_edit.c \
		$(gledit)cell_extra.c \
		$(gledit)cell_pixel.c \
		$(gledit)cell_shift.c \
		$(gledit)cell_super.c

atomes_opengl = $(atomes_glwin) \
		$(atomes_gldraw) \
		$(atomes_gledit) \
		$(ogl)arcball.c \
		$(ogl)glview.c \
		$(ogl)ogl_draw.c \
		$(ogl)ogl_shaders.c \
		$(ogl)ogl_shading.c \
		$(ogl)ogl_utils.c \
		$(ogl)selection.c 

atomes_includes = config.h \
		  version.h \
		  global.h \
		  bind.h \
		  affero.h \
		  $(gui)callbacks.h \
		  $(gui)gui.h \
		  $(gui)interface.h \
		  $(gui)xmlrw.h \
		  $(gui)preferences.h \
		  $(work)workspace.h \
		  $(proj)project.h \
		  $(read)readers.h \
		  $(calc)calc.h \
		  $(cpmd)cpmd.h \
		  $(cp2k)cp2k.h \
		  $(dl_poly)dlp_field.h \
		  $(field)force_fields.h \
		  $(curve)cedit.h \
		  $(curve)curve.h \
		  $(curve)datab.h \
		  $(glwin)color_box.h \
		  $(glwin)glwindow.h \
		  $(glwin)initcoord.h \
		  $(glwin)submenus.h \
		  $(glraw)movie.h \
		  $(gledit)atom_edit.h \
		  $(gledit)cbuild_edit.h \
		  $(gledit)cell_edit.h \
		  $(ogl)glview.h \
		  $(ogl)glwin.h \
		  $(ogl)math_3d.h \
		  $(ogl)ogl_shading.h

clean: 
	-rm -f *.mod
	-rm -f */*.o
	-rm -f */*/*.o

# List the files required to build the program
atomes_SOURCES = \
	$(atomes_fortran) \
	$(atomes_includes) \
	global.c \
	$(atomes_gui) \
	$(atomes_workspace) \
	$(atomes_project) \
	$(atomes_calc) \
	$(atomes_curve) \
	$(atomes_opengl)
