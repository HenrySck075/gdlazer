# Script to turn all the images to a BMFont
# Because Geode does not have directory preservation feature (which i dont think cocos2d even have)
import sys, os
from PIL import Image

filename = sys.argv[1] 

struct = f"""info face="osu!font" size=100 bold=0 italic=0 charset="" unicode=1 stretchH=100 smooth=1 aa=4 padding=0,0,0,0 spacing=4,4 outline=0
common lineHeight=100 base=83 scaleW=1024 scaleH=1024 pages=1 packed=0 alphaChnl=0 redChnl=4 greenChnl=4 blueChnl=4
page id=0 file="{filename}_0.png"
chars count="""

chars = ""
charcount = 0
bmsize = [0,0]
bmimgs = []
offset = [0,0]
h = 0
c = 0
for i in os.listdir():
    if filename+"_0.png" in i: continue
    if ".png" not in i: continue
    print(i)
    img = Image.open(i)
    if (offset[0]+img.size[0]>1024):
        offset[0]=0
        offset[1]+=img.size[1]
    bmimgs.append((img,(offset[0],offset[1])))
    offset[0]+=img.size[0]
    if (offset[0]>1024):
        offset[0]=0
        offset[1]+=img.size[1]
    bmsize[0] = max(bmsize[0],offset[0])
    bmsize[1] = max(bmsize[1],offset[1]+img.size[1])
    charcount+=1
    chars+=f"char id={56000+charcount} x={offset[0]} y={offset[1]} width={img.size[0]} height={img.size[1]} xoffset=0 yoffset=0 xadvance=0 page=0 chnl=15"+"\n"
bmimg = Image.new("RGBA",tuple(bmsize),(0,0,0,0))
for i in bmimgs:
    bmimg.paste(i[0],i[1]) 
bmimg.save(filename+"_0.png")
open(filename+".fnt","w").write(struct+str(charcount)+"\n"+chars)