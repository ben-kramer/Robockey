Notes on PCB etching:
1. Make sure you are printing a mirror of the board in EAGLE.
2. print consecutive boards on different parts of the page to save transparencies
	a. Do a test print: put a sharpie mark on the top-side, upper left corner of the page to see which way the printer takes paper
	b. Alternatively, group everything on 1 page first! Prepare singles (pages with just 1
	   design per) and full pages. Print the full page first, and reprint any extras. Make sure
	   singles are in different spots.

issues:
1. M2 needs a jump to ground
2. where does mRF go?



etching method
0. designing
	a. for single-layer boards, design circuit using the bottom layer. Wires are on the sides
	   opposite to the things you are mounting.
1. printing
	a. do a test print to make sure you know what direction the printer prints
	b. from EAGLE, make a pdf (or JPEG?) of the circuit that is BLACK and NOTNOTNOT MIRRORED
		i. look into using JPEGs so multiple pictures can be merged into 1 sheet
	c. take a transparency and mark it with a k or other non-symmetrical letter on the top left
	d. print out the circuits
		i. top and bottom layers
		ii. pad layer
		iii. hole layer
	e. fill in any holes with a fine-tip sharpie
	f. cut out circuits using scissors or xacto. Place circuit on top of board and cut (xacto works nicely for this). Mark anything else (like holes) that is too thin with a sharpie
2. Etching step 1 - photodevelopment
	a. clean transparency first with a duster can and water thorougly.
	b. minimizing fingerprints, wet a lintless towel/cloth with water and gently
	   rub it across the top of the circuit transparency. Make sure to spread the
	   water evenly.
	c. turn off lights, turn on red lights
	d. unpeel protective layer from copper and stick the transparency to the board using the water on the board. Compress the two to ensure they stick together.
	d. Place the board under glass and turn the LED light on for 10 minutes.
	e. Remove board from LED light, turning it off, and place in etching solution. With glove,
	   wiggle board around. This should take no more than 1.5 minutes. If it takes longer, add
	   more positive developer. Towards the end you might have to take one side of the board out
	   and swish around only a fraction of it because the photoresist doesn't develop perfectly
	   evenly.
	f. wash board under cold water
	g. dry board
	h. fill in errors with sharpie
3. Etching step 2 - etching
	a. place board in cupric acid. Ensure cupric acid is a nice green. If it's yellow or brown,
	   add more H202.
	b. swish with the large end of a funnel until board etches.
	c. Note: if etching is still slow after adding H202, add some HCl
	d. rinse board in cold water
	e. rinse board with acetone.
		i. put a little acetone on top of the board, swish it around, and remove it with a sponge.
	f. check for errors
		i. label each error found on the board. Put identifying marks below or to the right of the area, unless doing so is impossible.

todo:

update pinmap

1. add out for breakbeam led
2. add switch bank
3. add bicolor led spot
4. switch motor signal connectors

add enable pins for bridges