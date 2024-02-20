def RGBToWord(r,g,b):
	rvalue=0
	rvalue = rvalue + (g>>5)	
	rvalue = rvalue + ((g & 7)<< 13)
	rvalue = rvalue + ((r>>3)<<8)
	rvalue = rvalue + ((b >> 3) << 3)
	return rvalue

from PIL import Image
from os import sys
def main():
	args=sys.argv
	if (len(args) != 2):
		print("incorrect usage, please pass name of bmp to program")
		return -1
	
	ImageFileName=args[1]
	im=Image.open(ImageFileName)
	print(im.format,im.size,im.mode)
	pixels=list(im.getdata())
	for px in pixels:
		print(RGBToWord(px[0],px[1],px[2]),end=',')
	
if __name__ == "__main__":
	main()