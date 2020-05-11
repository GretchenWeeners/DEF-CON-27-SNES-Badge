.include "hdr.asm"

.section ".rodata1" superfree

patterns:
.incbin "map_512_512.pic"
patterns_end:

gfxpsrite:
.incbin "sprites.pic"
gfxpsrite_end:



.ends

.section ".rodata2" superfree
map:
.incbin "map_512_512.map"
map_end:

palette:
.incbin "map_512_512.pal"
palette_end:

palsprite:
.incbin "sprites.pal"
palsprite_end:

.ends

.section ".rodata3" superfree

snesfont:
.incbin "pvsneslibfont.pic"


soundbrr:
.incbin "tada.brr"
soundbrrend:

.ends
