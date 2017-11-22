CC      = gcc 
PROF    = -g -O 
NOCRYPT =
C_FLAGS =  -Wall $(PROF) $(NOCRYPT)
L_FLAGS =  $(PROF)

O_FILES = act_comm.o act_ente.o act_info.o act_move.o act_obj.o act_wiz.o \
          alias.o ban.o clan.o comm.o const.o db.o db2.o effects.o fight.o \
          flags.o handler.o healer.o interp.o note.o lookup.o magic.o magic2.o \
	  music.o obmagic.o recycle.o save.o scan.o skills.o special.o \
          tables.o update.o bit.o mem.o olc.o olc_act.o olc_save.o string.o \
	  obskill.o romobj.o obdb.o critical.o noble.o obmagsys.o language.o
               

obliv: $(O_FILES)
	rm -f obliv
	$(CC) $(L_FLAGS) -o obliv $(O_FILES)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<

