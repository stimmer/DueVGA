#!/usr/bin/python
import Image
import sys

if len(sys.argv)<3:
  print "usage: makeanim.py infile.gif outfile.dat"

p=[]
for r in xrange(8):
  for g in xrange(8):
    for b in xrange(4):
      p+=[ int((r*255.0)/7.0),int((g*255.0)/7.0),int((b*255.0)/3.0)]

pal=Image.new("P", (1,1))
pal.putpalette(p)

im=Image.open(sys.argv[1])
f=open(sys.argv[2],"wb")


while 1:
  print im.tell()  
  imp=im.copy()
  nim=imp.convert("RGB").resize((320,240),Image.ANTIALIAS).quantize(palette=pal)
  for y in xrange(240):
    for x in xrange(320):
      f.write(chr(nim.getpixel((x,y))))
  try:
    im.seek(im.tell()+1)
  except EOFError:
    break;
