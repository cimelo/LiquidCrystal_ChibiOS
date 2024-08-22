# RT Shell files.
LCDSRC = $(CHIBIOS)/os/hal/lib/lcd/liquidcrystal.c

LCDINC = $(CHIBIOS)/os/hal/lib/lcd

# Shared variables
ALLCSRC += $(LCDSRC)
ALLINC  += $(LCDINC)
