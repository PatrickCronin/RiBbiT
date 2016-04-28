# Makefile for RiBbiT
# Started 27 February 2004
# (c) Copyright Patrick Cronin

COMPILE=cc -O0 -Wall -g -c
LINK=libtool --mode=link gcc -export-dynamic -ldl -O0 -Wall -g

COMPILE_MOD=libtool --mode=compile gcc -g -O0 -c
LINK_MOD=libtool --mode=link gcc -O0 -g -module -avoid-version -rpath /usr/lib
INSTALL_MOD=libtool --mode=install cp -f
LIBDIR=/usr/local/lib

# Directory variables
PROGRAMDIR=src/program
LISTSDIR=src/data_types/list_structures
SCHOOLDIR=src/data_types/school_structures
MISCDIR=src/data_types/misc_structures
MODDIR=src/modules
TESTSDIR=src/tests
BINDIR=bin

ALLMODULES=$(MODDIR)/ribbit_subject_sameday.la $(MODDIR)/ribbit_teacher_sametime.la \
	$(MODDIR)/ribbit_blocked_cells.la $(MODDIR)/ribbit_room_sametime.la

# ALL
all: $(BINDIR)/test_silist_driver $(ALLMODULES) $(BINDIR)/ribbit

# BIN
$(BINDIR)/test_silist_driver: $(PROGRAMDIR)/memory.o $(LISTSDIR)/silist.o \
		$(TESTSDIR)/test_silist_driver.o
	$(LINK) $(PROGRAMDIR)/memory.o $(LISTSDIR)/silist.o \
		$(TESTSDIR)/test_silist_driver.o \
		-o $(BINDIR)/test_silist_driver

$(BINDIR)/ribbit: $(PROGRAMDIR)/main.o $(PROGRAMDIR)/parse_args.o \
		$(MISCDIR)/bigthing.o $(PROGRAMDIR)/memory.o \
		$(SCHOOLDIR)/mtt.o $(MISCDIR)/data.o \
		$(SCHOOLDIR)/stc.o $(LISTSDIR)/silist.o \
		$(SCHOOLDIR)/subject.o $(LISTSDIR)/sandndlist.o \
		$(SCHOOLDIR)/teacher.o $(SCHOOLDIR)/class.o \
		$(SCHOOLDIR)/room.o $(PROGRAMDIR)/parse_xml_input.o \
		$(SCHOOLDIR)/meetings.o $(LISTSDIR)/ulvlist.o \
		$(LISTSDIR)/ulvpclist.o $(PROGRAMDIR)/algorithm2.o \
		$(PROGRAMDIR)/module.o $(MISCDIR)/bct.o \
		$(LISTSDIR)/smlist.o $(SCHOOLDIR)/ttt.o \
		$(SCHOOLDIR)/day.o $(LISTSDIR)/muvdalist.o
	$(LINK) $(PROGRAMDIR)/main.o \
		$(PROGRAMDIR)/algorithm2.o \
		$(PROGRAMDIR)/parse_args.o \
		$(PROGRAMDIR)/parse_xml_input.o \
		$(PROGRAMDIR)/memory.o \
		$(MISCDIR)/bigthing.o \
		$(MISCDIR)/bct.o \
		$(MISCDIR)/data.o \
		$(LISTSDIR)/silist.o \
		$(LISTSDIR)/ulvlist.o \
		$(LISTSDIR)/ulvpclist.o \
		$(LISTSDIR)/muvdalist.o \
		$(LISTSDIR)/sandndlist.o \
		$(LISTSDIR)/smlist.o \
		$(SCHOOLDIR)/subject.o \
		$(SCHOOLDIR)/teacher.o \
		$(SCHOOLDIR)/class.o \
		$(SCHOOLDIR)/room.o \
		$(SCHOOLDIR)/stc.o \
		$(SCHOOLDIR)/meetings.o \
		$(SCHOOLDIR)/mtt.o \
		$(SCHOOLDIR)/ttt.o \
		$(SCHOOLDIR)/day.o \
		$(PROGRAMDIR)/module.o \
		$(MODDIR)/ribbit_subject_sameday.lo \
		-lxml2 \
		-lgmp \
		-ldl \
		-o $(BINDIR)/ribbit

# SRC/PROGRAM
$(PROGRAMDIR)/algorithm2.o: $(PROGRAMDIR)/algorithm2.c \
		$(PROGRAMDIR)/algorithm2.h $(LISTSDIR)/sandndlist.h \
		$(MISCDIR)/bigthing.h $(LISTSDIR)/ulvlist.h \
		$(SCHOOLDIR)/stc.h $(SCHOOLDIR)/mtt.h \
		$(PROGRAMDIR)/module.h
	$(COMPILE) $(PROGRAMDIR)/algorithm2.c \
		-o $(PROGRAMDIR)/algorithm2.o

$(PROGRAMDIR)/memory.o: $(PROGRAMDIR)/memory.c $(PROGRAMDIR)/memory.h
	$(COMPILE) $(PROGRAMDIR)/memory.c \
		-o $(PROGRAMDIR)/memory.o

$(PROGRAMDIR)/main.o: $(PROGRAMDIR)/main.c $(MISCDIR)/bigthing.h \
		$(PROGRAMDIR)/parse_args.h $(MISCDIR)/data.h \
		$(LISTSDIR)/ulvlist.h $(PROGRAMDIR)/parse_xml_input.h \
		$(PROGRAMDIR)/algorithm2.h $(PROGRAMDIR)/module.h
	$(COMPILE) $(PROGRAMDIR)/main.c \
		-o $(PROGRAMDIR)/main.o

$(PROGRAMDIR)/module.o: $(PROGRAMDIR)/module.c $(PROGRAMDIR)/module.h \
		$(PROGRAMDIR)/memory.h $(SCHOOLDIR)/stc.h \
		$(MISCDIR)/bigthing.h $(LISTSDIR)/sandndlist.h
	$(COMPILE) $(PROGRAMDIR)/module.c \
		-o $(PROGRAMDIR)/module.o

$(PROGRAMDIR)/parse_args.o: $(PROGRAMDIR)/parse_args.c \
		$(PROGRAMDIR)/parse_args.h $(MISCDIR)/data.h
	$(COMPILE) $(PROGRAMDIR)/parse_args.c \
		-o $(PROGRAMDIR)/parse_args.o

$(PROGRAMDIR)/parse_xml_input.o: $(PROGRAMDIR)/parse_xml_input.c \
		$(PROGRAMDIR)/parse_xml_input.h $(MISCDIR)/data.h \
		$(SCHOOLDIR)/subject.h $(SCHOOLDIR)/teacher.h \
		$(SCHOOLDIR)/class.h $(SCHOOLDIR)/room.h \
		$(SCHOOLDIR)/meetings.h $(LISTSDIR)/ulvlist.h \
		$(PROGRAMDIR)/module.h $(SCHOOLDIR)/day.h
	$(COMPILE) $(PROGRAMDIR)/parse_xml_input.c \
		-o $(PROGRAMDIR)/parse_xml_input.o

# SRC/DATA_TYPES/LIST_STRUCTURES
$(LISTSDIR)/cuvlist.o: $(LISTSDIR)/cuvlist.h $(LISTSDIR)/cuvlist.c \
		$(PROGRAMDIR)/memory.h
	$(COMPILE) $(LISTSDIR)/cuvlist.c \
		-o $(LISTSDIR)/cuvlist.o

$(LISTSDIR)/muvdalist.o: $(LISTSDIR)/muvdalist.h $(LISTSDIR)/muvdalist.c \
		$(PROGRAMDIR)/memory.h
	$(COMPILE) $(LISTSDIR)/muvdalist.c \
		-o $(LISTSDIR)/muvdalist.o

$(LISTSDIR)/sandndlist.o: $(LISTSDIR)/sandndlist.h $(LISTSDIR)/sandndlist.c \
		$(PROGRAMDIR)/memory.h
	$(COMPILE) $(LISTSDIR)/sandndlist.c \
		-o $(LISTSDIR)/sandndlist.o

$(LISTSDIR)/silist.o: $(LISTSDIR)/silist.h $(LISTSDIR)/silist.c \
		$(PROGRAMDIR)/memory.h
	$(COMPILE) $(LISTSDIR)/silist.c \
		-o $(LISTSDIR)/silist.o

$(LISTSDIR)/smlist.o: $(LISTSDIR)/smlist.h $(LISTSDIR)/smlist.c \
		$(PROGRAMDIR)/memory.h
	$(COMPILE) $(LISTSDIR)/smlist.c \
		-o $(LISTSDIR)/smlist.o

$(LISTSDIR)/ulvlist.o: $(LISTSDIR)/ulvlist.h $(LISTSDIR)/ulvlist.c \
	$(PROGRAMDIR)/memory.h
	$(COMPILE) $(LISTSDIR)/ulvlist.c \
		-o $(LISTSDIR)/ulvlist.o

$(LISTSDIR)/ulvpclist.o: $(LISTSDIR)/ulvpclist.h $(LISTSDIR)/ulvpclist.c \
	$(PROGRAMDIR)/memory.h
	$(COMPILE) $(LISTSDIR)/ulvpclist.c \
		-o $(LISTSDIR)/ulvpclist.o

# SRC/DATA_TYPES/SCHOOL_STRUCTURES
$(SCHOOLDIR)/class.o: $(SCHOOLDIR)/class.h $(SCHOOLDIR)/class.c \
	$(PROGRAMDIR)/memory.h
	$(COMPILE) $(SCHOOLDIR)/class.c \
		-o $(SCHOOLDIR)/class.o

$(SCHOOLDIR)/day.o: $(SCHOOLDIR)/day.h $(SCHOOLDIR)/day.c \
		$(PROGRAMDIR)/memory.h
	$(COMPILE) $(SCHOOLDIR)/day.c \
		-o $(SCHOOLDIR)/day.o

$(SCHOOLDIR)/meetings.o: $(SCHOOLDIR)/meetings.h $(SCHOOLDIR)/meetings.c \
		$(PROGRAMDIR)/memory.h $(LISTSDIR)/silist.h \
		$(LISTSDIR)/smlist.h
	$(COMPILE) $(SCHOOLDIR)/meetings.c \
		-o $(SCHOOLDIR)/meetings.o

$(SCHOOLDIR)/mtt.o: $(SCHOOLDIR)/mtt.h $(SCHOOLDIR)/mtt.c \
		$(PROGRAMDIR)/memory.h $(SCHOOLDIR)/stc.h \
		$(SCHOOLDIR)/class.h $(SCHOOLDIR)/room.h \
		$(SCHOOLDIR)/teacher.h $(SCHOOLDIR)/subject.h \
		$(MISCDIR)/data.h $(MISCDIR)/bigthing.h \
		$(SCHOOLDIR)/ttt.h $(LISTSDIR)/sandndlist.h
	$(COMPILE) $(SCHOOLDIR)/mtt.c \
		-o $(SCHOOLDIR)/mtt.o

$(SCHOOLDIR)/room.o: $(SCHOOLDIR)/room.h $(SCHOOLDIR)/room.c \
		$(PROGRAMDIR)/memory.h
	$(COMPILE) $(SCHOOLDIR)/room.c \
		-o $(SCHOOLDIR)/room.o

$(SCHOOLDIR)/stc.o: $(SCHOOLDIR)/stc.h $(SCHOOLDIR)/stc.c \
		$(PROGRAMDIR)/memory.h $(LISTSDIR)/silist.h
	$(COMPILE) $(SCHOOLDIR)/stc.c \
		-o $(SCHOOLDIR)/stc.o

$(SCHOOLDIR)/subject.o: $(SCHOOLDIR)/subject.h $(SCHOOLDIR)/subject.c \
		$(PROGRAMDIR)/memory.h
	$(COMPILE) $(SCHOOLDIR)/subject.c \
		-o $(SCHOOLDIR)/subject.o

$(SCHOOLDIR)/teacher.o: $(SCHOOLDIR)/teacher.h $(SCHOOLDIR)/teacher.c \
		$(PROGRAMDIR)/memory.h
	$(COMPILE) $(SCHOOLDIR)/teacher.c \
		-o $(SCHOOLDIR)/teacher.o

$(SCHOOLDIR)/ttt.o: $(SCHOOLDIR)/ttt.h $(SCHOOLDIR)/ttt.c \
		$(PROGRAMDIR)/memory.h $(SCHOOLDIR)/stc.h \
		$(SCHOOLDIR)/class.h $(SCHOOLDIR)/room.h \
		$(SCHOOLDIR)/teacher.h $(SCHOOLDIR)/subject.h \
		$(MISCDIR)/data.h $(MISCDIR)/bigthing.h \
		$(SCHOOLDIR)/mtt.h $(LISTSDIR)/sandndlist.h
	$(COMPILE) $(SCHOOLDIR)/ttt.c \
		-o $(SCHOOLDIR)/ttt.o

# SRC/DATA_TYPES/MISC_STRUCTURES
$(MISCDIR)/bct.o: $(MISCDIR)/bct.c $(MISCDIR)/bct.h \
		$(MISCDIR)/data.h $(PROGRAMDIR)/memory.h
	$(COMPILE) $(MISCDIR)/bct.c \
		-o $(MISCDIR)/bct.o

$(MISCDIR)/bigthing.o: $(MISCDIR)/bigthing.c $(MISCDIR)/bigthing.h \
		$(PROGRAMDIR)/memory.h $(SCHOOLDIR)/mtt.h \
		$(LISTSDIR)/ulvlist.h $(MISCDIR)/data.h \
		$(SCHOOLDIR)/meetings.h $(PROGRAMDIR)/module.h \
		$(SCHOOLDIR)/subject.h $(SCHOOLDIR)/teacher.h \
		$(SCHOOLDIR)/class.h $(SCHOOLDIR)/stc.h \
		$(LISTSDIR)/sandndlist.h $(LISTSDIR)/silist.h
	$(COMPILE) $(MISCDIR)/bigthing.c \
		-o $(MISCDIR)/bigthing.o

#$(MISCDIR)/ctt.o: $(MISCDIR)/ctt.c $(MISCDIR)/ctt.h \
#		$(MISCDIR)/data.h $(PROGRAMDIR)/memory.h \
#		$(MISCDIR)/bigthing.h
#	$(COMPILE) $(MISCDIR)/ctt.c \
#		-o $(MISCDIR)/ctt.o

$(MISCDIR)/data.o: $(MISCDIR)/data.c \
		$(MISCDIR)/data.h \
		$(LISTSDIR)/sandndlist.h \
		$(LISTSDIR)/ulvlist.h \
		$(PROGRAMDIR)/memory.h \
		$(SCHOOLDIR)/subject.h \
		$(SCHOOLDIR)/teacher.h \
		$(SCHOOLDIR)/class.h \
		$(SCHOOLDIR)/room.h \
		$(SCHOOLDIR)/meetings.h \
		$(SCHOOLDIR)/stc.h \
		$(PROGRAMDIR)/module.h
	$(COMPILE) $(MISCDIR)/data.c \
		-o $(MISCDIR)/data.o

# SRC/MODULES
$(MODDIR)/ribbit_room_sametime.o: $(MODDIR)/ribbit_room_sametime.c \
		$(PROGRAMDIR)/module.h $(MISCDIR)/bigthing.h \
		$(SCHOOLDIR)/mtt.h $(SCHOOLDIR)/stc.h
	$(COMPILE_MOD) $(MODDIR)/ribbit_room_sametime.c \
		-o $(MODDIR)/ribbit_room_sametime.o

$(MODDIR)/ribbit_room_sametime.la: $(MODDIR)/ribbit_room_sametime.o
	$(LINK_MOD) $(MODDIR)/ribbit_room_sametime.lo \
		-o $(MODDIR)/ribbit_room_sametime.la

$(MODDIR)/ribbit_subject_sameday.o: $(MODDIR)/ribbit_subject_sameday.c \
		$(PROGRAMDIR)/module.h $(MISCDIR)/bigthing.h \
		$(SCHOOLDIR)/mtt.h $(SCHOOLDIR)/stc.h
	$(COMPILE_MOD) $(MODDIR)/ribbit_subject_sameday.c \
		-o $(MODDIR)/ribbit_subject_sameday.o

$(MODDIR)/ribbit_subject_sameday.la: $(MODDIR)/ribbit_subject_sameday.o
	$(LINK_MOD) $(MODDIR)/ribbit_subject_sameday.lo \
		-o $(MODDIR)/ribbit_subject_sameday.la

$(MODDIR)/ribbit_teacher_sametime.o: $(MODDIR)/ribbit_teacher_sametime.c \
		$(PROGRAMDIR)/module.h $(MISCDIR)/bigthing.h \
		$(SCHOOLDIR)/mtt.h $(SCHOOLDIR)/stc.h
	$(COMPILE_MOD) $(MODDIR)/ribbit_teacher_sametime.c \
		-o $(MODDIR)/ribbit_teacher_sametime.o

$(MODDIR)/ribbit_teacher_sametime.la: $(MODDIR)/ribbit_teacher_sametime.o
	$(LINK_MOD) $(MODDIR)/ribbit_teacher_sametime.lo \
		-o $(MODDIR)/ribbit_teacher_sametime.la

$(MODDIR)/ribbit_blocked_cells.o: $(MODDIR)/ribbit_blocked_cells.c \
		$(MODDIR)/ribbit_blocked_cells.h \
		$(MISCDIR)/bct.h $(PROGRAMDIR)/memory.h \
		$(PROGRAMDIR)/module.h $(MISCDIR)/bigthing.h \
		$(SCHOOLDIR)/mtt.h $(MISCDIR)/data.h \
		$(SCHOOLDIR)/stc.h
	$(COMPILE_MOD) $(MODDIR)/ribbit_blocked_cells.c \
		-o $(MODDIR)/ribbit_blocked_cells.o

$(MODDIR)/ribbit_blocked_cells.la: $(MODDIR)/ribbit_blocked_cells.o
	$(LINK_MOD) $(MODDIR)/ribbit_blocked_cells.lo \
		-o $(MODDIR)/ribbit_blocked_cells.la

# INSTALLING MODULES
install: $(MODDIR)/ribbit_subject_sameday.la \
		$(MODDIR)/ribbit_teacher_sametime.la \
		$(MODDIR)/ribbit_blocked_cells.la
	$(INSTALL_MOD) $(MODDIR)/ribbit_subject_sameday.la $(LIBDIR)/ribbit_subject_sameday.la; \
	$(INSTALL_MOD) $(MODDIR)/ribbit_teacher_sametime.la $(LIBDIR)/ribbit_teacher_sametime.la; \
	$(INSTALL_MOD) $(MODDIR)/ribbit_blocked_cells.la $(LIBDIR)/ribbit_blocked_cells.la; \
	$(INSTALL_MOD) $(MODDIR)/ribbit_room_sametime.la $(LIBDIR)/ribbit_room_sametime.la

# SRC/TESTS
$(TESTSDIR)/test_silist_driver.o: $(TESTSDIR)/test_silist_driver.c \
		$(PROGRAMDIR)/memory.h $(LISTSDIR)/silist.h
	$(COMPILE) $(TESTSDIR)/test_silist_driver.c \
		-o $(TESTSDIR)/test_silist_driver.o

clean:
	rm -f *~; \
	rm -f $(LISTSDIR)/*~; \
	rm -f $(SCHOOLDIR)/*~; \
	rm -f $(MODDIR)/*~; \
	rm -f $(PROGRAMDIR)/*~; \
	rm -f $(TESTSDIR)/*~; \
	rm -f *.o; \
	rm -f $(LISTSDIR)/*.o; \
	rm -f $(SCHOOLDIR)/*.o; \
	rm -f $(MODDIR)/*.o; \
	rm -f $(MODDIR)/*.lo; \
	rm -f $(MODDIR)/*.la; \
	rm -rf $(MODDIR)/.libs; \
	rm -f $(PROGRAMDIR)/*.o; \
	rm -f $(TESTSDIR)/*.o; \
	rm -f $(BINDIR)/*
