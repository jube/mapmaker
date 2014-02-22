TARGETS=diamond_square_simple.png perlin_simple.png voronoi_simple.png

all: $(TARGETS)

%.pnm: %.yml
	mapmaker $<

%.png: %.pnm
	convert $< $@

clean:
	rm -f $(TARGETS)
