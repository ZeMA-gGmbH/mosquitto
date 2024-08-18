include ../../config.mk

.PHONY : all binary check clean reallyclean test install uninstall

PLUGIN_NAME=wamo
LOCAL_CPPFLAGS=-I../../src/ -DWITH_CJSON

OBJS=	\
		
		json_help.o \
		wamo.o 
		

ifeq ($(WITH_CJSON),yes)
ALL_DEPS:= binary
else
ALL_DEPS:=
endif
else
ALL_DEPS:=
endif

all : ${ALL_DEPS}
binary : ${PLUGIN_NAME}.so

${PLUGIN_NAME}.so : ${OBJS}
	${CROSS_COMPILE}${CC} $(PLUGIN_LDFLAGS) -fPIC -shared $^ -o $@ -lcjson



json_help.o : json_help.c 
	${CROSS_COMPILE}${CC} $(LOCAL_CPPFLAGS) $(PLUGIN_CPPFLAGS) $(PLUGIN_CFLAGS) -c $< -o $@

wamo.o : wamo.c 
	${CROSS_COMPILE}${CC} $(LOCAL_CPPFLAGS) $(PLUGIN_CPPFLAGS) $(PLUGIN_CFLAGS) -c $< -o $@


reallyclean : clean
clean:
	-rm -f *.o ${PLUGIN_NAME}.so *.gcda *.gcno

check: test
test:

install: all
ifeq ($(WITH_CJSON),yes)
	$(INSTALL) -d "${DESTDIR}$(libdir)"
	$(INSTALL) ${STRIP_OPTS} ${PLUGIN_NAME}.so "${DESTDIR}${libdir}/${PLUGIN_NAME}.so"
endif
endif

uninstall :
	-rm -f "${DESTDIR}${libdir}/${PLUGIN_NAME}.so"
