import sys, os

fntfilepath = os.path.join(os.path.curdir,sys.argv[1])
fntfolderpath = os.path.dirname(fntfilepath)

content = open(fntfilepath,'rb')

content.seek(4)

out = ""

pages = 1
def from_bytes_little(b: bytes):
    return int.from_bytes(b,"little")
def from_bytes_big(b: bytes):
    return int.from_bytes(b,"big")

while True:
    blockTypeRaw = content.read(1)
    if (blockTypeRaw.__len__() == 0): 
        break
    blockType = from_bytes_little(blockTypeRaw)
    blockSizeRaw = content.read(4)
    if (blockSizeRaw.__len__() < 4): 
        break
    blockSize = from_bytes_little(blockSizeRaw)

    # less indentations than match
    # fight me

    # also there's currently no record of these blocks swapping places
    if (blockType == 1):
        size = from_bytes_little(content.read(2))
        content.seek(2,1)
        sh = from_bytes_little(content.read(2))
        aa = from_bytes_little(content.read(1))
        content.seek(7,1)

        fontName = content.read(blockSize-15).decode()
        "BE CAREFUL! If you left the null character, the bushes will hide all the facts!"
        # (specifically it makes notepad autodetects the file into utf-16 BE)
        content.seek(1,1)
        out += (
            'info face="{name}" size={s} bold=0 italic=0 charset="" unicode=1 stretchH={sh} smooth=1 aa={aa} padding=0,0,0,0 spacing=4,4 outline=0\n'
            .format(
                name = fontName,
                s = size,
                sh = sh,
                aa = aa
            )
        )
    elif (blockType == 2):
        lh = from_bytes_little(content.read(2))
        base = from_bytes_little(content.read(2))
        w = from_bytes_little(content.read(2))
        h = from_bytes_little(content.read(2))
        pages = from_bytes_little(content.read(2))
        out += (
            ("common lineHeight={lh} base={base} scaleW={w} scaleH={h} pages={p} packed={packed}"
            "alphaChnl={a} redChnl={r} greenChnl={g} blueChnl={b}\n")
            .format(
                lh = lh,
                base = base,
                w = w,
                h = h,
                p = pages,
                packed = (from_bytes_little(content.read(1)) & 1 << 7),
                a = from_bytes_little(content.read(1)), 
                r = from_bytes_little(content.read(1)), 
                g = from_bytes_little(content.read(1)), 
                b = from_bytes_little(content.read(1))
            )
        )
    elif (blockType == 3):
        for i in range(pages):
            length = int(blockSize/pages)
            bitmapFile = content.read(
                length # filenameLength
            ).decode()[:length-1]
            out+='page id={} file="{}"\n'.format(i, bitmapFile)
    elif (blockType == 4):
        characters = int(blockSize/20)
        out+=f"chars count={characters}"+"\n"
        charInfo = []
        for i in range(characters):
            charInfo.append(
                "char id={} x={} y={} width={} height={} xoffset={} yoffset={} xadvance={} page={} chnl={}"
                .format(
                    from_bytes_little(content.read(4)), # id
                    from_bytes_little(content.read(2)), # x
                    from_bytes_little(content.read(2)), # y
                    from_bytes_little(content.read(2)), # width
                    from_bytes_little(content.read(2)), # height
                    from_bytes_little(content.read(2)), # xoffset
                    from_bytes_little(content.read(2)), # yoffset
                    from_bytes_little(content.read(2)), # xadvance
                    from_bytes_little(content.read(1)), # page
                    from_bytes_little(content.read(1))  # channel
                )
            )
        out+="\n".join(charInfo)
    elif (blockType == 5):
        kernings = int(blockSize/10)
        kerningInfo = []
        out+=f"kernings count={kernings}"+"\n"
        for i in range(kernings):
            kerningInfo.append(
                "kerning first={} second={} amount={}"
                .format(
                    from_bytes_little(content.read(4)),
                    from_bytes_little(content.read(4)),
                    from_bytes_little(content.read(2))
                )
            )
        out+="\n".join(kerningInfo)

f = os.path.basename(fntfilepath)
open(os.path.join(fntfolderpath, f[:f.rfind(".")]+".fnt"),"w",encoding="utf-8").write(out)
