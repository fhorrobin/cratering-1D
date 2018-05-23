#!/usr/bin/env python
with open("impactors.txt", "w") as file:
	for i in range(40):
		file.write("%f\n" % ((i + 1) * 0.01))
